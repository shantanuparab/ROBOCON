// This file stores the List of Constants to be used
#include "Constants.h"

// Use a BitField
// To Store Several Boolean Variables
// And save some memory
// In Return
struct BitField
{
   // Controls if the Junction 1 is
   // Working
   bool JunctionForwardEnable : 1;

   // Controls if we need to Realign Bot
   // And if Our Operation Succeeded
   bool JunctionForwardDetected : 1;
};

volatile BitField g_bools;

#include "MotorController.h"

MotionController g_motion;

void setupMotionController()
{
   // Set All the Different Motors
   g_motion.FrontLeftMotor({DIRECTION_FL, PWM_FL});
   g_motion.FrontRightMotor({DIRECTION_FR, PWM_FR});
   g_motion.BackLeftMotor({DIRECTION_BL, PWM_BL});
   g_motion.BackRightMotor({DIRECTION_BR, PWM_BR});

   g_motion.StraightDirection = MotionDirection::FORWARD;
   g_motion.MaxPWMAllowed     = 255;
   g_motion.StraightLinePWM(50);
}

void LSA08JunctionForwardISR()
{
   if (!g_bools.JunctionForwardEnable)
      return;

   // As the Junction was Enabled
   // And a Junction was Found
   g_bools.JunctionForwardDetected = true;
}

#include "LSA08s.h"

LSA08s g_line_sensors;

void setupLineSensor()
{
   // Set Baud Rate for UART Communication
   // With Line Sensor
   Serial3.begin(9600);

   // This is the Line Sensor for Forward Motion
   g_line_sensors.LSA08Forward({&Serial3, FORWARD_ENABLE, FORWARD_JUNCTION, FORWARD_ADDRESS});

   // This is the Line Sensor for Sideways Motion
   g_line_sensors.LSA08Sideways({&Serial3, SIDEWAYS_ENABLE, SIDEWAYS_JUNCTION, SIDEWAYS_ADDRESS});

   // This forces the Line Sensor to test the connection
   // If any Line Sensor is not connected
   // Displays Error Message
   g_line_sensors.TestConnection();

   // By default, forward Line Sensor Selected
   g_line_sensors.Selected = LSA08Selected::FORWARD;

   // Attach Interrupt to Junction Pin
   g_line_sensors.SetJunctionPinAsInterrupt(LSA08Selected::FORWARD, LSA08JunctionForwardISR, RISING);
}

// Applies PID to LSA08 Reading
int16_t LineSensorPIDCorrection(byte const p_reading)
{
   static int8_t prev_error = 0 /*Initialised only Once*/;

   // Application of PID
   // Requires 2 Constants
   // KP and KD
   float const static constexpr kp  = 1.75;
   int8_t const static constexpr kd = 26;

   // Note that 35 is the Middle Value between the Max and Min
   // Range of 0 - 70
   // In Mode 2
   int8_t static const set_point = 35;

   // Calculate Error by Subtracting from Set Point
   int8_t const cur_error = (set_point - (int8_t)p_reading);

   // Find Correction by Applying PID
   int16_t const correction = (kp * cur_error) + (kd * (cur_error - prev_error));

   prev_error = cur_error;

   return correction;
}

#include <Encoder.h>
Encoder EncoderFL{ENC_FL_U, ENC_FL_D};
Encoder EncoderFR{ENC_FR_U, ENC_FR_D};

uint32_t g_loop_time;

// The setup() function runs once each time the micro-controller starts
void setup()
{
   Serial.begin(9600);

   // Set-up the Motors
   setupMotionController();

   // Set-up the Line Sensor Configuration
   setupLineSensor();

   // Initially The Bot must be Halted
   g_motion.halt();

   g_bools.JunctionForwardEnable   = false;
   g_bools.JunctionForwardDetected = false;

   g_loop_time = micros(); /*Called Only At Start*/
}

// Add the main program code into the continuous loop() function
void loop()
{
   // Check if 0.5ms have passed between checks
   if ((micros() - g_loop_time) <= 500)
      return;

   // Portion dealing with the Encoder
   {
      // Find the Return Type
      // Of Encoder::read
      // For details refer to
      // https://stackoverflow.com/questions/5580253/get-return-type-of-member-function-without-an-object
      using EncoderCount = int32_t;

      EncoderCount const PositionFLAbs = abs(EncoderFL.read());
      EncoderCount const PositionFRAbs = abs(EncoderFR.read());
      EncoderCount const PositionAvg   = (PositionFLAbs + PositionFRAbs) / (EncoderCount)2;

      Serial.print(F("Encoder Readings: "));
      Serial.print(PositionFLAbs);
      Serial.print(F("  "));
      Serial.print(PositionFRAbs);
      Serial.print(F("  "));
      Serial.print(PositionAvg);
      Serial.println();

      // TODO: Find the Max Constant
      // After which to enable Junction
      if (!g_bools.JunctionForwardEnable && g_motion.StraightDirection == MotionDirection::FORWARD &&
          PositionAvg > 500 /*Constant*/)
      {
         g_bools.JunctionForwardEnable = true;
      }
   }

   // Detected a Junction
   // Lets Re-Align
   {
      if (g_bools.JunctionForwardDetected)
      {
         Serial.println(F("Forward Junction Detected. Starting Turn Procedure"));
         // Toggle State to Not Detected
         g_bools.JunctionForwardDetected = false;
         // Disable Junction Detections
         g_bools.JunctionForwardEnable = false;

         // Moves the Bot Forward
         g_motion.moveStraight();
         // Reset Encoders
         EncoderFL.write(0);
         EncoderFR.write(0);

         // Note that it was Noticed
         // That in many scenarios
         // That while a Junction was Detected
         // The LSA08 behind was still not over the Junction
         // As such we need to bring it ahead
         while (true)
         {
            using EncoderCount = int32_t;

            EncoderCount const PositionFLAbs = abs(EncoderFL.read());
            EncoderCount const PositionFRAbs = abs(EncoderFR.read());
            EncoderCount const PositionAvg   = (PositionFLAbs + PositionFRAbs) / (EncoderCount)2;

            Serial.print(F("Turn Encoder Readings: "));
            Serial.print(PositionFLAbs);
            Serial.print(F("  "));
            Serial.print(PositionFRAbs);
            Serial.print(F("  "));
            Serial.print(PositionAvg);
            Serial.println();

            // TODO: Find the Max Constant
            // Till Which to Go
            if (PositionAvg > 200)
               break;
         }
         Serial.println(F("Turn Done"));
         // Change Motion of Direction to Right
         g_motion.StraightDirection = MotionDirection::RIGHT;
         // Enable Sideways Line Sensor
         g_line_sensors.Selected = LSA08Selected::SIDEWAYS;

         // Reset Encoders
         EncoderFL.write(0);
         EncoderFR.write(0);

         // Halt the Robot till the Line Sensor takes a decision
         g_motion.halt();
      }
   }

   // Portion Dealing with Line Sensor
   // It Reads the Value
   // And Makes corrections accordingly
   {
      byte const lsa08_reading = g_line_sensors.read();
      // Verify if LSA08 Reading is in given constraint of UART Mode 2
      if (lsa08_reading >= 0 && lsa08_reading <= 70)
      {
         // Apply PID to LSA08 Value to find PWM Turn Value
         int16_t const pwm_correction = LineSensorPIDCorrection(lsa08_reading);
         g_motion.moveStraightWithCorrection(pwm_correction);

         Serial.print(F("Reading :"));
         Serial.print(lsa08_reading);
         Serial.print(F("\tCorrection PWM :"));
         Serial.print(pwm_correction);
         Serial.println();
      }
      // If No Proper Reading Detected,
      // Halt
      else
      {
         g_motion.halt();
      }
   }

   // Reset the cur_time to micros
   g_loop_time = micros();
}

/*

	This Code Illustrates the Last Bot
	Please ignore

  // This is an Encoder.h definition
  // This Must Always be Defined Before Using Encoder.h
  // Danger:- Delete this if using MORE than ONE Interrupt Pin
  // Note:- Check Again if Multiple Interrupts Used
  #define ENCODER_OPTIMIZE_INTERRUPTS

  // Dare not Use Encoder without using Libraries
  // For Details about this Library Check
  // http://www.pjrc.com/teensy/td_libs_Encoder.html
  #include <Encoder.h>

  // Create Encoder Object
  // NOTE
  // Both PINS provided to Encoder
  // Must be Interrupt Pins
  // If they are not,
  // We shall face a Performance Penalty
  // Check List of Interrupt Pins for your board

  Encoder g_encoder(ENCODER_A, ENCODER_B);
  // Stores the Previous Number of Pulses
  // Volatile ensures that this variable is not
  // Optimised Away
  // volatile int32_t g_prev_number_of_pulses = 0;

#include "MotorController.h"

// FRONT LEFT
const SingleMotorController g_motor_fl(DIRECTION_FL, PWM_FL, LOW);
// FRONT RIGHT
const SingleMotorController g_motor_fr(DIRECTION_FR, PWM_FR, HIGH);
// BACK LEFT
const SingleMotorController g_motor_bl(DIRECTION_BL, PWM_BL, LOW);
// BACK RIGHT
const SingleMotorController g_motor_br(DIRECTION_BR, PWM_BR, HIGH);

// Note that
// +x -> Right
// +y -> Forward
// +z -> Counter Clock Wise
void moveBot(const int16_t p_pwmx, const int16_t p_pwmy, const int16_t p_pwmrot)
{
	int16_t flpwm = +p_pwmx + p_pwmy - p_pwmrot;
	int16_t frpwm = -p_pwmx + p_pwmy + p_pwmrot;
	int16_t blpwm = -p_pwmx + p_pwmy - p_pwmrot;
	int16_t brpwm = +p_pwmx + p_pwmy + p_pwmrot;

	g_motor_fl.setSpeed((flpwm));
	g_motor_fr.setSpeed(frpwm);
	g_motor_bl.setSpeed(blpwm);
	g_motor_br.setSpeed(brpwm);
}
  void moveForward(const int16_t p_speed)
  {
  moveBot(0, p_speed, 0);
  Serial.println("Move Straight");
  }
  void moveReverse(const int16_t p_speed)
  {
  moveBot(0, -p_speed, 0);
  Serial.println("Move Reverse");
  }

void wait()
{
	moveBot(0, 0, 0);
	// Serial.println("Move Halt");
}
 void moveClockwise(const int16_t p_speed)
  {
  moveBot(0, 0, -p_speed);
  Serial.println("Move Clockwise");
  }
  void moveCounterClockWise(const int16_t p_speed)
  {
  moveBot(0, 0, p_speed);
  Serial.println("Move Counter Clockwise");
  }
  void moveRight(const int16_t p_speed)
  {
  moveBot(p_speed, 0, 0);
  Serial.println("Move Right");
  }
  void moveLeft(const int16_t p_speed)
  {
  moveBot(-p_speed, 0, 0);
  Serial.println("Move Left");
  }


// Move Robot depending on the PWM in Straight Line
void movePlatform(const int pwm_sway)
{
	if (g_turn == BOT_FORWARD)
		moveBot(pwm_sway, 200, 0);
	else
		moveBot(200, g_turn * pwm_sway, 0);
}
*/

// This file stores the List of Constants to be used
#include "Constants.h"

#include "MotorController.h"
MotionController g_motion;

void setupMotionController()
{
	// Set All the Different Motors
	g_motion.setFrontLeftMotor({DIRECTION_FL, PWM_FL, LOW});
	g_motion.setFrontRightMotor({DIRECTION_FR, PWM_FR, HIGH});
	g_motion.setBackLeftMotor({DIRECTION_BL, PWM_BL, LOW});
	g_motion.setBackRightMotor({DIRECTION_BR, PWM_BR, HIGH});

	// Initially Will Consider Straight As Forward Direction
	g_motion.setStraightAsForward();

	// Set the Straight Line PWM
	g_motion.setStraightLinePWM(90);

	// Set the Maximum PWM Value the Motors will be subjected to
	g_motion.setMaxPWMAllowed(150);
}

#include "LSA08.h"

LSA08s g_line_sensors;

void setupLineSensor()
{
	// Set Baud Rate for UART Communication
	// With Line Sensor
	Serial3.begin(9600);

	// This is the Line Sensor for Forward Motion
	g_line_sensors.setLSA08Forward({&Serial3, SERIAL_ENABLE_FORWARD, JUNCTION_PULSE_FORWARD});

	// This is the Line Sensor for Sideways Motion
	g_line_sensors.setLSA08Sideways({&Serial3, SERIAL_ENABLE_SIDEWAYS, JUNCTION_PULSE_SIDEWAYS});

	// This forces the Line Sensor to test the connection
	// If any Line Sensor is not connected
	// Displays Error Message
	g_line_sensors.testConnection();

	// By default, forward Line Sensor Selected
	g_line_sensors.setForwardActive();
}

#include "IRSensor.h"
IRSensors g_ir_sensors{IR_INPUT_LEFT, IR_INPUT_RIGHT, IR_INPUT_FRONT};

// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(9600);

	// Set-up the Motors
	setupMotionController();

	// Set-up the Line Sensor Configuration
	setupLineSensor();

	// Set-up the IR Sensor
	// By Default IR Must detect Sideways line
	g_ir_sensors.setSidewaysActive();

	// Initially The Bot must be Halted
	g_motion.halt();
}

// Add the main program code into the continuous loop() function
void loop()
{
	// Portion Dealing with Line Sensor
	// It Reads the Value
	// And Makes corrections accordingly
	{
		const byte lsa08_reading = g_line_sensors.readLineSensor();
		// Verify if LSA08 Reading is in given constraint of UART Mode 2
		if (lsa08_reading >= 0 && lsa08_reading <= 70)
		{
			const int16_t kp = 3;
			// Note that 35 is the Middle Value between the Max and Min
			// Range of 0 - 70
			// Apply PID to LSA08 Value to find PWM Turn Value
			const int16_t pwm_correction = kp * (35 - lsa08_reading);

			g_motion.moveStraightWithCorrection(pwm_correction);
			Serial.print("Correction PWM\t:");
			Serial.print(pwm_correction);
			Serial.print("\tReading\t:");
			Serial.print(lsa08_reading);
			Serial.println();
		}
		// If No Proper Reading Detected,
		// Halt
		else
		{
			g_motion.halt();
		}
	}

	if (g_ir_sensors.isLeftIRDetected())
	{
		Serial.println("Left IR Line Detect");

		// Enable Front IR
		// Disable Left And Right IR Sensor
		g_ir_sensors.setForwardActive();

		// As Bot Must Go Sideways
		// Set the Sideways Line Sensor As
		// Active One
		g_line_sensors.setSidewaysActive();

		g_motion.setStraightAsLeft();
		g_motion.moveStraight();
	}
	else if (g_ir_sensors.isRightIRDetected())
	{
		Serial.println("Right IR Line Detect");

		// Enable Front IR
		// Disable Left And Right IR Sensor
		g_ir_sensors.setForwardActive();

		// As Bot Must Go Sideways
		// Set the Sideways Line Sensor As
		// Active One
		g_line_sensors.setSidewaysActive();

		g_motion.setStraightAsRight();
		g_motion.moveStraight();
	}
	else if (g_ir_sensors.isForwardIRDetected())
	{
		Serial.println("Front IR Line Detected");

		// Disable Front IR
		// Enable Left And Right IR Sensor
		g_ir_sensors.setSidewaysActive();

		// As Bot Must Go Forward
		// Set the Forward Line Sensor As
		// Active One
		g_line_sensors.setForwardActive();

		g_motion.setStraightAsForward();
		g_motion.moveStraight();
	}
}

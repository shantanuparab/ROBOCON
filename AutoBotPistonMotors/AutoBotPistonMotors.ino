#include "Constants.h"

// KP Value for PWM Control
double constexpr const KP_PWM_CONTROL = 5.0 / 20000.0;
// Base PWM at which to SPIN
byte constexpr const LEG_BASE_PWM = 127;
// Number of Revolutions both diagonals undergo
int32_t constexpr const REVOLUTIONS = 3;

#include <Encoder.h>
Encoder g_enc_fl{ENC_FL_U, ENC_FL_D};
Encoder g_enc_fr{ENC_FR_U, ENC_FR_D};
Encoder g_enc_bl{ENC_BL_U, ENC_BL_D};
Encoder g_enc_br{ENC_BR_U, ENC_BR_D};

// Keep IR Sensor Pins Below 64
//#include "IRSensor.h"
// IRSensor const g_ir_fl{19, LOW};
// IRSensor const g_ir_fr{20, LOW};
// IRSensor const g_ir_bl{18, LOW};
// IRSensor const g_ir_br{21, LOW};

#include "MotionController.h"
MotionController g_motion{
    {DIRECTION_FL, PWM_FL}, {DIRECTION_FR, PWM_FR}, {DIRECTION_BL, PWM_BL}, {DIRECTION_BR, PWM_BR}};

bool g_micro_controller_restart = false;

// For Further Details regarding Restart
// Read
// https://www.reddit.com/r/Teensy/comments/7r19uk/reset_and_reboot_teensy_lc_via_code/
//#define SCB_AIRCR (*(volatile uint32_t*)0xE000ED0C) // Application Interrupt and Reset Control location
void SoftRestart()
{
   noInterrupts();         // Disable Interrupts
   Serial.end();           // clears the serial monitor  if used
   SCB_AIRCR = 0x05FA0004; // write value for restart
   // Halt till Restart Occurs
   while (true)
   {
   }
}

void setup()
{
   Serial.begin(9600);
   g_motion.halt();

   // Added a delay of 2s
   // To Ensure that the Start is not
   // Instantaneous
   delay(2000);
   // Pressing Button Restarts Teensy
   pinMode(RESTART_MICRO_CONTROLLER_PIN, INPUT_PULLDOWN);
   attachInterrupt(digitalPinToInterrupt(RESTART_MICRO_CONTROLLER_PIN), SoftRestart, RISING);
   g_motion.halt();
   Serial.println(F("Starting Motion"));
   InitAllLegsPWM(LEG_BASE_PWM);
   g_motion.MaxSpeedAllowed = 90;
   //TestOmni(-50);
   MoveByCounts(-80,98'000);
}

void loop() {}

int16_t g_pwm_fl;
int16_t g_pwm_fr;
int16_t g_pwm_bl;
int16_t g_pwm_br;

void SyncAutoBot()
{
   InitAllLegsPWM(LEG_BASE_PWM);

   // while (true)
   //{
   //   g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);

   //   if (g_pwm_fl != LEG_BASE_PWM && g_pwm_br != LEG_BASE_PWM && (millis() - last_sync_fl_br) > 0 &&
   //       g_ir_fl_br.isDetected())
   //   {
   //      Serial.println(F("Halting FL&BR"));
   //      g_pwm_fl = 0 /*LEG_BASE_PWM*/;
   //      g_pwm_br = 0 /*LEG_BASE_PWM*/;
   //   }
   //   // if (g_pwm_bl != LEG_BASE_PWM && g_pwm_fr != LEG_BASE_PWM && (millis() - last_sync_fr_bl) > 0 &&
   //   //    g_ir_fr_bl.isDetected())
   //   //{
   //   //   Serial.println(F("Slowing BL&FR"));
   //   //   g_pwm_bl = LEG_BASE_PWM;
   //   //   g_pwm_fr = LEG_BASE_PWM;
   //   //}
   //   if (g_pwm_fl != LEG_BASE_PWM && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_fl.isDetected())
   //   {
   //      Serial.println(F("Halting FL"));
   //      g_pwm_fl = 0;
   //   }
   //   if (g_pwm_fr != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR && g_ir_fr.isDetected())
   //   {
   //      Serial.println(F("Halting FR"));
   //      g_pwm_fr = 0;
   //   }
   //   if (g_pwm_br != 0 && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_br.isDetected())
   //   {
   //      Serial.println(F("Halting BR"));
   //      g_pwm_br = 0;
   //   }
   //   if (g_pwm_bl != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR && g_ir_bl.isDetected())
   //   {
   //      Serial.println(F("Halting BL"));
   //      g_pwm_bl = 0;
   //   }

   //   g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
   //   if (g_pwm_bl == 0 && g_pwm_br == 0 && g_pwm_fl == 0 && g_pwm_fr == 0)
   //   {
   //      Serial.println("Sync Motors Brought");
   //      break;
   //   }
   //}
}

void PerformFRBLSync()
{
   auto const fr_counts = g_enc_fr.read();
   auto const bl_counts = g_enc_bl.read();

   auto const diff = fr_counts - bl_counts;

   // Code to Sync Motors
   g_pwm_fr = LEG_BASE_PWM + KP_PWM_CONTROL * diff;
   g_pwm_bl = LEG_BASE_PWM - KP_PWM_CONTROL * diff;

   // By Problem Statement for Syncing Purposes
   // Motors must always move
   // Ahead
   // So Constrain between 0 & 255
   // As -ve Implies Backwards
   g_pwm_fr = constrain(g_pwm_fr, 0, 255);
   g_pwm_bl = constrain(g_pwm_bl, 0, 255);
}
void PerformFLBRSync()
{
   auto const fl_counts = g_enc_fl.read();
   auto const br_counts = g_enc_br.read();

   auto const diff = fl_counts - br_counts;

   // Code to Sync Motors
   g_pwm_fl = LEG_BASE_PWM + KP_PWM_CONTROL * diff;
   g_pwm_br = LEG_BASE_PWM - KP_PWM_CONTROL * diff;

   // By Problem Statement for Syncing Purposes
   // Motors must always move
   // Ahead
   // So Constrain between 0 & 255
   // As -ve Implies Backwards
   g_pwm_fl = constrain(g_pwm_fl, 0, 255);
   g_pwm_br = constrain(g_pwm_br, 0, 255);
}

uint16_t g_counts_single_diag_done = 0;

void MoveAutoBotSingleDiagsOn()
{
   MoveAutoBotFLBRSingleDiagOn();
   MoveAutoBotFRBLSingleDiagOn();
   ++g_counts_single_diag_done;
   CheckHaltingConditionByCountsSingleDiag();
}
void MoveAutoBotFLBRSingleDiagOn()
{
   // last_sync_fl_br = millis();
   // Initially Speed Up Wheels
   InitAllLegsPWM(LEG_BASE_PWM);
   // Run till Both FL & BR finish 1 Revolution
   while (true)
   {
      Serial.print("FL: ");
      Serial.print(g_enc_fl.read());
      Serial.print(" ");
      Serial.print(g_pwm_fl);
      Serial.print(" ");
      Serial.print("FR: ");
      Serial.print(g_enc_fr.read());
      Serial.print(" ");
      Serial.print(g_pwm_fr);
      Serial.print(" ");
      Serial.print("BL: ");
      Serial.print(g_enc_bl.read());
      Serial.print(" ");
      Serial.print(g_pwm_bl);
      Serial.print(" ");
      Serial.print("BR: ");
      Serial.print(g_enc_br.read());
      Serial.print(" ");
      Serial.print(g_pwm_br);
      Serial.println();

      PerformFLBRSync();
      // Keep Moving Forward
      g_motion.moveLegs(g_pwm_fl, 0, 0, g_pwm_br);
      //if (g_enc_fl.read() / ENC_PER_REV > 1)
      //   g_pwm_fl = 0;
      // If Both of them Complete One Revolution
      // Halt Bot
      // FL_BR Single Sync Done
      if (g_enc_fl.read() / ENC_PER_REV > 1 && g_enc_br.read() / ENC_PER_REV > 1)
      {
         g_motion.halt();

         // Reset Encoder Values
         g_enc_fl.write(0);
         g_enc_br.write(0);
         // Terminate LOOP
         break;
      }
   }
   // As One Leg Motion Over
   // Halt Both Legs
   // Go to Next Leg
   // To Restart
   g_motion.halt();
}
void MoveAutoBotFRBLSingleDiagOn()
{
   // last_sync_fr_bl = millis();
   // Initially Slow down Wheels
   InitAllLegsPWM(LEG_BASE_PWM);
   // Run till Both Motors are Not in Sync
   while (true)
   {
      Serial.print("FL: ");
      Serial.print(g_enc_fl.read());
      Serial.print(" ");
      Serial.print(g_pwm_fl);
      Serial.print(" ");
      Serial.print("FR: ");
      Serial.print(g_enc_fr.read());
      Serial.print(" ");
      Serial.print(g_pwm_fr);
      Serial.print(" ");
      Serial.print("BL: ");
      Serial.print(g_enc_bl.read());
      Serial.print(" ");
      Serial.print(g_pwm_bl);
      Serial.print(" ");
      Serial.print("BR: ");
      Serial.print(g_enc_br.read());
      Serial.print(" ");
      Serial.print(g_pwm_br);
      Serial.println();
      PerformFRBLSync();
      // Till they are in Sync, Keep Moving Forward
      g_motion.moveLegs(0, g_pwm_fr, g_pwm_bl, 0);

      // If Both of them Complete One Revolution
      // Halt Bot
      // FR_BL Single Sync Done
      if (g_enc_fr.read() / ENC_PER_REV > 1 || g_enc_bl.read() / ENC_PER_REV > 1)
      {
         g_motion.halt();
         // Reset Encoder Values
         g_enc_fr.write(0);
         g_enc_bl.write(0);
         // Terminate LOOP
         break;
      }
   }
   // As One Leg Motion Over
   // Halt Both Legs
   // Go to Next Leg
   // To Restart
   g_motion.halt();
}

// Check if the FL or FR or BL or BR Have Completed Required Revolutions
void CheckHaltingConditionByRevs()
{
   if ((g_enc_fl.read() / ENC_PER_REV) > REVOLUTIONS || (g_enc_bl.read() / ENC_PER_REV) > REVOLUTIONS ||
       (g_enc_fr.read() / ENC_PER_REV) > REVOLUTIONS || (g_enc_br.read() / ENC_PER_REV) > REVOLUTIONS)
   {
      HaltBotFinal();
   }
}
void CheckHaltingConditionByCountsSingleDiag()
{
   if (g_counts_single_diag_done > REVOLUTIONS)
   {

      HaltBotFinal();
   }
}
void HaltBotFinal()
{
   Serial.print(F("Halting Bot as counts done"));
   SyncAutoBot();
   g_motion.halt();
   // Reset All Encoders
   g_enc_bl.write(0);
   g_enc_br.write(0);
   g_enc_fl.write(0);
   g_enc_fr.write(0);
   // Ensure it Stops
   while (true)
   {
   }
}

void InitAllLegsPWM(int16_t const p_leg_pwm)
{
   g_pwm_fl = p_leg_pwm;
   g_pwm_fr = p_leg_pwm;
   g_pwm_bl = p_leg_pwm;
   g_pwm_br = p_leg_pwm;
}

void TestOmni(int16_t const pwm)
{
   // If any Legs goes in Opposite Direction than Required
   // Please Modify moveLegsDirect function in MotionController.h
   // Change Sign of Parameter
   // If Encoder Values are Negative, Invert PINS Please
   {
      Serial.print(F("FL Dir :"));
      Serial.print(DIRECTION_FL);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_FL);
      Serial.println();
      g_motion.moveLegs(pwm, 0, 0, 0);
      delay(200);
      Serial.print("FL Finished Counts\t:");
      Serial.print(g_enc_fl.read());
      Serial.println();
      // Reset FL Encoder
      g_enc_fl.write(0);
   }
   {
      Serial.print(F("FR Dir :"));
      Serial.println(DIRECTION_FR);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_FR);
      Serial.println();
      g_motion.moveLegs(0, pwm, 0, 0);
      delay(200);
      Serial.print("FR Finished Counts\t:");
      Serial.print(g_enc_fr.read());
      Serial.println();
      // Reset FR Encoder
      g_enc_fr.write(0);
   }
   {
      Serial.print(F("BL Dir :"));
      Serial.println(DIRECTION_BL);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_BL);
      Serial.println();
      g_motion.moveLegs(0, 0, pwm, 0);
      delay(500);
      Serial.print("BL Finished Counts\t:");
      Serial.print(g_enc_bl.read());
      Serial.println();
      // Reset BL Encoder
      g_enc_bl.write(0);
   }
   {
      Serial.print(F("BR Dir :"));
      Serial.println(DIRECTION_BR);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_BR);
      Serial.println();
      g_motion.moveLegs(0, 0, 0, pwm);
      delay(200);
      Serial.print("BR Finished Counts\t:");
      Serial.print(g_enc_br.read());
      Serial.println();
      // Reset BR Encoder
      g_enc_br.write(0);
   }
   g_motion.halt();
}

float FindRPM(float time)
{
   time = (((((float)time) / REVOLUTIONS) / 1000000));
   time = 60.0f / time;
   return time;
}

void MoveByCounts(int16_t const pwm, int32_t const counts)
{
   InitAllLegsPWM(pwm);
   g_motion.moveLegs(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
   while (true)
   {
      if (abs(g_enc_fl.read()) > abs(counts))
      {
         g_pwm_fl = 0;
      }
      if (abs(g_enc_bl.read()) > abs(counts))
      {
         g_pwm_bl = 0;
      }
      if (abs(g_enc_fr.read()) > abs(counts))
      {
         g_pwm_fr = 0;
      }
      if (abs(g_enc_br.read()) > abs(counts))
      {
         g_pwm_br = 0;
      }
      g_motion.moveLegs(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
      if (g_pwm_fl == 0 && g_pwm_bl == 0 && g_pwm_fr == 0 && g_pwm_br == 0)
      {
         break;
      }
      Serial.print("FL: ");
      Serial.print(g_enc_fl.read());
      Serial.print(" ");
      Serial.print(g_pwm_fl);
      Serial.print(" ");
      Serial.print("FR: ");
      Serial.print(g_enc_fr.read());
      Serial.print(" ");
      Serial.print(g_pwm_fr);
      Serial.print(" ");
      Serial.print("BL: ");
      Serial.print(g_enc_bl.read());
      Serial.print(" ");
      Serial.print(g_pwm_bl);
      Serial.print(" ");
      Serial.print("BR: ");
      Serial.print(g_enc_br.read());
      Serial.print(" ");
      Serial.print(g_pwm_br);
      Serial.println();
      
   }
   Serial.println("Moved Required Counts");
   g_motion.halt();
}
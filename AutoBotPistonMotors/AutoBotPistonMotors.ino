#include "Constants.h"

// KP Value for PWM Control
double constexpr KP_PWM_CONTROL = 0.08 /*5.0 / 20'000.0*/;
// Base PWM at which to SPIN
int16_t constexpr LEG_BASE_PWM = 60;
// Number of Revolutions both diagonals undergo
int32_t constexpr REVOLUTIONS = 3;

#include "Encoders.h"
Encoders g_encoders{
    ENC_FL_U,
    ENC_FL_D,
    ENC_FR_U,
    ENC_FR_D,
    ENC_BL_U,
    ENC_BL_D,
    ENC_BR_U,
    ENC_BR_D,
};

// Encoder g_enc_fr{ENC_FR_U, ENC_FR_D};

// Keep IR Sensor Pins Below 64
//#include "IRSensor.h"
// IRSensor const g_ir_fl{19, LOW};
// IRSensor const g_ir_fr{20, LOW};
// IRSensor const g_ir_bl{18, LOW};
// IRSensor const g_ir_br{21, LOW};

#include "MotionController.h"
MotionController g_motion{
    {DIRECTION_FL, PWM_FL}, {DIRECTION_FR, PWM_FR}, {DIRECTION_BL, PWM_BL}, {DIRECTION_BR, PWM_BR}};

// For Further Details regarding Restart
// Read
// https://www.reddit.com/r/Teensy/comments/7r19uk/reset_and_reboot_teensy_lc_via_code/
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
   g_motion.Halt();

   // Added a delay of 2s
   // To Ensure that the Start is not
   // Instantaneous
   delay(2000);
   // Pressing Button Restarts Microcontroller
   pinMode(RESTART_MICRO_CONTROLLER_PIN, INPUT_PULLDOWN);
   attachInterrupt(digitalPinToInterrupt(RESTART_MICRO_CONTROLLER_PIN), SoftRestart, RISING);

   Serial.println(F("Starting Motion"));
   InitAllLegsPWM(LEG_BASE_PWM);
   g_motion.MaxSpeedAllowed = 150;
   // g_motion.MoveLegs(0,0,0,50);
   // delay(180);
   // g_motion.Halt();
   //MoveByCountsFL(-30, 0'000);
   //MoveByCountsFR(-30, 15'000);
   //MoveByCountsBL(-30, 15'000);
   //MoveByCountsBR(-30, 50'000);

   //TestOmni(20, 150);
   // MoveByCounts(80,980);

    MoveAutoBotFLBRSingleDiagOn(1);
}

void loop()
{
    MoveAutoBotAllDiagOn();
   if (Serial.available())
      SoftRestart();
}

int16_t g_pwm_fl;
int16_t g_pwm_fr;
int16_t g_pwm_bl;
int16_t g_pwm_br;

void SyncAutoBot()
{
   // InitAllLegsPWM(LEG_BASE_PWM);
   //
   // while (true)
   //{
   //   g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
   //
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
   //      g_pwm_br = 0;
   //      Serial.println(F("Halting BR"));
   //   }
   //   if (g_pwm_bl != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR && g_ir_bl.isDetected())
   //   {
   //      Serial.println(F("Halting BL"));
   //      g_pwm_bl = 0;
   //   }
   //
   //   g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
   //   if (g_pwm_bl == 0 && g_pwm_br == 0 && g_pwm_fl == 0 && g_pwm_fr == 0)
   //   {
   //      Serial.println("Sync Motors Brought");
   //      break;
   //   }
   //}
}

void PerformFRBLSync(int16_t const sign)
{
   int32_t const diff = g_encoders.FRBLDiff();

   // Code to Sync Motors
   g_pwm_fr = sign * LEG_BASE_PWM - KP_PWM_CONTROL * diff;
   g_pwm_bl = sign * LEG_BASE_PWM + KP_PWM_CONTROL * diff;

   Serial.print(g_encoders.BL());
   Serial.print(" ");
   Serial.print(g_encoders.FR());
   Serial.print(" ");
   Serial.print(diff);
   Serial.println();

   // By Problem Statement for Syncing Purposes
   // Motors must always move
   // Ahead
   // So Constrain between 0 & 255
   // As -ve Implies Backwards
   if (sign > 0)
   {
      g_pwm_fr = constrain(g_pwm_fr, 0, 255);
      g_pwm_bl = constrain(g_pwm_bl, 0, 255);
   }
   else
   {
      g_pwm_fr = constrain(g_pwm_fr, -255, 0);
      g_pwm_bl = constrain(g_pwm_bl, -255, 0);
   }
}
void PerformFLBRSync(int16_t const sign)
{
   int32_t const diff = g_encoders.FLBRDiff();

   // Serial.print("1 COUNTS ");
   // Serial.print(g_encoders.FL());
   // Serial.print(" ");
   // Serial.print(g_encoders.BR());
   // Serial.print(" ");
   // Serial.print(diff);
   // Serial.println();

   // Code to Sync Motors
   g_pwm_fl = sign * LEG_BASE_PWM - KP_PWM_CONTROL * diff;
   g_pwm_br = sign * LEG_BASE_PWM + KP_PWM_CONTROL * diff;

   // By Problem Statement for Syncing Purposes
   // Motors must always move
   // Ahead
   // So Constrain between 0 & 255
   // As -ve Implies Backwards
   if (sign > 0)
   {
      g_pwm_fl = constrain(g_pwm_fl, 0, 255);
      g_pwm_br = constrain(g_pwm_br, 0, 255);
   }
   else
   {
      g_pwm_fl = constrain(g_pwm_fl, -255, 0);
      g_pwm_br = constrain(g_pwm_br, -255, 0);
   }

   // Serial.print("2 PWM ");
   // Serial.print(g_pwm_fl);
   // Serial.print(" ");
   // Serial.print(g_pwm_br);
   // Serial.println();
}
uint16_t g_counts_single_diag_done = 0;

void PerformAllSync(int16_t const sign)
{
   PerformFLBRSync(-sign);
   PerformFRBLSync(sign);
}
void MoveAutoBotAllDiagOn()
{
   MoveAutoBotAllDiagOn(1);
   MoveAutoBotAllDiagOn(-1);
}
void MoveAutoBotAllDiagOn(int16_t const sign)
{
   // last_sync_fl_br = millis();
   // Initially Speed Up Wheels
   InitAllLegsPWM(LEG_BASE_PWM);
   // Reset Encoder Values
   g_encoders.Reset();
   // Run till Both FL & BR finish 1 Revolution
   while (true)
   {
      PerformAllSync(sign);
      // Keep Moving Forward
      g_motion.MoveLegs(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);

      // If Both of them Complete One Revolution
      // Halt Bot
      // FL_BR Single Sync Done
      if ((abs(g_encoders.FL()) / ENC_PER_REV > 0) || (abs(g_encoders.FR()) / ENC_PER_REV > 0) ||
          (abs(g_encoders.BL()) / ENC_PER_REV > 0) || (abs(g_encoders.BR()) / ENC_PER_REV > 0))
      {
         Serial.println("khlsdasadf sdakldsfakldfskl");
         Serial.print(g_encoders.BR());
         Serial.print(" ");
         Serial.print(g_encoders.FL());
         Serial.println();
         g_motion.Halt();
         // Reset PWM Values
         InitAllLegsPWM(0);

         // Reset Encoder Values
         g_encoders.Reset(0);
         // Terminate LOOP
         break;
      }
   }
   // As One Leg Motion Over
   // Halt Both Legs
   // Go to Next Leg
   // To Restart
   g_motion.Halt();
}

void MoveAutoBotSingleDiagOn()
{
   MoveAutoBotSingleDiagOn(1 /*FORWARD*/);
   MoveAutoBotSingleDiagOn(-1 /*BACKWARD*/);
}
void MoveAutoBotSingleDiagOn(int16_t const sign)
{
   MoveAutoBotFLBRSingleDiagOn(sign);
   MoveAutoBotFRBLSingleDiagOn(sign);
   ++g_counts_single_diag_done;
   CheckHaltingConditionByCountsSingleDiag();
}
void MoveAutoBotFLBRSingleDiagOn(int16_t const sign)
{
   // last_sync_fl_br = millis();
   // Initially Speed Up Wheels
   InitAllLegsPWM(LEG_BASE_PWM);
   // Reset Encoder Values
   g_encoders.FL(0);
   g_encoders.BR(0);
   // Run till Both FL & BR finish 1 Revolution
   while (true)
   {
      PerformFLBRSync(sign);
      // Keep Moving Forward
      g_motion.MoveLegs(g_pwm_fl, 0, 0, g_pwm_br);

      // If Both of them Complete One Revolution
      // Halt Bot
      // FL_BR Single Sync Done
      if ((abs(g_encoders.FL()) / ENC_PER_REV > 0) || (abs(g_encoders.BR()) / ENC_PER_REV > 0))
      {
         Serial.println("khlsdasadf sdakldsfakldfskl");
         Serial.print(g_encoders.BR());
         Serial.print(" ");
         Serial.print(g_encoders.FL());
         Serial.println();
         g_motion.Halt();
         // Reset PWM Values
         g_pwm_fl = 0;
         g_pwm_br = 0;
         // Reset Encoder Values
         g_encoders.FL(0);
         g_encoders.BR(0);
         // Terminate LOOP
         break;
      }
   }
   // As One Leg Motion Over
   // Halt Both Legs
   // Go to Next Leg
   // To Restart
   g_motion.Halt();
}
void MoveAutoBotFRBLSingleDiagOn(int16_t const sign)
{
   // last_sync_fr_bl = millis();
   // Initially Slow down Wheels
   InitAllLegsPWM(LEG_BASE_PWM);
   // Reset Encoder Values
   g_encoders.FR(0);
   g_encoders.BL(0);
   // Run till Both Motors are Not in Sync
   while (true)
   {
      PerformFRBLSync(sign);
      // Till they are in Sync, Keep Moving Forward
      g_motion.MoveLegs(0, g_pwm_fr, g_pwm_bl, 0);

      // If Both of them Complete One Revolution
      // Halt Bot
      // FR_BL Single Sync Done
      if ((abs(g_encoders.FR()) / ENC_PER_REV > 0) || (abs(g_encoders.BL()) / ENC_PER_REV > 0))
      {
         Serial.println("Hllo hdd");
         g_motion.Halt();
         // Reset PWM Values
         g_pwm_fr = 0;
         g_pwm_bl = 0;
         // Reset Encoder Values
         g_encoders.FR(0);
         g_encoders.BL(0);
         // Terminate LOOP
         return;
      }
   }
   // As One Leg Motion Over
   // Halt Both Legs
   // Go to Next Leg
   // To Restart
   g_motion.Halt();
}

// Check if the FL or FR or BL or BR Have Completed Required Revolutions
void CheckHaltingConditionByRevs()
{
   if ((abs(g_encoders.FL()) / ENC_PER_REV) > REVOLUTIONS || (abs(g_encoders.BL()) / ENC_PER_REV) > REVOLUTIONS ||
       (abs(g_encoders.FR()) / ENC_PER_REV) > REVOLUTIONS || (abs(g_encoders.BR()) / ENC_PER_REV) > REVOLUTIONS)
      HaltBotFinal();
}
void CheckHaltingConditionByCountsSingleDiag()
{
   if (g_counts_single_diag_done > REVOLUTIONS)
      HaltBotFinal();
}
void HaltBotFinal()
{
   Serial.print(F("Halting Bot as counts done"));
   SyncAutoBot();
   g_motion.Halt();
   // Reset All Encoders
   g_encoders.Reset();
   // Ensure it Stops
   while (true)
   {
   }
}
void MoveByCountsFL(int16_t const pwm, int32_t const counts)
{
   while (true)
   {
      g_motion.MoveLegs(pwm, 0, 0, 0);
      if (abs(g_encoders.FL()) > counts)
      {
         g_motion.Halt();
         break;
      }
   }
}
void MoveByCountsFR(int16_t const pwm, int32_t const counts)
{
   while (true)
   {
      g_motion.MoveLegs(0, pwm, 0, 0);
      if (abs(g_encoders.FR()) > counts)
      {
         g_motion.Halt();
         break;
      }
   }
}
void MoveByCountsBL(int16_t const pwm, int32_t const counts)
{
   while (true)
   {
      g_motion.MoveLegs(0, 0, pwm, 0);
      if (abs(g_encoders.BL()) > counts)
      {
         g_motion.Halt();
         break;
      }
   }
}
void MoveByCountsBR(int16_t const pwm, int32_t const counts)
{
   while (true)
   {
      g_motion.MoveLegs(0, 0, 0, pwm);
      if (abs(g_encoders.BR()) > counts)
      {
         g_motion.Halt();
         break;
      }
   }
}

void InitAllLegsPWM(int16_t const p_leg_pwm)
{
   g_pwm_fl = p_leg_pwm;
   g_pwm_fr = p_leg_pwm;
   g_pwm_bl = p_leg_pwm;
   g_pwm_br = p_leg_pwm;
}

void TestOmni(int16_t const pwm, uint32_t const time)
{
   // If any Legs goes in Opposite Direction than Required
   // Please Modify moveLegsDirect function in MotionController.h
   // Change Sign of Parameter
   // If Encoder Values are Negative, Invert PINS Please
   {
      Serial.print(F("FL Dir :"));
      Serial.print(DIRECTION_FL);
      Serial.print(F("  PWM :"));
      Serial.print(PWM_FL);
      Serial.println();
      g_motion.MoveLegs(pwm, 0, 0, 0);
      delay(time);
      Serial.print("FL Finished Counts\t:");
      Serial.print(g_encoders.FL());
      Serial.println();
      // Reset FL Encoder
      g_encoders.FL(0);
   }
   {
      Serial.print(F("FR Dir :"));
      Serial.println(DIRECTION_FR);
      Serial.print(F("  PWM :"));
      Serial.print(PWM_FR);
      Serial.println();
      g_motion.MoveLegs(0, pwm, 0, 0);
      delay(time);
      Serial.print("FR Finished Counts\t:");
      Serial.print(g_encoders.FR());
      Serial.println();
      // Reset FR Encoder
      g_encoders.FR(0);
   }
   {
      Serial.print(F("BL Dir :"));
      Serial.println(DIRECTION_BL);
      Serial.print(F("  PWM :"));
      Serial.print(PWM_BL);
      Serial.println();
      g_motion.MoveLegs(0, 0, pwm, 0);
      delay(time);
      Serial.print("BL Finished Counts\t:");
      Serial.print(g_encoders.BL());
      Serial.println();
      // Reset BL Encoder
      g_encoders.BL(0);
   }
   {
      Serial.print(F("BR Dir :"));
      Serial.println(DIRECTION_BR);
      Serial.print(F("  PWM :"));
      Serial.print(PWM_BR);
      Serial.println();
      g_motion.MoveLegs(0, 0, 0, pwm);
      delay(time);
      Serial.print("BR Finished Counts\t:");
      Serial.print(g_encoders.BR());
      Serial.println();
      // Reset BR Encoder
      g_encoders.BR(0);
   }
   g_motion.Halt();
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
   g_motion.MoveLegs(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
   while (true)
   {
      if (abs(g_encoders.FL()) > abs(counts))
         g_pwm_fl = 0;
      if (abs(g_encoders.FR()) > abs(counts))
         g_pwm_fr = 0;
      if (abs(g_encoders.BL()) > abs(counts))
         g_pwm_bl = 0;
      if (abs(g_encoders.BR()) > abs(counts))
         g_pwm_br = 0;

      g_motion.MoveLegs(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
      if (g_pwm_fl == 0 && g_pwm_bl == 0 && g_pwm_fr == 0 && g_pwm_br == 0)
         break;

      Debug();
   }
   Serial.println(F("Moved Required Counts"));
   g_motion.Halt();
}

void Debug()
{
   Serial.print(F("FL: "));
   Serial.print(g_encoders.FL());
   Serial.print(" ");
   Serial.print(g_pwm_fl);
   Serial.print(" ");
   Serial.print("FR: ");
   Serial.print(g_encoders.FR());
   Serial.print(" ");
   Serial.print(g_pwm_fr);
   Serial.print(" ");
   Serial.print("BL: ");
   Serial.print(g_encoders.BL());
   Serial.print(" ");
   Serial.print(g_pwm_bl);
   Serial.print(" ");
   Serial.print("BR: ");
   Serial.print(g_encoders.BR());
   Serial.print(" ");
   Serial.print(g_pwm_br);
   Serial.println();
}
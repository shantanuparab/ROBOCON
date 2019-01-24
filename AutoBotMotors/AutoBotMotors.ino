#include "Constants.h"

#include "IRSensor.h"
#include "MotorController.h"

byte constexpr const LEG_HIGH_PWM = 127;
byte constexpr const LEG_SLOW_PWM = 127;

uint32_t constexpr const THRES_IGNR = 1200;

// Number of Revolutions both diagonals undergo
int32_t constexpr const REVOLUTIONS = 3;

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

Encoder g_enc_fl{30, 31};
Encoder g_enc_fr{29, 28};
Encoder g_enc_bl{26, 27};
Encoder g_enc_br{24, 25};

MotionController const g_motion{
    {DIRECTION_FL, PWM_FL}, {DIRECTION_FR, PWM_FR}, {DIRECTION_BL, PWM_BL}, {DIRECTION_BR, PWM_BR}};

// Keep IR Sensor Pins Below 64
// IRSensor const g_ir_fl{19, LOW};
// IRSensor const g_ir_fl_br{48, LOW};
// IRSensor const g_ir_fr{20, LOW};
// IRSensor const g_ir_fr_bl{46, LOW};
// IRSensor const g_ir_bl{18, LOW};
// IRSensor const g_ir_br{21, LOW};
// IRSensor const g_ir_pin{52, LOW};

// volatile int16_t g_enc_fl = 0;
// volatile int16_t g_enc_fr = 0;
// volatile int16_t g_enc_bl = 0;
// volatile int16_t g_enc_br = 0;

void SetupIRSensors()
{
   // g_ir_fl.AttachInterrupt(InterruptISRFL, FALLING);
   // g_ir_fr.AttachInterrupt(InterruptISRFR, FALLING);
   // g_ir_bl.AttachInterrupt(InterruptISRBL, FALLING);
   // g_ir_br.AttachInterrupt(InterruptISRBR, FALLING);
}
void setup()
{
   Serial.begin(9600);

   // Added a delay of 2s
   // To Ensure that the Start is not
   // Instantaneous
   // delay(2000);
   // g_motion.moveLegsDirect(200, 200, 200, 200);
   delay(100);
   g_motion.halt();
   Serial.println(F("Starting Motion"));
   // SyncAutoBot();
   TestOmni(LEG_SLOW_PWM);
   //InitAllLegsPWM(LEG_HIGH_PWM);
   //MoveAutoBotFLBRSingleDiagOn();
}

int16_t g_pwm_fl;
int16_t g_pwm_fr;
int16_t g_pwm_bl;
int16_t g_pwm_br;

void loop()
{
   // g_motion.moveLegsDirect(0, 0, 0, 25);
   // CheckFRBLSync();
   // MoveAutoBotJatinRecommendationBothDiagsOn();
   // if (g_pwm_fl != 0 && g_enc_fl > REVS)
   //{
   //   Serial.print("Halt FL at ");
   //   Serial.print(FindRPM(micros() - start));
   //   Serial.println();
   //   g_pwm_fl = 0;
   //}
   // if (g_pwm_fr != 0 && g_enc_fr > REVS)
   //{
   //   Serial.print("Halt FR at ");
   //   Serial.print(FindRPM(micros() - start));
   //   Serial.println();
   //   g_pwm_fr = 0;
   //}
   // if (g_pwm_bl != 0 && g_enc_bl > REVS)
   //{
   //   Serial.print("Halt BL at ");
   //   Serial.print(FindRPM(micros() - start));
   //   Serial.println();
   //   g_pwm_bl = 0;
   //}
   // if (g_pwm_br != 0 && g_enc_br > REVS)
   //{
   //   Serial.print("Halt BR at ");
   //   Serial.print(FindRPM(micros() - start));
   //   Serial.println();
   //   g_pwm_br = 0;
   //}
   // put your main code here, to run repeatedly:
   // Serial.println(g_ir_pin.isDetected() ? "D" : "N");
   // MoveAutoBotFRBLSingleDiagOn();
   // g_motion.moveLegsDirect(0,0,0,LEG_SLOW_PWM);
}

// float FindRPM(float time)
//{
//   time = (((((float)time) / REVS) / 1000000));
//   time = 60.0f / time;
//   return time;
//}

int16_t counts = 0;

void SyncAutoBot()
{
   InitAllLegsPWM(LEG_HIGH_PWM);

   // while (true)
   //{
   //   g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);

   //   if (g_pwm_fl != LEG_SLOW_PWM && g_pwm_br != LEG_SLOW_PWM && (millis() - last_sync_fl_br) > 0 &&
   //       g_ir_fl_br.isDetected())
   //   {
   //      Serial.println(F("Halting FL&BR"));
   //      g_pwm_fl = 0 /*LEG_SLOW_PWM*/;
   //      g_pwm_br = 0 /*LEG_SLOW_PWM*/;
   //   }
   //   // if (g_pwm_bl != LEG_SLOW_PWM && g_pwm_fr != LEG_SLOW_PWM && (millis() - last_sync_fr_bl) > 0 &&
   //   //    g_ir_fr_bl.isDetected())
   //   //{
   //   //   Serial.println(F("Slowing BL&FR"));
   //   //   g_pwm_bl = LEG_SLOW_PWM;
   //   //   g_pwm_fr = LEG_SLOW_PWM;
   //   //}
   //   if (g_pwm_fl != LEG_HIGH_PWM && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_fl.isDetected())
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

void MoveAutoBotJDRecommendationFourSyncTogetherOn()
{
   // Run the Synchronisation to Synchronise 4 wheels
   SyncAutoBot();
   // Gets Here when Sync Done
   ++counts;
   CheckHaltingConditionByCounts();
}

void MoveAutoBotJatinRecommendationBothDiagsOn()
{
   // As Both Diags are On
   // Move Entire Bot
   g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);

   CheckFLBRSync();
   CheckFRBLSync();

   CheckHaltingConditionByCounts();
}
bool CheckFRBLSync()
{
   // Sync FR & BL
   {
      auto const fr_counts = g_enc_fr.read();
      auto const bl_counts = g_enc_bl.read();

      auto const diff = fr_counts - bl_counts;

      g_pwm_fr = LEG_SLOW_PWM + 5 * (diff / 20000);
      g_pwm_bl = LEG_SLOW_PWM - 5 * (diff / 20000);

      if ((fr_counts / ENC_PER_REV) > REVOLUTIONS || (bl_counts / ENC_PER_REV) > REVOLUTIONS)
      {
         g_pwm_fr = 0;
         g_pwm_bl = 0;
         g_enc_fr.write(0);
         g_enc_bl.write(0);
         return false;
      }
   }

   // if (g_pwm_fr != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR + 300 && g_ir_fr.isDetected())
   //{
   //   Serial.println(F("Halting FR"));
   //   g_pwm_fr = 0;
   //}
   // if (g_pwm_bl != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR + 300 && g_ir_bl.isDetected())
   //{
   //   Serial.println(F("Halting BL"));
   //   g_pwm_bl = 0;
   //}
   // if (g_pwm_bl != LEG_HIGH_PWM && g_pwm_fr != LEG_HIGH_PWM && (millis() - last_sync_fr_bl) > 500 &&
   //    g_ir_fr_bl.isDetected())
   //{
   //   Serial.println(F("Slowing BL&FR"));
   //   g_pwm_bl = LEG_HIGH_PWM;
   //   g_pwm_fr = LEG_HIGH_PWM;
   //}

   // if (g_pwm_fr == 0 && g_pwm_bl == 0 && (millis() - last_sync_fr_bl) > THRES_IGNR)
   //{
   //   counts++;
   //   Serial.println(F("Restarting Motors FR&BL"));
   //   g_pwm_fr        = LEG_HIGH_PWM;
   //   g_pwm_bl        = LEG_HIGH_PWM;
   //   last_sync_fr_bl = millis();
   //   return false /*Both Motors Off*/;
   //}
   return true; /*1/2 Motors Still On*/
}
bool CheckFLBRSync()
{
   // Sync FL & BR
   {
      auto const fl_counts = g_enc_fl.read();
      auto const br_counts = g_enc_br.read();

      auto const diff = fl_counts - br_counts;

      g_pwm_fl = LEG_SLOW_PWM + 5 * (diff / 20000);
      g_pwm_br = LEG_SLOW_PWM + 5 * (diff / 20000);

      if ((fl_counts / ENC_PER_REV) > REVOLUTIONS || (br_counts / ENC_PER_REV) > REVOLUTIONS)
      {
         g_pwm_fl = 0;
         g_pwm_br = 0;
         g_enc_fl.write(0);
         g_enc_br.write(0);
         return false;
      }
   }

   // last_sync_fl_br = millis();
   // if (g_pwm_fl != 0 && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_fl.isDetected())
   //{
   //   Serial.println(F("Halting FL"));
   //   g_pwm_fl = 0;
   //}
   // if (g_pwm_br != 0 && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_br.isDetected())
   //{
   //   Serial.println(F("Halting BR"));
   //   g_pwm_br = 0;
   //}
   // if (g_pwm_fl != LEG_SLOW_PWM && g_pwm_br != LEG_SLOW_PWM && (millis() - last_sync_fl_br) > 0 &&
   //    g_ir_fl_br.isDetected())
   //{
   //   Serial.println(F("Halting FL&BR"));
   //   g_pwm_fl = LEG_SLOW_PWM;
   //   g_pwm_br = LEG_SLOW_PWM;
   //}
   // else
   //{
   //}

   // if (g_pwm_br == 0 && g_pwm_fl == 0 && (millis() - last_sync_fl_br) > THRES_IGNR)
   //{
   //   counts++;
   //   Serial.println(F("Restarting Motors FL&BR"));
   //   g_pwm_fl        = LEG_HIGH_PWM;
   //   g_pwm_br        = LEG_HIGH_PWM;
   //   last_sync_fl_br = millis();
   //   return false /*Both Motors Off*/;
   //}
   return true; /*1/2 Motors Still On*/
}

void MoveAutoBotChinmayRecommendationSingleDiagsOn()
{
   MoveAutoBotFLBRSingleDiagOn();
   MoveAutoBotFRBLSingleDiagOn();

   CheckHaltingConditionByCounts();
}
void MoveAutoBotFLBRSingleDiagOn()
{
   // last_sync_fl_br = millis();
   // Initially Speed Up Wheels
   InitAllLegsPWM(LEG_HIGH_PWM);
   // Run till Both FL & BR are Not in Sync
   while (CheckFLBRSync())
   {
      // Till they are in Sync
      // Keep Moving Forward
      g_motion.moveLegsDirect(g_pwm_fl, 0, 0, g_pwm_br);
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
   InitAllLegsPWM(LEG_SLOW_PWM);
   // Run till Both Motors are Not in Sync
   while (CheckFRBLSync())
   {
      // Till they are in Sync, Keep Moving Forward
      g_motion.moveLegsDirect(0, g_pwm_fr, g_pwm_bl, 0);
   }
   // As One Leg Motion Over
   // Halt Both Legs
   // Go to Next Leg
   // To Restart
   g_motion.halt();
}

void CheckHaltingConditionByCounts()
{
   if (counts >= REVOLUTIONS)
   {
      Serial.print(F("Halting Counts = "));
      Serial.println(counts);
      SyncAutoBot();
      g_motion.halt();
      // Ensure it Stops
      while (true)
      {
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

void MoveDiagnonalOneByOne(int16_t const pwm)
{
   RotateDiagonal1Only(pwm);
   delay(1650 /*Time taken for FL & BR Diagonal Motion to Complete*/);
   RotateDiagonal2Only(pwm);
   delay(1650 /*TODO: Find Out*/);
}

// Call this Function
// To test the Omni Wheel Motion
void TestOmni(int16_t const pwm)
{
   {
      Serial.print(F("FL Dir :"));
      Serial.print(DIRECTION_FL);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_FL);
      Serial.println();
      g_motion.moveLegsDirect(pwm, 0, 0, 0);
      delay(3000);
      Serial.print("FL Finished Counts\t:");
      Serial.print(g_enc_fl.read());
      Serial.println();
   }
   {
      Serial.print(F("FR Dir :"));
      Serial.println(DIRECTION_FR);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_FR);
      Serial.println();
      g_motion.moveLegsDirect(0, pwm, 0, 0);
      delay(3000);
      Serial.print("FR Finished Counts\t:");
      Serial.print(g_enc_fr.read());
      Serial.println();
   }
   {
      Serial.print(F("BL Dir :"));
      Serial.println(DIRECTION_BL);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_BL);
      Serial.println();
      g_motion.moveLegsDirect(0, 0, pwm, 0);
      delay(3000);
      Serial.print("BL Finished Counts\t:");
      Serial.print(g_enc_bl.read());
      Serial.println();
   }
   {
      Serial.print(F("BR Dir :"));
      Serial.println(DIRECTION_BR);
      Serial.print(F("\tPWM :"));
      Serial.print(PWM_BR);
      Serial.println();
      g_motion.moveLegsDirect(0, 0, 0, pwm);
      delay(3000);
      Serial.print("BR Finished Counts\t:");
      Serial.print(g_enc_br.read());
      Serial.println();
   }
   g_motion.halt();
}

// Diag 1 is for FL & BR
// Diag 2 is for FR & BL
void RotateDiagonals(int16_t diag_fl_br, int16_t diag_fr_bl)
{
   g_motion.moveLegsDirect(diag_fl_br, diag_fr_bl, diag_fr_bl, diag_fl_br);
}

void RotateDiagonal1Only(int16_t const pwm)
{
   Serial.println(F("Moving FL & BR"));
   g_motion.moveLegsDirect(pwm, 0, 0, pwm);
}
void RotateDiagonal2Only(int16_t const pwm)
{
   Serial.println(F("Moving FR & BL"));
   g_motion.moveLegsDirect(0, pwm, pwm, 0);
}

void SpinAll(int16_t const pwm)
{
   Serial.println(F("Moving All Motors at Constant PWM"));
   g_motion.moveLegsDirect(pwm, pwm, pwm, pwm);
}

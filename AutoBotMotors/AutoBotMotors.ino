#include "Constants.h"

#include "IRSensor.h"
#include "MotorController.h"

byte constexpr const LEG_PWM = 200;

uint32_t constexpr const THRES_IGNR = 600;

MotionController g_motion;

// Keep IR Sensor Pins Below 64
IRSensor const g_ir_fl{19, LOW};
IRSensor const g_ir_fr{20, LOW};
IRSensor const g_ir_bl{18, LOW};
IRSensor const g_ir_br{21, LOW};

// volatile int16_t g_enc_fl = 0;
// volatile int16_t g_enc_fr = 0;
// volatile int16_t g_enc_bl = 0;
// volatile int16_t g_enc_br = 0;

void SetupMotionController()
{
   // Set All the Different Motors
   g_motion.FrontLeftMotor({DIRECTION_FL, PWM_FL});
   g_motion.FrontRightMotor({DIRECTION_FR, PWM_FR});
   g_motion.BackLeftMotor({DIRECTION_BL, PWM_BL});
   g_motion.BackRightMotor({DIRECTION_BR, PWM_BR});

   g_motion.StraightDirection = MotionDirection::FORWARD;
   g_motion.MaxPWMAllowed     = 255;
   g_motion.StraightLinePWM(50);

   // Initially Halt
   g_motion.halt();
}

void SetupIRSensors()
{
   // g_ir_fl.AttachInterrupt(InterruptISRFL, FALLING);
   // g_ir_fr.AttachInterrupt(InterruptISRFR, FALLING);
   // g_ir_bl.AttachInterrupt(InterruptISRBL, FALLING);
   // g_ir_br.AttachInterrupt(InterruptISRBR, FALLING);
}
int16_t g_pwm_fl;
int16_t g_pwm_fr;
int16_t g_pwm_bl;
int16_t g_pwm_br;

void setup()
{
   Serial.begin(9600);
   SetupMotionController();

   SetupIRSensors();

   // Added a delay of 2s
   // To Ensure that the Start is not
   // Instantaneous
   //delay(2000);
   // g_motion.moveLegsDirect(200, 200, 200, 200);
    delay(100);
   Serial.println(F("Starting Motion"));
   SyncAutoBot();
   InitAllLegsPWM(LEG_PWM);
}

void loop()
{
   // put your main code here, to run repeatedly:
   // MoveAutoBotJDRecommendationFourSyncTogetherOn();
   MoveAutoBotJatinRecommendationBothDiagsOn();
   // TestOmni(LEG_PWM);
}

int16_t counts = 0;

uint32_t last_sync_fl_br = millis();
uint32_t last_sync_fr_bl = millis();

void SyncAutoBot()
{
   InitAllLegsPWM(LEG_PWM);
   last_sync_fl_br = millis();
   last_sync_fr_bl = millis();

   while (true)
   {
      g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);

      if (g_pwm_fl != 0 && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_fl.isDetected())
      {
         Serial.println(F("Halting FL"));
         g_pwm_fl = 0;
      }
      if (g_pwm_fr != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR && g_ir_fr.isDetected())
      {
         Serial.println(F("Halting FR"));
         g_pwm_fr = 0;
      }
      if (g_pwm_br != 0 && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_br.isDetected())
      {
         Serial.println(F("Halting BR"));
         g_pwm_br = 0;
      }
      if (g_pwm_bl != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR && g_ir_bl.isDetected())
      {
         Serial.println(F("Halting BL"));
         g_pwm_bl = 0;
      }

      g_motion.moveLegsDirect(g_pwm_fl, g_pwm_fr, g_pwm_bl, g_pwm_br);
      if (g_pwm_bl == 0 && g_pwm_br == 0 && g_pwm_fl == 0 && g_pwm_fr == 0)
      {
         Serial.println("Sync Motors Brought");
         break;
      }
   }
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
   if (g_pwm_bl != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR && g_ir_bl.isDetected())
   {
      Serial.println(F("Halting BL"));
      g_pwm_bl = 0;
   }
   if (g_pwm_fr != 0 && (millis() - last_sync_fr_bl) > THRES_IGNR && g_ir_fr.isDetected())
   {
      Serial.println(F("Halting FR"));
      g_pwm_fr = 0;
   }

   if (g_pwm_fr == 0 && g_pwm_bl == 0 && (millis() - last_sync_fr_bl) > THRES_IGNR)
   {
      counts++;
      Serial.println(F("Restarting Motors FR&BL"));
      g_pwm_fr        = LEG_PWM;
      g_pwm_bl        = LEG_PWM;
      last_sync_fr_bl = millis();
      return false /*Both Motors Off*/;
   }
   return true; /*1/2 Motors Still On*/
}
bool CheckFLBRSync()
{
   // last_sync_fl_br = millis();
   if (g_pwm_fl != 0 && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_fl.isDetected())
   {
      Serial.println(F("Halting FL"));
      g_pwm_fl = 0;
   }
   if (g_pwm_br != 0 && (millis() - last_sync_fl_br) > THRES_IGNR && g_ir_br.isDetected())
   {
      Serial.println(F("Halting BR"));
      g_pwm_br = 0;
   }
   else
   {
   }

   if (g_pwm_br == 0 && g_pwm_fl == 0 && (millis() - last_sync_fl_br) > THRES_IGNR)
   {
      counts++;
      Serial.println(F("Restarting Motors FL&BR"));
      g_pwm_fl        = LEG_PWM;
      g_pwm_br        = LEG_PWM;
      last_sync_fl_br = millis();
      return false /*Both Motors Off*/;
   }
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
   last_sync_fl_br = millis();
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
   last_sync_fr_bl = millis();
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
   if (counts >= 20)
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
   Serial.print(F("FL Dir :"));
   Serial.print(DIRECTION_FL);
   Serial.print(F("\tPWM :"));
   Serial.print(PWM_FL);
   Serial.println();
   g_motion.moveLegsDirect(pwm, 0, 0, 0);
   delay(3000);

   Serial.print(F("FR Dir :"));
   Serial.println(DIRECTION_FR);
   Serial.print(F("\tPWM :"));
   Serial.print(PWM_FR);
   Serial.println();
   g_motion.moveLegsDirect(0, pwm, 0, 0);
   delay(3000);

   Serial.print(F("BL Dir :"));
   Serial.println(DIRECTION_BL);
   Serial.print(F("\tPWM :"));
   Serial.print(PWM_BL);
   Serial.println();
   g_motion.moveLegsDirect(0, 0, pwm, 0);
   delay(3000);

   Serial.print(F("BR Dir :"));
   Serial.println(DIRECTION_BR);
   Serial.print(F("\tPWM :"));
   Serial.print(PWM_BR);
   Serial.println();
   g_motion.moveLegsDirect(0, 0, 0, pwm);
   delay(3000);
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

// volatile uint32_t cur = millis();
//
// void InterruptISRFL()
//{
//   if (millis() - cur > 10000)
//   {
//      cur = millis();
//      ++g_enc_fl;
//   }
//}
// void InterruptISRFR()
//{
//   ++g_enc_fr;
//}
// void InterruptISRBL()
//{
//   ++g_enc_bl;
//}
// void InterruptISRBR()
//{
//   ++g_enc_br;
//}
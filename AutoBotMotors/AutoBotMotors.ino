#include "Constants.h"

#include "IRSensor.h"
#include "MotorController.h"

MotionController g_motion;

IRSensor const g_ir_fl{18, LOW};
IRSensor const g_ir_fr{21, LOW};
IRSensor const g_ir_bl{20, LOW};
IRSensor const g_ir_br{19, LOW};

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
void SetupIRSensors()
{
   // attachInterrupt(digitalPinToInterrupt(g_ir_fl.InputPin), InterruptISRFL, CHANGE);
   // g_ir_fr.AttachInterrupt(InterruptISRFR, FALLING);
   // g_ir_bl.AttachInterrupt(InterruptISRBL, FALLING);
   // g_ir_br.AttachInterrupt(InterruptISRBR, FALLING);
}

void setup()
{
   Serial.begin(9600);
   SetupMotionController();

   SetupIRSensors();

   // Added a delay of 2s
   // To Ensure that the Start is not
   // Instantaneous
   delay(2000);
   // g_motion.moveLegsDirect(200, 200, 200, 200);
   // delay(100);
   Serial.println(F("Starting Motion"));
   //SyncAutoBotJatinRecommendations();
}

void loop()
{
   // put your main code here, to run repeatedly:
   // MoveDiagnonalOneByOne(150);

   // TestOmni(50);

    NoveAutoBotJatinRecommendation();

   // SyncAutoBotJatinRecommendations();
}

int16_t pwm_fl = 150;
int16_t pwm_fr = 150;
int16_t pwm_bl = 150;
int16_t pwm_br = 150;

int counts = 0;

uint32_t last_sync_fl_br = millis();
uint32_t last_sync_fr_bl = millis();

void SyncAutoBotJatinRecommendations()
{
   while (true)
   {
      g_motion.moveLegsDirect(pwm_fl, pwm_fr, pwm_bl, pwm_br);

      if (pwm_fl != 0 && (millis() - last_sync_fl_br) > 600 && g_ir_fl.isDetected())
      {
         Serial.println(F("Halting FL"));
         pwm_fl = 0;
      }
      if (pwm_br != 0 && (millis() - last_sync_fl_br) > 600 && g_ir_br.isDetected())
      {
         Serial.println(F("Halting BR"));
         pwm_br = 0;
      }
      if (pwm_fr != 0 && (millis() - last_sync_fr_bl) > 600 && g_ir_fr.isDetected())
      {
         Serial.println(F("Halting FR"));
         pwm_fr = 0;
      }
      if (pwm_bl != 0 && (millis() - last_sync_fr_bl) > 600 && g_ir_bl.isDetected())
      {
         Serial.println(F("Halting BL"));
         pwm_bl = 0;
      }

      g_motion.moveLegsDirect(pwm_fl, pwm_fr, pwm_bl, pwm_br);
      if (pwm_bl == 0 && pwm_br == 0 && pwm_fl == 0 && pwm_fr == 0)
      {
         Serial.println("Halt");
         return;
      }
   }
}

void NoveAutoBotJatinRecommendation()
{
   g_motion.moveLegsDirect(pwm_fl, pwm_fr, pwm_bl, pwm_br);

   if (pwm_fl != 0 && (millis() - last_sync_fl_br) > 600 && g_ir_fl.isDetected())
   {
      Serial.println(F("Halting FL"));
      pwm_fl = 0;
   }
   if (pwm_br != 0 && (millis() - last_sync_fl_br) > 600 && g_ir_br.isDetected())
   {
      Serial.println(F("Halting BR"));
      pwm_br = 0;
   }
   if (pwm_fr != 0 && (millis() - last_sync_fr_bl) > 600 && g_ir_fr.isDetected())
   {
      Serial.println(F("Halting FR"));
      pwm_fr = 0;
   }
   if (pwm_bl != 0 && (millis() - last_sync_fr_bl) > 600 && g_ir_bl.isDetected())
   {
      Serial.println(F("Halting BL"));
      pwm_bl = 0;
   }
   else
   {
   }
   if (pwm_br == 0 && pwm_fl == 0 && (millis() - last_sync_fl_br) > 600)
   {
      counts++;
      Serial.println(F("Restarting Motors FL&BR"));
      pwm_fl          = 150;
      pwm_br          = 150;
      last_sync_fl_br = millis();
   }
   if (pwm_fr == 0 && pwm_bl == 0 && (millis() - last_sync_fr_bl) > 600)
   {
      counts++;
      Serial.println(F("Restarting Motors FR&BL"));
      pwm_fr          = 150;
      pwm_bl          = 150;
      last_sync_fr_bl = millis();
   }
   if (counts >= 40)
   {
      Serial.print(F("Halting Counts = "));
      Serial.println(counts);
      SyncAutoBotJatinRecommendations();
      g_motion.halt();
      while (true)
      {
      }
   }

   // Serial.println(g_enc_fl);

   // bool cur = g_ir_fl.isDetected();
   // if (cur != old)
   //{
   //   ++g_enc_fl;
   //   old = cur;
   //}
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
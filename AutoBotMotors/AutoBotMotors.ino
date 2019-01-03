#include "Constants.h"

#include "MotorController.h"

MotionController g_motion;

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

void setup()
{
   Serial.begin(9600);
   SetupMotionController();

   // Added a delay of 2s
   // To Ensure that the Start is not
   // Instantaneous
   delay(2000);
   Serial.println(F("Starting Motion"));
}

void loop()
{
   // put your main code here, to run repeatedly:
   MoveDiagnonalOneByOne(150);
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

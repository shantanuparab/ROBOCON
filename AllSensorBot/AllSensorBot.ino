#include "Constants.h"
#include "MotorDriver.h"
// Rack and Pinnion ALWAYS Opposite

MotorDriver g_left_pinnion{CONSTANTS::PIN::PINNION::LEFT_DIRECTION1,CONSTANTS::PIN::PINNION::LEFT_DIRECTION2,CONSTANTS::PIN::PINNION::LEFT_PWM_SPEED};
MotorDriver g_right_pinnion{CONSTANTS::PIN::PINNION::RIGHT_DIRECTION1,CONSTANTS::PIN::PINNION::RIGHT_DIRECTION2,CONSTANTS::PIN::PINNION::RIGHT_PWM_SPEED};

MotorDriver g_left_motion{CONSTANTS::PIN::MOTION::LEFT_DIRECTION1,CONSTANTS::PIN::MOTION::LEFT_DIRECTION2,CONSTANTS::PIN::MOTION::LEFT_PWM_SPEED};
MotorDriver g_right_motion{CONSTANTS::PIN::MOTION::RIGHT_DIRECTION1,CONSTANTS::PIN::MOTION::RIGHT_DIRECTION2,CONSTANTS::PIN::MOTION::RIGHT_PWM_SPEED};

void setup()
{
}

void loop() {
  // put your main code here, to run repeatedly:
  g_left_pinnion.setSpeed(90);
  g_right_pinnion.setSpeed(90);
  g_left_motion.setSpeed(90);
  g_right_motion.setSpeed(90);

  delay(3000);

  g_left_pinnion.setSpeed(0);
  g_right_pinnion.setSpeed(0);
  g_left_motion.setSpeed(0);
  g_right_motion.setSpeed(0);
  delay(3000);

  g_left_pinnion.setSpeed(-90);
  g_right_pinnion.setSpeed(-90);
  g_left_motion.setSpeed(-90);
  g_right_motion.setSpeed(-90);

  delay(3000);


  g_left_pinnion.setSpeed(0);
  g_right_pinnion.setSpeed(0);
  g_left_motion.setSpeed(0);
  g_right_motion.setSpeed(0);
  delay(3000);

  g_left_pinnion.setSpeed(-90);
  g_right_pinnion.setSpeed(90);
  g_left_motion.setSpeed(-90);
  g_right_motion.setSpeed(90);

  delay(3000);
  

  g_left_pinnion.setSpeed(0);
  g_right_pinnion.setSpeed(0);
  g_left_motion.setSpeed(0);
  g_right_motion.setSpeed(0);
  delay(3000);

  g_left_pinnion.setSpeed(90);
  g_right_pinnion.setSpeed(-90);
  g_left_motion.setSpeed(90);
  g_right_motion.setSpeed(-90);

  delay(3000);
}

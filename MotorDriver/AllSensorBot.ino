#include "Constants.h"
#include "MotorDriver.h"
// Rack and Pinnion ALWAYS Opposite
}

void loop() {
  // put your main code here, to run repeatedly:
  g_left_pinnion.setSpeed(90);
  g_right_pinnion.setSpeed(90);
  g_left_motion.setSpeed(90);
  g_right_motion.setSpeed(90);

  delay(3000);

  g_left_pinnion.setSpeed(-90);
  g_right_pinnion.setSpeed(-90);
  g_left_motion.setSpeed(-90);
  g_right_motion.setSpeed(-90);

  delay(3000);

  g_left_pinnion.setSpeed(-90);
  g_right_pinnion.setSpeed(90);
  g_left_motion.setSpeed(-90);
  g_right_motion.setSpeed(90);

  delay(3000);
  
  g_left_pinnion.setSpeed(90);
  g_right_pinnion.setSpeed(-90);
  g_left_motion.setSpeed(90);
  g_right_motion.setSpeed(-90);

  delay(3000);
}

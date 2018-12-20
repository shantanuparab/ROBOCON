int mSpeed1, mSpeed2, mSpeed3, mSpeed4;
int maxSpeedAllowed = 150;

void moveMotor(int i, int pwm) {
  digitalWrite(motorDir[i], pwm > 0 ? HIGH : LOW);
  analogWrite(motorPwm[i], abs(pwm));
}

void moveAllMotors(int mSpeed1, int mSpeed2, int mSpeed3, int mSpeed4) {
  moveMotor(0, mSpeed1);
  moveMotor(1, mSpeed2);
  moveMotor(2, mSpeed3);
  moveMotor(3, mSpeed4);
}

void movePlatform(int x, int y, int a) {
  mSpeed1 = - x - a;
  mSpeed2 = + y + a;
  mSpeed3 = + x - a;
  mSpeed4 = - y + a;

  int maxSpd = abs(mSpeed1);
  if (abs(mSpeed2) > abs(maxSpd)) maxSpd = mSpeed2;
  if (abs(mSpeed3) > abs(maxSpd)) maxSpd = mSpeed3;
  if (abs(mSpeed4) > abs(maxSpd)) maxSpd = mSpeed4;
  if (abs(maxSpd) > 255) {
    mSpeed1 = mSpeed1 / maxSpd * maxSpeedAllowed;
    mSpeed2 = mSpeed2 / maxSpd * maxSpeedAllowed;
    mSpeed3 = mSpeed3 / maxSpd * maxSpeedAllowed;
    mSpeed4 = mSpeed4 / maxSpd * maxSpeedAllowed;
  }
  Serial.print(" ");
  Serial.print(maxSpd);
  Serial.print(" ");
  Serial.print(mSpeed1);
  Serial.print(" ");
  Serial.print(mSpeed2);
  Serial.print(" ");
  Serial.print(mSpeed3);
  Serial.print(" ");
  Serial.print(mSpeed4);
  moveAllMotors(mSpeed1, mSpeed2, mSpeed3, mSpeed4);
}

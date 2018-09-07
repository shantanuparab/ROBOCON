#include <Arduino.h>

void setMotor1(float speed) {
  if (speed > 0) {
    digitalWrite(mot1, cw_1);
    analogWrite(pwm1,  calibrated_pwm1(speed));
  }
  else          {
    digitalWrite(mot1, acw_1);
    analogWrite(pwm1, calibrated_pwm1(-speed));
  }
}

void setMotor2(float speed) {
  if (speed > 0) {
    digitalWrite(mot2, cw_2);
    analogWrite(pwm2,  calibrated_pwm1(speed));
  }
  else          {
    digitalWrite(mot2, acw_2);
    analogWrite(pwm2, calibrated_pwm1(-speed));
  }
}

void setMotor3(float speed) {
  if (speed > 0) {
    digitalWrite(mot3, cw_3);
    analogWrite(pwm3,  calibrated_pwm3(speed));
  }
  else          {
    digitalWrite(mot3, acw_3);
    analogWrite(pwm3, calibrated_pwm3(-speed));
  }
}

void setMotor4(float speed) {
  if (speed > 0) {
    digitalWrite(mot4, cw_4);
    analogWrite(pwm4,  calibrated_pwm4(speed));
//    Serial.print(speed);
//    Serial.print(" ");
//    Serial.print(cw_4);
//    Serial.print("\n");
  }
  else          {
    digitalWrite(mot4, acw_4);
    analogWrite(pwm4, calibrated_pwm4(-speed));
//    Serial.print(speed);
//    Serial.print(" ");
//    Serial.print(acw_4);
//    Serial.print("\n");
  }
}

void setEachMotorSpeed(float speed1, float speed2, float speed3, float speed4)
{
  setMotor1(speed1);
  setMotor2(speed2);
  setMotor3(speed3);
  setMotor4(speed4);
//  Serial.println(speed2);
}

void setupMotors()  {
  pinMode(mot1, OUTPUT);
  pinMode(mot2, OUTPUT);
  pinMode(mot3, OUTPUT);
  pinMode(mot4, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(pwm4, OUTPUT);
}

void stop_motors()  {
  analogWrite(pwm1, 0);  //Right
  analogWrite(pwm2, 0); //Left Motor Speed
  analogWrite(pwm3, 0);  //Left Motor Speed
  analogWrite(pwm4, 0);  //Right
}

void mecanumRun(float xSpeed, float ySpeed, float aSpeed) {
  int maxLinearSpeed = 128;

  float speed1 = ySpeed + aSpeed + xSpeed;
  float speed2 = ySpeed - aSpeed - xSpeed;
  float speed3 = ySpeed - aSpeed + xSpeed;
  float speed4 = ySpeed + aSpeed - xSpeed;



  float max = speed1;
  if (max < speed2)   max = speed2;
  if (max < speed3)   max = speed3;
  if (max < speed4)   max = speed4;

  if (max > maxLinearSpeed)
  {
    speed1 = speed1 / max * maxLinearSpeed;
    speed2 = speed2 / max * maxLinearSpeed;
    speed3 = speed3 / max * maxLinearSpeed;
    speed4 = speed4 / max * maxLinearSpeed;
  }

//  Serial.print(speed1);
//  Serial.print("\n");
//  Serial.print(speed2);
//  Serial.print("\n");
//  Serial.print(speed3);
//  Serial.print("\n");
//  Serial.println(speed4);
//  Serial.print("\n");

  setEachMotorSpeed(speed1, speed2, speed3, speed4);
}

void ramp(int x,int y,int r) {
  if (tmpRx < Rx && Rx > 0)
  {
    tmpRx += x;
    flag = true;
  }
  else if (tmpRx > Rx && Rx < 0)
  {
    tmpRx-= x;
    flag = true;
  }
  else
    tmpRx = Rx;
  if (tmpRy < Ry && Ry > 0)
  {
    tmpRy += y;
    flag = true;
  }
  else if (tmpRy > Ry && Ry < 0)
  {
    tmpRy-= y;
    flag = true;
  }
  else
    tmpRy = Ry;


  if (tmpR < Lx && Lx > 0)
  {
    tmpR += r;
    flag = true;
  }
  else if (tmpR > Lx && Lx < 0)
  {
    tmpR -= r;
    flag = true;
  }
  else
    tmpR = Lx;
}


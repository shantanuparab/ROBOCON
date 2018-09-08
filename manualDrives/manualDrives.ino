#include"pins.h"
#include"lsa08.h"
#include"drives.h"
#include"gyro9250_with_filter.h"
int setPoint = 35;
float Kp = 2, Kd = 2.5;
float lastError = 0;
gyro9250 gyro;
float yawAngle;
void setup() {
  Serial.begin(9600);
  Serial.println("Start Setup");
  gyro.init();
  //lineSensorInit();
  for (int i = 0; i < 3; i++) {
    pinMode(motorDir[i], OUTPUT);
    pinMode(motorPwm[i], OUTPUT);
  }
  Serial.println("End Setup");
}

void loop() {
  gyro.readGyro();
  yawAngle = gyro.readYaw();
  //lineValue = readLineSensor();
  Serial.print(" yawAngle: ");
  Serial.print(yawAngle);
  Serial.print(" Line Value: ");
  Serial.print(lineValue);
  if (lineValue == 255) {
    movePlatform(0, 0, 0);
  }
  else {
    float error = lineValue - setPoint;
    Serial.print(" Error: ");
    Serial.print(error);
    float Correction = Kp * error + Kd * (error - lastError);
    Serial.print(" Correction: ");
    Serial.print(Correction);
    movePlatform(0, 255, Correction);
    lastError = error;
  }
  Serial.println();
}

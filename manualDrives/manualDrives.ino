#include"pins.h"
#include"lsa08.h"
#include"drives.h"
#include"gyro9250_with_filter.h"

byte lineValue,prevLineVal;
int setPoint = 35;
float Kp = 1.75, Kd = 3;
float lastError = 0;
//gyro9250 gyro;
float yawAngle;
long pidTime;
byte side = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Start Setup");
  //gyro.init();
  lineSensorInit();
  for (int i = 0; i < 3; i++) {
    pinMode(motorDir[i], OUTPUT);
    pinMode(motorPwm[i], OUTPUT);
  }
  Serial.println("End Setup");
  pidTime = micros();
}

void loop() {
  //gyro.readGyro();
  //yawAngle = gyro.readYaw();
  
  lineValue = readLineSensor();
  //  Serial.print(" yawAngle: ");
  //  Serial.print(yawAngle);
  Serial.print(" jPin : ");
  Serial.print(digitalRead(junctionPulse));
  Serial.print(" side: ");
  Serial.print(side);
  Serial.print(" Line Value: ");
  Serial.print(lineValue);
  if (lineValue == 255) {
    if(prevLineVal == 255) {
    }
    else if(prevLineVal < 35) {
      side = 1;
    }
    else if(prevLineVal >= 35 && prevLineVal < 70)
      side = 2;
    if(side == 1){
      movePlatform(-70, 0, 0);
    }
    else {
      movePlatform(70, 0, 0);
    }
  }
  else {
    if (micros() - pidTime > 1000) {
      pidTime = micros();
      float error = lineValue - setPoint;
      Serial.print(" Error: ");
      Serial.print(error);
      float Correction = Kp * error + Kd * (error - lastError);
      Serial.print(" Correction: ");
      Serial.print(Correction);
      movePlatform(0, 90, -Correction);
      side = 0;
      lastError = error;
    }
  }
  prevLineVal = lineValue;
  Serial.println();
}

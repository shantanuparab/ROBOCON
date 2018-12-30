#include"pins.h"
#include"LSA08.h"
#include"drives.h"
#include<Encoder.h>

long baudRate = 9600;
int flag = 3;

char Address1 = 0x00;
char UartMode1 = 0x02;
byte SerialPin1 = 24;
byte JunctionPin1 = 19;

char Address2 = 0x03;
char UartMode2 = 0x02;
byte SerialPin2 = 28;
byte JunctionPin2 = 26;

byte lineValue;
int setPoint = 35;
float Kp = 1.75, Kd = 26;
long pidTime;
bool junction2Enab = false;
float lastError = 0;
int PositionL = 0;
int PositionR = 0;

bool Align = false;

LSA08 lineSensor1(&Serial3, baudRate, Address1, UartMode1, SerialPin1, JunctionPin1);
LSA08 lineSensor2(&Serial3, baudRate, Address2, UartMode2, SerialPin2, JunctionPin2);
Encoder encodR(2 , A7);
Encoder encodL(3, A4);

void setup() {
  delay(3000);
  Serial.begin(9600);
  Serial.println("Start Setup");
  lineSensor1.initialize();
  lineSensor2.initialize();
  for (int i = 0; i < 3; i++) {
    pinMode(motorDir[i], OUTPUT);
    pinMode(motorPwm[i], OUTPUT);
  }
  Serial.println("End Setup");
  attachInterrupt(digitalPinToInterrupt(JunctionPin1), junctionCheck, RISING);
  pidTime = micros();
  movePlatform(0, 0, 0);
}

void loop() {
  PositionL = encodL.read();
  PositionR = encodR.read();
  Serial.print(PositionL);
  Serial.print("  ");
  Serial.print(PositionR);
  Serial.print("  ");
  Serial.print(((PositionL + PositionR) / 2));
  Serial.println("  ");

  if (!junction2Enab && flag == 1 && abs((PositionL + PositionR) / 2)  >= 220 )
  {
    Serial.println("HUGE");
    junction2Enab = true;
  }
  if (Align)
  {
    Align = false;
    encodR.write(0);
    movePlatform(0, 100, 0);
    while (true) {
      PositionR = abs(encodR.read());
      Serial.print("Encoder align");
      Serial.println(PositionR);
      if (PositionR > 90)
        break;
    }
    movePlatform(0, 0, 0);
  }
  if (flag == 1)
    lineValue = lineSensor1.read();
  else
    lineValue = lineSensor2.read();

  if (lineValue >= 0 && lineValue <= 70 )
  {
    if (micros() - pidTime > 1000) {
      Serial.print(" Line Value: ");
      Serial.print(lineValue);
      pidTime = micros();
      float error = lineValue - setPoint;
      //      Serial.print(" Error: ");
      //      Serial.print(error);
      float Correction = Kp * error + Kd * (error - lastError);
      Serial.print(" flag: ");
      Serial.println(flag);
      if (flag == 1)
        movePlatform(0, 40, Correction);
      else if (flag == 2)
        movePlatform(-40, 0, Correction);
      else if (flag == 3)
        movePlatform(40, 0 , Correction);
      lastError = error;
    }
  }
  else
  {
    movePlatform(0, 0, 0);
  }
}

void junctionCheck()
{
  if (junction2Enab && flag == 1)
  {
    flag = 3;
    Align = true;
    junction2Enab = false;
  }
}

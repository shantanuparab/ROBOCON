#define DEBUG_PLATFORM

#include <PS3BT.h>
#include <usbhub.h>
#include "MotorController.h"
#include"LSA08.h"

long baudRate = 9600;
float Kp = 3, Kd = 36;       //add kd if kp doesnt work
long pidTime;
float lastError = 0;
int setPoint = 35;
byte lineValue;

char Address1 = 0x00;
char UartMode1 = 0x02;
byte SerialPin1 = 24;
byte JunctionPin1 = 2;
int align = 0;


// FRONT LEFT
const Pin DIRECTION_FL = 26;
const Pin PWM_FL = 9;
// FRONT RIGHT
const Pin DIRECTION_FR = 22;
const Pin PWM_FR = 8;
// BACK LEFT
const Pin DIRECTION_BL = 30;
const Pin PWM_BL = 12;
// BACK RIGHT
const Pin DIRECTION_BR = 28;
const Pin PWM_BR = 11;

#define RAMP_FACTOR_X 1
#define RAMP_FACTOR_Y 1

bool flag = false;     //ramping

MotionController motion;
LSA08 lineSensor1(&Serial3, baudRate, Address1, UartMode1, SerialPin1, JunctionPin1);

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>
#include <math.h>

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

int Lx = 0, Ly = 0, Rx = 0, Ry = 0;
int tmpLx = 0, tmpLy = 0;   // ramping

void setupMotionController()
{
  // Set All the Different Motors
  motion.setFrontLeftMotor({ DIRECTION_FL, PWM_FL, LOW });
  motion.setFrontRightMotor({ DIRECTION_FR, PWM_FR, HIGH });
  motion.setBackLeftMotor({ DIRECTION_BL, PWM_BL, LOW });
  motion.setBackRightMotor({ DIRECTION_BR, PWM_BR, HIGH });

  // Initially Will Consider Straight As Forward Direction
  motion.setStraightAsForward();

  // Set the Straight Line PWM
  motion.setStraightLinePWM(90);

  // Set the Maximum PWM Value the Motors will be subjected to
  motion.setMaxPWMAllowed(255);
}

//void TestOmni()
//{
//  motion.moveMotorsDirect(100, 0, 0, 0);
//  delay(3000);
//
//  motion.moveMotorsDirect(0, 100, 0, 0);
//  delay(3000);
//
//  motion.moveMotorsDirect(0, 0, 100, 0);
//  delay(3000);
//
//  motion.moveMotorsDirect(0, 0, 0, 100);
//  delay(3000);
//}

void setup() {
  Serial.begin(115200);
  lineSensor1.initialize();
  setupMotionController();
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  motion.halt();
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  pidTime = micros();

}



void loop() {

  Usb.Task();

  if (PS3.getButtonClick(PS)) {
    Serial.print(F("\r\nPS"));
    PS3.disconnect();
  }

  Ly = map(PS3.getAnalogHat(LeftHatY), 0, 255, -255, 255);
  Lx = map(PS3.getAnalogHat(LeftHatX), 0, 255, -255, 255);
  Ly = -Ly;

  Rx = map(PS3.getAnalogHat(RightHatX), 0, 255, -255, 255);



  if (Rx < 45 && Rx > -45) Rx = 0;
  if (Lx < 45 && Lx > -45) Lx = 0;
  if (Ry < 45 && Ry > -45) Ry = 0;
  if (Ly < 45 && Ly > -45) Ly = 0;
  if (abs(Lx) > abs(Ly)) {
    if ( abs(Ly) < 0.5 * abs(Lx)) {
      Ly = 0;
    }
  }
  else if (abs(Lx) < abs(Ly)) {
    if ( abs(Lx) < 0.5 * abs(Ly)) {
      Lx = 0;
    }
  }

  ramp(RAMP_FACTOR_X, RAMP_FACTOR_Y);

  if (PS3.PS3Connected || PS3.PS3NavigationConnected)
  {
    if (flag) {
      delay(10);
      flag = false;
    }
    motion.moveRawTo(tmpLy, tmpLx, Rx);
    Serial.print(" Lx : ");
    Serial.print(Lx);
    Serial.print(" Ly : ");
    Serial.print(Ly);
    Serial.print(" Rx : ");
    Serial.print(Rx);
    Serial.println();

    if (PS3.getButtonClick(L1))
    {
      align = 1;
    }

    if (align == 1)
    {
      align = 0;
      while (PS3.getButtonClick(L1))
      {
        if (micros() - pidTime > 1000) {
          lineValue = lineSensor1.read();
          pidTime = micros();
          if (lineValue <= 36 && lineValue >= 32)
            break;
          if (lineValue >= 0 && lineValue <= 70)
          {
            lineValue = lineSensor1.read();
            float error = lineValue - setPoint;
            float Correction = Kp * error + Kd * (error - lastError);     //Add this to the code if kp alone doesnt work
            lastError = error;
            if (abs(error) < 8)
            {
              if (error >= 0)
                Correction += 10;
              else
                Correction -= 10;
            }

            //          Serial.print(" Error: ");
            //          Serial.println(error);
            Serial.print(Correction);
            Serial.print("   ");
            Serial.print(lineValue);
            Serial.println();
            motion.moveRawTo(0, Correction, 0);
          }
          else
          {
            break;
          }
        }
      }
    }
  }
}



void ramp(int x, int y) {

  if (tmpLy < Ly && Ly > 0)
  {
    tmpLy += y;
    flag = true;
  }
  else if (tmpLy > Ly && Ly < 0)
  {
    tmpLy -= y;
    flag = true;
  }
  else
    tmpLy = Ly;


  if (tmpLx < Lx && Lx > 0)
  {
    tmpLx += x;
    flag = true;
  }
  else if (tmpLx > Lx && Lx < 0)
  {
    tmpLx -= x;
    flag = true;
  }
  else
    tmpLx = Lx;
}


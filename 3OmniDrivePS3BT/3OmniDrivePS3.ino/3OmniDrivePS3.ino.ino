#define DEBUG_PLATFORM

#include "omnilib.h"
#include <PS3BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <math.h>
#define dir_1 2
#define pwm_1 3
#define dir_2 4
#define pwm_2 5

#define dir_3 7
#define pwm_3 6
USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);
bool printTemperature, printAngle;

int Lx = 0, Ly = 0, Rx = 0, Ry = 0;
Omni_Wheel MyWheels[3];
Platform MyPlatform(3, 2);

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  int vel, ang, turning;
  byte r;
  byte moveEnable;
  pinMode(dir_1, OUTPUT);
  pinMode(dir_2, OUTPUT);
  pinMode(dir_3, OUTPUT);

  pinMode(pwm_1, OUTPUT);
  pinMode(pwm_2, OUTPUT);
  pinMode(pwm_3, OUTPUT);

  MyWheels[0].SetWheel(150, pwm_1, 0, dir_1);
  MyWheels[1].SetWheel(270, pwm_3, 0, dir_3);
  MyWheels[2].SetWheel(30,  pwm_2, 0, dir_2);

  MyPlatform.AddWheel(0, &MyWheels[0]);
  MyPlatform.AddWheel(1, &MyWheels[1]);
  MyPlatform.AddWheel(2, &MyWheels[2]);

  vel = ang = moveEnable = turning = 0;

  moveEnable = 1;
}


void loop() {
  Usb.Task();

  if (PS3.getButtonClick(PS)) {
    Serial.print(F("\r\nPS"));
    PS3.disconnect();
  }

  Ly = PS3.getAnalogHat(LeftHatY) - 123;
  Lx = PS3.getAnalogHat(LeftHatX) - 123;
  Ly = -Ly;

  Ry = PS3.getAnalogHat(RightHatY) - 123;
  Rx = PS3.getAnalogHat(RightHatX) - 123;
  Ry = -Ry;

  //  if (Rx > 123)
  //    Rx = 123;
  //  if (Ry < -123)
  //    Ry = -123;
  //  if (Lx > 123)
  //    Lx = 123;
  //  if (Ly > -123)
  //    Ly = 123;

  if (Rx < 15 && Rx > -15) Rx = 0;
  if (Lx < 15 && Lx > -15) Lx = 0;
  if (Ry < 15 && Ry > -15) Ry = 0;
  if (Ly < 15 && Ly > -15) Ly = 0;

  //  Rx = map(Rx, -123, 123, -125, 125);
  //  Ry = map(Ry, -123, 123, -125, 125);

  if (PS3.PS3Connected || PS3.PS3NavigationConnected)
  {
    //      Serial.print(Rx);
    //      Serial.print(",");
    //      Serial.print(Ry);
    //      Serial.print(",");

    float x = sqrt(Ly * Ly + Lx * Lx);
    float angle = (atan2(Ly, Lx) * 57.2883);
    MyPlatform.Enable();
    x = map(x, 0, 123, 0, 255);
    MyPlatform.Move(x, angle - 90, Rx);
    Serial.print(Lx);
    Serial.print(",");
    Serial.print(",");
    Serial.print("  R :   ");
    Serial.print(x);
    Serial.print(",angle : ");
    Serial.println(angle - 90);
    //      Serial.print(",");
    //      Serial.print(Rx);
    //      Serial.print("  CC   ");
  }
}

//    if (Ly < 137 && Ly > 0)
//    {
//
//    }
//    else if (Ly > -137 && Ly < 0)
//    {
//      float y = sqrt(Ly * Ly + Lx * Lx);
//      float angle = atan2(Ly, Lx);
//      MyPlatform.Enable();
//      MyPlatform.Move(y, angle, Rx);
//    }
//    else if (Rx < 137 && Rx > 0)
//    {
//      //      Serial.print(Lx);
//      //      Serial.print(",");
//      //      Serial.print(Ly);
//      //      Serial.print(",");
//      //      Serial.print(Rx);
//      //      Serial.print(",");
//      //      Serial.print(Ry);
//      //      Serial.print(",");
//      float z = sqrt(Ly * Ly + Lx * Lx);
//      float angle = atan2(Ly, Lx);
//      MyPlatform.Enable();
//      MyPlatform.Move(z, 180, 0);
//    }
//    else if (Rx < -137 && Rx < 0)
//    {
//      float w = sqrt(Ly * Ly + Lx * Lx);
//      float angle = atan2(Ly, Lx);
//      MyPlatform.Enable();
//      MyPlatform.Move(w, -180, 0);
//    }
//    //    else if (PS3.getAnalogHat(RightHatX) < 117)
//    //    {
//    //      float angle = atan2(Ly, Lx);
//    //      MyPlatform.Move(w, -180, 0);
//  }
//  else if (PS3.getAnalogHat(LeftHatY) > 0 || PS3.getAnalogHat(LeftHatX) == 0)
//  {
//    MyPlatform.Enable();
//    MyPlatform.Move(0, 0, 0);
//  }
//}


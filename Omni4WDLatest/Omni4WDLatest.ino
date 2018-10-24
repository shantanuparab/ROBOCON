#define DEBUG_PLATFORM

#include <PS3BT.h>
#include <usbhub.h>
#include "MotorController.h"

// FRONT LEFT
const Pin DIRECTION_FL = 7;
const Pin PWM_FL = 6;
// FRONT RIGHT
const Pin DIRECTION_FR = 4;
const Pin PWM_FR = 5;
// BACK LEFT
const Pin DIRECTION_BL = 2;
const Pin PWM_BL = 3;
// BACK RIGHT
const Pin DIRECTION_BR = 8;
const Pin PWM_BR = 9;

MotionController motion;

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

#include <SPI.h>
#include <math.h>

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

int Lx = 0, Ly = 0, Rx = 0, Ry = 0;

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


void setup() {
	Serial.begin(115200);
	setupMotionController();
#if !defined(__MIPSEL__)
	while (!Serial);
#endif
	if (Usb.Init() == -1) {
		Serial.print(F("\r\nOSC did not start"));
		while (1); 
	}
	Serial.print(F("\r\nPS3 Bluetooth Library Started"));

}


void loop() {
	Usb.Task();

	if (PS3.getButtonClick(PS)) {
		Serial.print(F("\r\nPS"));
		PS3.disconnect();
	}

	Ly = PS3.getAnalogHat(LeftHatY) -  123;
	Lx = PS3.getAnalogHat(LeftHatX) - 123;
	Ly = -Ly;
	//Lx = -Lx;

//	Ry = PS3.getAnalogHat(RightHatY);
	Rx = PS3.getAnalogHat(RightHatX) - 123;
	//Rx = -Rx;	

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
			motion.moveRawTo(Ly, Lx, Rx);
		
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

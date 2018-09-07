#include "omnilib.h"

Omni_Wheel MyWheels[3];
Platform MyPlatform(3, 2);


// MyWheels[0] <- 60 Deg
// MyWheels[1] <- 180 Deg
// MyWheels[2] <- 60 Deg


void setup()
{

  Serial.begin(9600);
  int vel, ang, turning;
  byte r;
  byte moveEnable;

  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);


  MyWheels[0].SetWheel(150, 3, 0, 4);
  MyWheels[1].SetWheel(270, 6, 0, 7);
  MyWheels[2].SetWheel(30,  9, 0, 10);

  MyPlatform.AddWheel(0, &MyWheels[0]);
  MyPlatform.AddWheel(1, &MyWheels[1]);
  MyPlatform.AddWheel(2, &MyWheels[2]);

  vel = ang = moveEnable = turning = 0;

  moveEnable = 1;
}

void loop()
{

  
//  Serial.println("des");
  MyPlatform.Enable();
  MyPlatform.Move(0, 0, 60);
//  MyWheels[0].ServoWrite(100);
//  MyWheels[1].ServoWrite(100);
//  MyWheels[2].ServoWrite(250);
}












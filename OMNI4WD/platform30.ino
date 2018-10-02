#include <omnilib.h>

int vel, ang, turning;
Driver_Wheel MyWheels[3];
Platform MyPlatform(3, 2);
byte r;
byte moveEnable;

void setup() 
{ 
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Initialization");
 
  //
  // Params SetWheel(Degrees, OutputNr for servo PWM, offset, OutputNr for dir1, OutputNr for dir2)  
  //
  MyWheels[0].SetWheel(150, 3, 0, 4, 5);
  MyWheels[1].SetWheel(270, 6, 0, 7, 8);
  MyWheels[2].SetWheel(30,  9, 0, 10, 11);

  MyPlatform.AddWheel(0, &MyWheels[0]);
  MyPlatform.AddWheel(1, &MyWheels[1]);
  MyPlatform.AddWheel(2, &MyWheels[2]);
  MyPlatform.Disable();

  vel = ang = moveEnable = turning = 0;
  
} 

void loop() 
{ 
  
  // wait for orders
  if (Serial1.available())
  {
  // order received. Wich?
  r = Serial1.read();
  switch (r) 
  {
  case 'M' : 
    vel = Serial1.parseInt()*2;        // Magnitude
  case 'A' : 
    ang = Serial1.parseInt();        // Angle 
  case 'E' : 
    moveEnable = Serial1.parseInt(); // Move Enable
  case 'T' : 
    turning = Serial1.parseInt();    // Turning
    if (abs(turning) < 40) turning = 0;  // create threshold
  }
  }
  
   Serial.print(vel);
   Serial.print(", ");
   Serial.print(ang);
   Serial.print(", ");
   Serial.print(turning);
   Serial.print(", ");
   Serial.print(moveEnable);
 
   Serial.println();

  if ((vel == 0) && (turning == 0)) moveEnable = 0;  // if there is no movement on going, then stop

  if (moveEnable)
  {
    MyPlatform.Enable(); 
    MyPlatform.Move(vel*2, ang, turning);  
  }
  else
  {
    MyPlatform.Disable();
  }

} 












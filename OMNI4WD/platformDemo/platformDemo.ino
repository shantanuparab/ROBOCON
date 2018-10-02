#include <omnilib.h>

Driver_Wheel MyWheels[3];
Platform MyPlatform(3, 2);

void setup() 
{ 
 
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
} 

void loop() 
{ 
   
    MyPlatform.Enable(); 
//
//  Params Move(Vel, Direction, Turning)


    MyPlatform.Move(250, 0, 0);    // Move at 100, forward pure, no turning added
    delay (1000);  
    MyPlatform.Move(0, 0, 0);    // stop
    delay (1000);

    MyPlatform.Move(250, 180, 0);    // Move at 100, backwards pure, no turning added
    delay (1000);
    MyPlatform.Move(0, 0, 0);    // stop
    delay (1000);

    MyPlatform.Move(250, 90, 0);    // Move at 100, to the left, no turning added
    delay (1000);
    MyPlatform.Move(0, 0, 0);    // stop
    delay (1000);

    MyPlatform.Move(250, 270, 0);    // Move at 100, to the right, no turning added
    delay (1000);
    MyPlatform.Move(0, 0, 0);    // stop
    delay (1000);

    MyPlatform.Move(0, 0, 200);    // Turn at 50 CW
    delay (500);
    MyPlatform.Move(0, 0, -200);    // Turn at 50 CCW
    delay (500);


    MyPlatform.Move(0, 0, 0);    // stop
    delay (200);
    MyPlatform.Disable();
    delay (1000);
} 












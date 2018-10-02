//======================================================================
// Library PLATFORM
//======================================================================

/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://sourceforge.net/projects/omniplatform/files/
*/

#ifndef OmniLib30_h
#define OmniLib30_h


#include <inttypes.h>
#include <math.h>
#include <Arduino.h>

#define ERR_ILLEGAL_WHEEL_NR -2;

// Uncoment the line below to obtain verbose debug information through Serial
//#define DEBUG_PLATFORM



// This is the basic, Virtual class from which a wheel type must be derivated
// It has 3 basic Data: 
//    Degrees: the offset voltage (command value to keep the wheel on power but without movement
//    ServoPin: the output Nr. (maybe a physical pin, maybe a servo channel) 
//    Degrees: When mounted on a omni-platform, each wheel has a certain inclination from the
//    central line. This value indicates this inclination

// The functions are self explained. Pay special attention to 2 functions:
//    ServoWrite( int Speed)  This function is virtual. This means we MUST create it, specifically
//    for our wheel type(s). Depending on the controll electronics of the wheel, we must program here
//    what shall be done in order to move the wheel. Speed will be a value from -255 to 255, where 0 means still
//    A positive value means move forwards, a negative one backwards.
class B_Wheel
{
private:
  float Degrees;
  int ServoPin;
  int Zero_Offset;

public:
  inline B_Wheel()                               {    SetWheel( 0, 0);  };
  inline B_Wheel(int WDegrees, int SChannel)     {    SetWheel(WDegrees, SChannel);   };
  inline void Enable()                           {  };
  void Disable()                                 {    analogWrite(ServoPin, Zero_Offset);  };
  inline virtual void ServoWrite(int Speed)      {  };
  void Move(float Speed)                         {    ServoWrite(Speed);   };
  inline void  SetOffset(int Ofst)               {    Zero_Offset = Ofst;  }
  inline int ReadOffset()                        {    return(Zero_Offset);  }
  inline void SetDegrees(float deg)              {    Degrees = deg;  }
  float ReadDegrees()                            {    return(Degrees);  };
  inline void SetSChannel(int Sch)               {    ServoPin = Sch;  }
  int ReadSChannel()                             {    return(ServoPin);  };
  void SetWheel(int WDegrees, int MSpeed);
  
};



// This is an implementation of a wheel, controlled by a driver.
// I used here this one: https://www.sparkfun.com/products/9457
// but it can be used with any other driver requesting 2 outputs to
// indicate turning direction
// DATA
// AN1, AN2: Outputs to controll the turning direction.
// SetWheel is being expanded to include this 2 additional parameters

class Driver_Wheel : 
public B_Wheel
{
private:
  int AN1;
  int AN2;

public:
  Driver_Wheel();
  Driver_Wheel(int WDegrees, int SChannel, int iOffser, int A1, int A2);
  void ServoWrite(int Speed);
  //void Move(float Speed);
  void SetWheel(int WDegrees, int MSpeed, int iOffset, int A1, int A2);

};



// Now, the magic part...
// Platform is a class created to control omni-directional platforms
// So far, it controls only 3 wheeled platform, but I will expand it to 4
//
// DATA
// B_Wheel    Array of pointers to B_Wheel variables, each one controlling one wheel
// NoWheels   Number of used wheels
// _enabled   Internal variable to memorize wheter the platform is move enabled or not
// O_Enable   Output Nr to be set to TRUE before moving, or to FALSE if not enabled. Usually, 
//            we will connect this output to the STND_BY input of the motor driver 
//
// FUNCTIONS
// Platform(int NW, int ena);  
//            Constructor, you must indicate the amount of wheels and the O_Enable output
//
// signed char AddWheel(int NrW, B_Wheel *W);
//            This is the way you give in your wheels. You indicate which wheel you want to add to 
//            the platform (remember: first wheel is No. 0), then you indicate a pointer to a B_Wheel variable
//            The Variable must exist (must live) after the function call. I don't copy the content, I just
//            keep the address of your wheels. I know, pointers are tricky... 
//
// void Move(float Vel, float Dir, float Turn); 
//            This function calculates the speed of each wheel based on three commands:
//            Vel: lineal speed of the platform
//            Dir: direction of the movement (0 means pure forwards, 180 pure backwards) 
//                 Any other degree is also possible
//            Turn:Turning speed, to be added to the lineal speed. It is possible to move and turn at once 

class Platform
{
private: 
  B_Wheel *MWheel[4];
  int NoWheels;
  bool _enabled;
  int O_Enable;

public:
  Platform(int NW, int ena);
  signed char AddWheel(int NrW, B_Wheel *W);
  void Disable();
  void Enable();
  void Move(float Vel, float Dir, float Turn); 
  bool Enabled();
};


#endif



//======================================================================
// End of Library PLATFORM
//======================================================================

#include <Wire.h>
#include <I2C_Anything.h>

const byte MY_ADDRESS = 42;

void setup()
{
  Wire.begin (MY_ADDRESS);
  Serial.begin (115200);
  Wire.onReceive (receiveEvent);
}  // end of setup

volatile boolean haveData = false;
volatile float fnum;
volatile long foo;

void loop()
{
  if (haveData)
    {
    Serial.print ("Received fnum = ");
    Serial.println (fnum);
    Serial.print ("Received foo = ");
    Serial.println (foo);
    haveData = false;
    }  // end if haveData

}  // end of loop

// called by interrupt service routine when incoming data arrives
void receiveEvent (int howMany)
 {
 if (howMany >= (sizeof fnum) + (sizeof foo))
   {
   I2C_readAnything (fnum);
   I2C_readAnything (foo);
   haveData = true;
   }  // end if have enough data
 }

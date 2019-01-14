#include <Wire.h>
#include <I2C_Anything.h>

const byte SLAVE_ADDRESS = 42;

void setup()
{
  Wire.begin ();
}  // end of setup

void loop()
{

 long foo = 42;

 for (float fnum = 1; fnum <= 10; fnum += 0.015)
    {
    Wire.beginTransmission (SLAVE_ADDRESS);
    I2C_writeAnything (fnum);
    I2C_writeAnything (foo++);
    Wire.endTransmission ();

    delay (200);
    }  // end of for

}  // end of loop

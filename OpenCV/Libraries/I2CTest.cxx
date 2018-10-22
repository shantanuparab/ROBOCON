// Note that the given code must follow
// https://learn.sparkfun.com/tutorials/raspberry-pi-spi-and-i2c-tutorial/all
// To work Properly

#include "I2C.hxx"
#include <wiringPi.h>

#include <iostream>

// I2C Device Address
const std::uint8_t I2C_ADDRESS = 0x04;

// I2C Device Default Register
const std::uint8_t I2C_DATA_REGISTER = 0x01;

int main()
{
   // Initialise WiringPi
   wiringPiSetup();
   // Create a Pi I2C Device
   const Pi::I2C device{I2C_ADDRESS};

   // Send 255 Values
   for (std::uint8_t i = 0; i < 255; ++i)
   {
      // Write a Byte to the Data Register
      device.write(I2C_DATA_REGISTER, i);
   }
}
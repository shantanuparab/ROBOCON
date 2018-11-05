//  Uno - SPI Slave
//  10 - SS - 53
//  11 - MOSI - 51
//  12 - MISO - 50
//  13 - SCK - 52

// You must always define these Pins
#define SS 10
#define MOSI 11
#define MISO 12
#define SCK 13

#include <RasPiAPISlave.h>

void setup (void)
{
  Serial.begin (115200);   // debugging
  RasPiSlave::SPISetup();
}  // end of setup

int16_t rec;

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (RasPiSlave::SPIRead(rec))
  {
    Serial.print("Value Received\t:");
    Serial.println(rec);
  }
}  // end of loop

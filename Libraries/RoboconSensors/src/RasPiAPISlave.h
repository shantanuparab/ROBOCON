#pragma once

#include <SPI.h>

namespace RasPiSlave
{

namespace
{
volatile byte buf [4]; // buffer array to store received data
volatile byte no_of_bytes;  //store no of bytes of received data
volatile byte pos; //variable to store position of received data
volatile bool process_it; //flag to test if all data has been received
volatile byte sign; //to store the sign of the data
}

void SPISetup()
{
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);   //enable spi as slave
  SPCR |= _BV(SPIE);  //for ISR
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);
  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;
  //now turn on interrupts
  SPI.attachInterrupt();
}

// SPI interrupt service routine
ISR (SPI_STC_vect)
{
  const byte c = SPDR;  // grab byte from SPI Data Register
  //alternative: recVal=SPI.transfer();
  // add to buffer if room
  if (pos < (sizeof (buf)))
    buf [pos] = c;
  //check if pos points to index 0 of array
  //if true then store no_of_bytes in first position of array
  if (pos == 0)
  {
    no_of_bytes = buf[pos];
  }
  //<<<<<<< HEAD
  //check if pos points to index 1 of array
  //if true then store sign in second position of array
  if (pos  ==  1)
  {
    sign = buf[pos];
  }
  //cross-checking to prevent incorrect byte size
  //for more accurate data
  //resets pos if false
  if (no_of_bytes != 2)
    pos  = -1;
  //if pos meets length requirement, process the individual bytes received
  if (pos == no_of_bytes + 1)
    process_it = true;
  //reset pos in case pos accidentally exceeds 3
  if (pos > no_of_bytes + 1)
    pos = -1; //on going out of isr pos becomes 0
  pos++;
  //isr = true;
}  // end of interrupt routine SPI_STC_vect

bool SPIRead(int16_t& rec)
{
  //to receive data you must transfer data since spi is full-duplex
  //i.e sending and receiving happens simultaneously on spi
  rec = SPI.transfer(10);
  if (!process_it)
    return false;

  //reference for how the loop works
  /*byte b1=buf[2];  //Read Upper byte
    byte b2=buf[3];
    rec = ((uint32_t)(b1) << 8)
          + (uint32_t)(b2);)*/
  rec = 0;
  //i variable stores array positon
  //initialized to 2 since received bytes are stored starting from pos = 2
  int i = 2;
  for (int k = no_of_bytes - 1; k >= 0; k--)
  {
    //adding individual bytes by left-shifting the bits by multiples of 8
    rec = rec + ((uint32_t)buf[i] << (8 * k));
    //and increment array position
    i++;
  }
  sign *= -1;
  rec = (int32_t) rec;
  if ( sign == -1)
    rec *= -1;
  //reset array positon
  pos = 0;
  //reset flag
  process_it = false;

  /*Value Read*/
  return true;
}  // end of flag set
}

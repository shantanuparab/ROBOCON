//  Uno - SPI Slave
//  10 - SS - 53
//  11 - MOSI - 51
//  12 - MISO - 50
//  13 - SCK - 52

#define SS 10
#define MOSI 11
#define MISO 12
#define SCK 13

#include <SPI.h>

byte buf [10]; // buffer array to store received data
volatile byte no_of_bytes;  //store no of bytes of received data
volatile byte pos; //variable to store position of received data
volatile bool process_it; //flag to test if all data has been received
volatile byte sign; //to store the sign of the data
//volatile bool isr = false;

void setup (void)
{
  Serial.begin (115200);   // debugging
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
}  // end of setup

// SPI interrupt service routine
ISR (SPI_STC_vect)
{
  const byte c = SPDR;  // grab byte from SPI Data Register
  //alternative: recVal=SPI.transfer();
  // add to buffer if room
  if (pos < (sizeof (buf) - 1))
    buf [pos] = c;
  //check if pos points to start of array
  //if true then store no_of-bytes in first position
  if (pos == 0)
  {
    no_of_bytes = buf[pos];
  }
  //cross-checking to prevent incorrect byte size
  //for more accurate data
  //resets pos if false
  if (no_of_bytes != 2) 
    pos  = -1;
  //if pos meets length requirement, process the individual bytes received
  if (pos == no_of_bytes+1)
    process_it = true;
  //reset pos in case pos accidentally exceeds 3 
  if (pos > no_of_bytes+1)
    pos = -1; //on going out of isr pos becomes 0
  pos++;
  //isr = true;
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  //rec variable store final 2 byte/16 bit data
  uint16_t rec;
  //to receive data you must transfer data since spi is full-duplex
  //i.e sending and receiving happens simultaneously on spi
  rec = SPI.transfer(10);
  //debugging
  /*if (isr == true)
    {
    Serial.println("------------------------");
    Serial.println("isr on");
    Serial.println(process_it);
    Serial.println(no_of_bytes);
    Serial.println(pos);
    Serial.println("------------------------");
    isr = false;
    }*/
  Serial.print("^");//debugging
  Serial.println(pos);//''
  if (process_it)
  {
    /*byte b1=buf[1];  //Read Upper byte
      byte b2=buf[2];
      byte b3=buf[3];
      byte b4=buf[4];
      rec = ((long)(b4) << 24)
            + ((long)(b3) << 16)
            + ((long)(b2) << 8)
            + ((long)(b1));*/
    rec = 0;
    //i variable stores array positon 
    //initialized to 2 since received bytes are stored starting from pos = 2
    int i = 2;
    for (int k = no_of_bytes - 1; k >= 0; k--)
    {
      //adding individual bytes by left-shifting the bits by multiples of 8
      rec = rec + ((uint16_t)buf[i] << (8 * k));
      //and increment array position
      i++;
    }
    //debugging
    Serial.println(no_of_bytes);
    Serial.print("*");
    Serial.println (rec);
    //reset array positon
    pos = 0;
    //reset flag
    process_it = false;
  }  // end of flag set
}  // end of loop

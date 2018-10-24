//Uno -SPI  master
// 11 - MOSI
// 12 - MISO
// 13 - SCK
// 10 - SS

#define SS 10
#define MOSI 11
#define MISO 12
#define SCK 13

#include <SPI.h>

  byte tb[10];  //byte array to store split individual bytes
  byte len;     //variable to store length of sent data
  uint32_t x = 1000;  //test data
  
void setup() {
  // put your setup code here, to run once:
  // Put SCK, MOSI, SS pins into output mode
    pinMode(SCK,OUTPUT);
    pinMode(MOSI,OUTPUT);
    pinMode(SS,OUTPUT);
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  digitalWrite(SCK,LOW);
  digitalWrite(MOSI,LOW);
  digitalWrite(SS, HIGH);  // ensure SS stays high for now
  // Then put SPI hardware into Master mode and turn SPI on
  //SPCR |= (1<<MSTR); //auto done by library (maybe)
  //SPCR |= bit(SPE); //enable SPI
  //attachInterrupt(digitalPinTOInterrupt(7),sendnow,RISING);
  Serial.begin(9600);
  SPI.begin ();
  //speed calculated by processor speed (16MHz) divided by 8
  //depreciated function please replace with SPI.begintransmission
  //less accuracy at speeds above 2 MHz
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

//Splitting data into individual bytes
void bitsplit()
{
  //Serial.println("ENTERED BITSPLIT");
  for(int j=len-1;j>=0;j--)
  {
    //shifting data by multiples of 8 and "anding" with 255 to mask the required byte
    //stored msb first
    tb[len-1-j]= (x>>(8*j))&0xff; 
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  len = sizeof(x);
  bitsplit();
  //Serial.println("Sending");
  digitalWrite(SS, LOW);// Pull SS low to begin transmitting data
  //Sending number of bytes first
  SPI.transfer(len);
  for(int k=0;k<len;k++)
  {
    SPI.transfer(tb[k]);
    //Serial.println(tb[k]);
    //udelay(20);   
  }
  // disable Slave Select
   digitalWrite(SS, HIGH);
  delay (1000);// 1 seconds delay 
  x++;
}


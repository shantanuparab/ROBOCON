#include <Wire.h>

struct SEND_DATA_STRUCTURE {
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t blinks;
  int16_t pause;
};

//define slave i2c address
#define I2C_SLAVE_ADDRESS 9

template<typename T>
void SendData(byte const address, T const& p_data)
{
  auto const size = sizeof(p_data);
  byte* data_byte = (byte*)(&p_data);

  SendData(address, data_byte, size);
}
void SendData(byte const i2c_address, byte* address, byte const size)
{
  Wire.beginTransmission(i2c_address);
  Wire.write(0x06);
  Wire.write(0x85);
  Wire.write(size);
  uint8_t CS = size;
  for(int i = 0; i<size; i++){
    CS^=address[i];
    Wire.write(address[i]);
  }
  Wire.write(CS);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
}
int i = 0;
SEND_DATA_STRUCTURE mydata;
void loop() {
  //this is how you access the variables. [name of the group].[variable name]
  mydata.blinks = 34 + i;
  mydata.pause = 45 + i;

  SendData(I2C_SLAVE_ADDRESS, mydata);
  //send the data
  //Just for fun, we will blink it out too
  delay(500);
  Serial.print(F("Blins are\t:"));
  Serial.println(mydata.blinks);
  Serial.print(F("Pause are\t:"));
  Serial.println(mydata.pause);
  ++i;

}

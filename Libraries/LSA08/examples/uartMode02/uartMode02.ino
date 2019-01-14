#include"LSA08.h"
long lineSensorBaudRate = 9600;
char lineSensorAddress = 0x00;
char lineSensorUartMode = 0x02;
byte SerialPin = 24;
byte JunctionPin = 22;
LSA08 lineSensor(&Serial3,lineSensorBaudRate,lineSensorAddress,lineSensorUartMode,SerialPin,JunctionPin);
void setup() {
  Serial.begin(9600);
  lineSensor.initialize();
  Serial.println("Setup Finished");
}
byte linePosition;
int count;
void loop() {
  Serial.print(count);
  count++;
  linePosition = lineSensor.read();
  Serial.print(" Line Position : ");
  Serial.print(linePosition);
  Serial.println();
}

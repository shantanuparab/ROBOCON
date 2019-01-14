#include"LSA08.h"
long baudRate = 9600;

char Address1 = 0x03;
char UartMode1 = 0x02;
byte SerialPin1 = 28;
byte JunctionPin1 = 26;

char Address2 = 0x00;
char UartMode2 = 0x02;
byte SerialPin2 = 24;
byte JunctionPin2 = 22;

LSA08 lineSensor1(&Serial3,baudRate,Address1,UartMode1,SerialPin1,JunctionPin1);
LSA08 lineSensor2(&Serial3,baudRate,Address2,UartMode2,SerialPin2,JunctionPin2);

void setup() {
  Serial.begin(9600);
  lineSensor1.initialize();
  lineSensor2.initialize();
  Serial.println("Setup Finished");
}
byte linePosition1,linePosition2;
int count;
void loop() {
  Serial.print(count);
  count++;
  linePosition1 = lineSensor1.read();
  Serial.print(" Line Position1 : ");
  Serial.print(linePosition1);
  linePosition2 = lineSensor2.read();
  Serial.print(" Line Position2 : ");
  Serial.print(linePosition2);
  Serial.println();
}

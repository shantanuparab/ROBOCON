const byte rx = 15;
const byte tx = 14;
const byte serialEn = 28;
const byte junctionPulse = 19;
const char address = 0x01;
char command;
byte dummy = 0;
int sensor[8];
unsigned int junctionCount = 0;
int LedCount = 0;

/////////////////////////////////////////////////////////////////////////////

void sendCommand(char command, char data) {
  char checksum = address + command + data;

  Serial3.write(address);
  Serial3.write(command);
  Serial3.write(data);
  Serial3.write(checksum);
}

byte readLineSensor() {
  digitalWrite(serialEn, LOW);  // Set serialEN to LOW to request UART data
  while(Serial3.available() <= 0);
  dummy = Serial3.read();    // Read incoming data and store in dummy
  digitalWrite(serialEn, HIGH);   // Stop requesting for UART data
  return dummy;
//  //Serial.print(" LED Count = ");
//  //Serial.print(LedCount);
}

void setContrast(char data) {
  command = 'S';
  sendCommand(command, data);
}

void setBackLight(char data) {
  command = 'B';
  sendCommand(command , data);
}

void setJunctionWidth(char data) {
  command = 'J';
  sendCommand(command, data);
}

void setThresholdValue(char data) {
  command = 'T';
  sendCommand(command, data);
}

void setLineMode(char data) {
  command = 'L';
  sendCommand(command, data);
}

void setUartMode(char data) {
  command = 'D';
  sendCommand(command, data);
}

void startCalibration() {
  command = 'C';
  char data = 0x00;
  sendCommand(command, data);
}

void clearJunction() {
  command = 'X';
  char data = 0x00;
  sendCommand(command, data);
}

int getJunction() {
  command = 'X';
  char data = 0x01;
  sendCommand(command, data);
  
  while (Serial3.available() <= 0);
  return (int(Serial3.read()));
}

void lineSensorInit() {
  pinMode(serialEn, OUTPUT);  // Setting serialEn as digital output pin
  pinMode(junctionPulse, INPUT);  // Setting junctionPulse as digital input pin
  Serial3.begin(9600);
  Serial3.flush();
  
  // Setting initial condition of serialEn pin to HIGH
  digitalWrite(serialEn, HIGH);
  //sendCommand('R', 0x05);
//  clearJunction();
  delay(100);
} 

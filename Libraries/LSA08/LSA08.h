#ifndef LSA08_Hh
#define LSA08_h

class LSA08 {
  private:
    char address = 0x01;
    byte dummy = 0;
    int lineReadings[8];
    byte serialEn;
    byte junctionPulse;
    long baudRate = 230400;
    byte dummies[5];
    char currentMode = 0x02;
    byte linePosition = 0;
    byte prevDummy = 0;
	HardwareSerial *serialPort;
  public:
    LSA08 (HardwareSerial *port = &Serial3, long baud = 9600, char add=0x01,char curMode = 0x02, byte sPin = -1, byte jPin = -1) {
      serialPort = port;
      serialEn = sPin;
      junctionPulse = jPin;
      address = add;
	  currentMode = curMode;
      baudRate = baud;
    }
    bool readJunction() {
      return digitalRead(junctionPulse);
    }
    void initialize() {

      pinMode(serialEn, OUTPUT);
      pinMode(junctionPulse, INPUT);
      serialPort->begin(baudRate);
      digitalWrite(serialEn, HIGH);
	    delay(10);
	    setUartMode(currentMode);
      clearJunction();
      serialPort->flush();
      delay(100);
    }
    byte ledCount = 0;
    void setContrast(char data) {
      sendCommand('S', data);
    }
    void setBackLight(char data) {
      sendCommand('B', data);
    }
    void setJunctionWidth(char data) {
      sendCommand('J', data);
    }
    void setThreshold(char data) {
      sendCommand('T', data);
    }
    void setLineMode(char data) {
      sendCommand('L', data);
    }
    char getUartMode() {
      return currentMode;
    }
    byte getPosition() {
      return linePosition;
    }
    void setUartMode(char data) {
      currentMode = data;
      sendCommand('D', data);
    }
    void calibrate() {
      sendCommand('C', 0x00);
    }
    void clearJunction() {
      sendCommand('X', 0x00);
    }
    int getJunction() {
      sendCommand('X', 0x01);
      while (serialPort->available() <= 0);
      return (int(serialPort->read()));
    }
    byte read() {
		digitalWrite(serialEn, LOW);  // Set serialEN to LOW to request UART data
		while(serialPort->available() <= 0);
		dummy = serialPort->read();    // Read incoming data and store in dummy
		digitalWrite(serialEn, HIGH);   // Stop requesting for UART data
		return dummy;
    }
    void sendCommand(char command, char data) {
      char checksum = address + command + data;

      serialPort->write(address);
      serialPort->write(command);
      serialPort->write(data);
      serialPort->write(checksum);
    }
};
#endif
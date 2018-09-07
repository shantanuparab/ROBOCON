class LSA08 {
  private:
    char address = 0x01;
    byte dummy = 0;
    int lineReadings[8];
    byte serialEn;
    byte junctionPulse;
    long baudRate = 230400;
    byte dummies[5];
    char currentMode = 0x01;
    byte linePosition = 0;
    byte prevDummy = 0;
  public:
    LSA08 (int port, char add, byte sPin, byte jPin, long baud = 230400) {
      if (port == 0) {
#define serialPort Serial
      }
      else if (port == 1) {
#define serialPort Serial1
      }
      else if (port == 2) {
#define serialPort Serial2
      }
      else if (port == 3) {
#define serialPort Serial3
      }

      serialEn = sPin;
      junctionPulse = jPin;
      address = add;
      baudRate = baud;
    }
    bool readJunction() {
      return digitalRead(junctionPulse);
    }
    void initialize() {

      pinMode(serialEn, OUTPUT);
      pinMode(junctionPulse, INPUT);

      serialPort.begin(baudRate);
      digitalWrite(serialEn, HIGH);

      clearJunction();
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
    void Calibrate() {
      sendCommand('C', 0x00);
    }
    void clearJunction() {
      sendCommand('X', 0x00);
    }
    int getJunction() {
      sendCommand('X', 0x01);
      while (serialPort.available() <= 0);
      return (int(serialPort.read()));
    }
    void readLineSensor(int sensor[8]) {
      for (int i = 0; i < 5; i++) {
        readOnce();
        dummies[i] = dummy;
      }
      ledCount = 0;

      for (int i = 0; i < 8; i++) {
        lineReadings[i] = bitRead(dummies[0], i) + bitRead(dummies[1], i) + bitRead(dummies[2], i) + bitRead(dummies[3], i) + bitRead(dummies[4], i);
        if (lineReadings[i] >= 3) {
          if (currentMode == 0x01) {
            sensor[i] = 1;
            ledCount++;
          }
          else if (currentMode == 0x02) {
            bitWrite(linePosition, i, 1);
          }
        }
        else {
          if (currentMode == 0x01) {
            sensor[i] = 0;
          }
          else if (currentMode == 0x02) {
            bitWrite(linePosition, i, 0);
          }

        }
      }
    }
    void sendCommand(char command, char data) {
      char checksum = address + command + data;

      serialPort.write(address);
      serialPort.write(command);
      serialPort.write(data);
      serialPort.write(checksum);
    }
    void readOnce() {
      digitalWrite(serialEn, LOW);
      while (serialPort.available() <= 0);
      dummy = serialPort.read();
      digitalWrite(serialEn, HIGH);
      if (dummy < 0 || dummy > 70) {
        dummy = prevDummy;
      }
      prevDummy = dummy;
    }
};
class PIDcalc {

  private:
    double error = 0;
    double integral = 0;
    double derivative = 0;
    double errorPrior = 0;
    float Kp;
    float Ki;
    float Kd;
    int mode = 0;                  // 0 - P, 1 - PID, 2 - PI, 3 - PD
    int outM, outm;
    int bias = 0;

  public:

    PIDcalc(int PIDmode, float kp, float ki = 0, float kd = 0) {
      mode = PIDmode;
      Kp = kp;
      Ki = ki;
      Kd = kd;
    }
    long SetPoint = 0;
    long iterationTimer = 10000;
    double out = 0;

    inline void setKp(float a)                      {
      Kp = a;
    }
    inline void setKi(float a)                      {
      Ki = a;
    }
    inline void setKd(float a)                      {
      Kd = a;
    }
    inline void setMode(int a)                      {
      mode = a;
    }
    inline void setOutMax(int a)                    {
      outM = a;
    }
    inline void setOutMin(int a)                    {
      outm = a;
    }
    inline void setSetPoint(long encoderReading)    {
      SetPoint = encoderReading;
    }
    inline void setBias(int a)                      {
      bias = a;
    }

    void resetPID() {
      error = 0;
      integral = 0;
      derivative = 0;
      errorPrior = 0;
    }
    int Calculate(long CurrentValue) {
      switch (mode) {
        case 0: Pcontroller(CurrentValue);
          break;
        case 1: PIDcontroller(CurrentValue);
          break;
        case 2: break;                              //Not Yet Used
        case 3: break;
      }
      return out;
    }
    void PIDcontroller(long CurrentValue) {
      error = SetPoint - CurrentValue;
      integral = integral + (error * iterationTimer / 1000000.0);
      derivative = (error - errorPrior) * 1000000 / double(iterationTimer);
      out = out + error / Kp + Ki * integral + Kd * derivative;
      if (out > outM) out = outM;
      else if (out < outm) out = outm;
    }
    void Pcontroller(long CurrentValue) {
      error = SetPoint - CurrentValue;
      out = bias + error * Kp;
      if (out > outM) out = outM;
      else if (out < outm) out = outm;

    }
};
#define Conn_Mode 49
#define Conn_Pin1 47
#define Conn_Pin2 45
#define Conn_Pin3 43
#define Conn_Pin4 41
char finalColor = '-';
long waitToRecieve = 0;
bool colorReceived = false;

class CommMega {
  public:
    CommMega() {
      pinMode(Conn_Mode, OUTPUT);
    }
    void setMode(int mode) {
      digitalWrite(Conn_Mode, mode);
      if (mode == 0) {
        //ColorSensor
        pinMode(Conn_Pin1, INPUT);
        pinMode(Conn_Pin2, INPUT);
        pinMode(Conn_Pin3, INPUT);
        pinMode(Conn_Pin4, OUTPUT);
      }
      if (mode == 1) {
        //GLA+Gyro
        pinMode(Conn_Pin1, INPUT);
        pinMode(Conn_Pin2, OUTPUT);
        pinMode(Conn_Pin3, OUTPUT);
        pinMode(Conn_Pin4, OUTPUT);
        digitalWrite(Conn_Pin2,HIGH);
        digitalWrite(Conn_Pin3,HIGH);
        digitalWrite(Conn_Pin4,HIGH);
      }
    }
    void requestColor() {
      setMode(0);
      digitalWrite(Conn_Pin4, HIGH);
      if (micros() - waitToRecieve > 25000) {
        if (digitalRead(Conn_Pin1)) {
          finalColor = 'R';
          colorReceived = true;
        }
        else if (digitalRead(Conn_Pin2)) {
          finalColor = 'B';
          colorReceived = true;
        }
        else if (digitalRead(Conn_Pin3)) {
          finalColor = 'Y';
          colorReceived = true;
        }
        else {
          colorReceived = false;
        }
        
      }
    }
    void setGlaAngle(int angle) {
      setMode(1);
      if (angle == 1) {
        digitalWrite(Conn_Pin2, LOW);
        digitalWrite(Conn_Pin3, LOW);
        digitalWrite(Conn_Pin4, LOW);
      }
      else if (angle == 2) {
        digitalWrite(Conn_Pin2, HIGH);
        digitalWrite(Conn_Pin3, LOW);
        digitalWrite(Conn_Pin4, LOW);
      }
      else if (angle == 3) {
        digitalWrite(Conn_Pin2, HIGH);
        digitalWrite(Conn_Pin3, HIGH);
        digitalWrite(Conn_Pin4, HIGH);
      }
      else {
        digitalWrite(Conn_Pin2, HIGH);
        digitalWrite(Conn_Pin3, LOW);
        digitalWrite(Conn_Pin4, HIGH);
      }
      //Serial.println(angle);
    }
    bool getGlaStatus() {
      return digitalRead(Conn_Pin1);
    }
};

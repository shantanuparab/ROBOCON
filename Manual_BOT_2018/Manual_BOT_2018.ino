#include "pins.h"

PS2X ps2x; // create PS2 Controller Class

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

#define debugOff
//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int speedmag = 150;
bool flagRelay = false;
int R = 0;
bool flag = false;
bool soleButPress = false;
int soleIn1 = 49;
int error = 0;
byte type = 0;
byte vibrate = 0;
int reMot1 = 37, reMot2 = 35 , reMot3 = 31, reMot4 = 29;
int Lx = 0, Ly = 0, Rx = 0, Ry = 0;
int setGyroVal = 0;

float rotation = 0;
uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data
float magRefVal = 90.0;
bool rotateOn = false;
long counter = 0;
int pushButton = 36;
int lineSensorValue = 0;

void setup() {
  pinMode(reMot1, OUTPUT);
  pinMode(reMot2, OUTPUT);
  pinMode(reMot3, OUTPUT);
  pinMode(reMot4, OUTPUT);
  digitalWrite(reMot1, LOW);
  digitalWrite(reMot2, LOW);
  digitalWrite(reMot3, LOW);
  digitalWrite(reMot4, LOW);
  pinMode(soleIn1, OUTPUT);
  digitalWrite(soleIn1, LOW);
  Serial.begin(115200);

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  ps2Init();
  //magInit();
}


long x[10];
double avg = 0;
int c = 0;
long start = 0;
int tmpRx = 0, tmpRy = 0, tmpR = 0;

void loop() {

//  rotation = (360 + (int)magCalc() - setGyroVal) % 360;
//  Serial.println(rotation);
//  if (ps2x.ButtonPressed(PSB_L1)) {
//    setGyroVal = magCalc();
//    Serial.println(setGyroVal);
//  }



  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  //  if (ps2x.Button(PSB_START)) {                 //will be TRUE as long as button is pressed
  //    //    uping();
  //    Serial.println("Start is being held");
  //  }
  //  if (ps2x.Button(PSB_SELECT))
  //    Serial.println("Select is being held");
  //
  //
  //  if (ps2x.Button(PSB_PAD_UP)) {        //will be TRUE as long as button is pressed
  //    Serial.print("Up held this hard: ");
  //    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
  //  }
  //  if (ps2x.Button(PSB_PAD_RIGHT)) {
  //    Serial.print("Right held this hard: ");
  //    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  //  }
  //  if (ps2x.Button(PSB_PAD_LEFT)) {
  //    Serial.print("LEFT held this hard: ");
  //    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  //  }
  //  if (ps2x.Button(PSB_PAD_DOWN)) {
  //    Serial.print("DOWN held this hard: ");
  //    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
  //  }


  vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on

  //how hard you press the blue (X) button
  //
  //  if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
  //  {
  //
  //
  //
  //    if (ps2x.Button(PSB_L3))
  //      Serial.println("L3 pressed");
  //    if (ps2x.Button(PSB_R3))
  //      Serial.println("R3 pressed");
  //    if (ps2x.Button(PSB_L2))
  //      Serial.println("L2 pressed");
  //    if (ps2x.Button(PSB_R2))
  //      Serial.println("R2 pressed");
  //    if (ps2x.Button(PSB_GREEN))
  //      Serial.println("Triangle pressed");
  //
  //  }
  //


  if (ps2x.ButtonPressed(PSB_RED))   //will be TRUE if button was JUST pressed
  {
    soleButPress = !soleButPress;

    digitalWrite(soleIn1, soleButPress);
    //    if (soleButPress) {
    //      analogWrite(solePwm, 122);
    //      soleButPress = !soleButPress;
    //    }
    //    else if (!soleButPress) {
    //      analogWrite(solePwm, 0);
    //      soleButPress = !soleButPress;
    //    }

  }
  if(ps2x.ButtonPressed(PSB_GREEN)) {
    Serial.println("Calibrating line sensor");
    Wire.beginTransmission(8);
    Wire.write(10);
    Wire.endTransmission();
  }
  if(ps2x.ButtonPressed(PSB_BLUE)) {
    Wire.requestFrom(8,sizeof(int));
    
    
  }
  if (ps2x.ButtonPressed(PSB_R1)) //will be TRUE if button was JUST released
  {
    rotateOn = !rotateOn;

  }
  if(Wire.available()) {
    lineSensorValue = Wire.read();
    Serial.println(lineSensorValue);
  }
  //  if (ps2x.NewButtonState(PSB_BLUE))           //will be TRUE if button was JUST pressed OR released
  //    Serial.println("X just changed");
  //


  //  Ly = ps2x.Analog(PSS_LY) - 123;/
  Lx = ps2x.Analog(PSS_LX) - 123;
  //  Ly = -Ly; // invert y-axis because/ the OP of the PS2 is inverted wrt maths

  Ry = ps2x.Analog(PSS_RY) - 132;
  Rx = ps2x.Analog(PSS_RX) - 123;
  Ry = -Ry;

  //  Rx += 100;
  //  Ry += 100;

  if (Rx > 123)
    Rx = 123;
  if (Ry < -123)
    Ry = -123;
  if (Lx > 123)
    Lx = 123;
  if (Rx < 10 && Rx > -10) Rx = 0;
  if (Lx < 10 && Lx > -10) Lx = 0;
  if (Ry < 10 && Ry > -10) Ry = 0;
  if (Ly < 10 && Ly > -10) Ly = 0;


  Rx = map(Rx, -123, 123, -125, 125);
  Ry = map(Ry, -123, 123, -125, 125);

  Lx = map(Lx, -123, 123, -125, 125);

    

  ramp(RAMP_FACTOR_X, RAMP_FACTOR_Y, RAMP_FACTOR_R);

//    if (rotation >= 359 || rotation <= 1 ) {
//      mecanumRun(Rx, Ry, Lx);
//    }
//    else if (rotation > 350 && rotation < 359) {
//
//      if(rotateOn)
//        Lx+=10;
//      mecanumRun(Rx, Ry, Lx);
//    }
//    else if (rotation < 10 && rotation > 1) {
//      if(rotateOn)
//        Lx-=10;
//      mecanumRun(Rx, Ry, Lx);
//    }
//    else if (rotation > 180 && rotation < 350) {
//      if(rotateOn)
//        Lx+=50;
//      mecanumRun(Rx, Ry, Lx+50);
//    }
//    else if (rotation < 180 && rotation > 10) {
//      if(rotateOn)
//        Lx-=10;
//      mecanumRun(Rx, Ry, Lx);
//    }




  if (!rotateOn) {
    float angle = atan2(tmpRy, tmpRx);
    if (angle > M_PI / 4.0 && angle < 3 * M_PI / 4.0) {
      mecanumRun(0, tmpRy, tmpR);
    }
    else if ((angle > 3 * M_PI / 4.0 && angle < M_PI) || (angle < -3 * M_PI / 4.0 && angle > -M_PI)) {
      mecanumRun(tmpRx, 0, tmpR);
    }
    else if (angle < -M_PI / 4.0 && angle > -3 * M_PI / 4.0) {
      mecanumRun(0, tmpRy, tmpR);
    }
    else {
      mecanumRun(tmpRx, 0, tmpR);
    }
  }
  if (flag) {
    delay(10);
    flag = false;
  }
#ifdef debugOn
  debug();
#endif
}

void ps2Init() {
  error = ps2x.config_gamepad(52, 51, 53, 50, true, true);

  if (error == 0) {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;

  }
}


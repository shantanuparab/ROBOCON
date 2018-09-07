#include <PS3BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#define dir_1 7
#define pwm_1 5
#define dir_2 4
#define pwm_2 3
//#define dir_3 7
//#define pwm_3 9
USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);


void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  pinMode(pwm_1, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(pwm_2, OUTPUT);
  pinMode(dir_2, OUTPUT);
  //  pinMode(pwm_3, OUTPUT);
  //  pinMode(dir_3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int pwm_value;
  Usb.Task();
  if (PS3.PS3Connected || PS3.PS3NavigationConnected)
  {
    if (PS3.getAnalogHat(LeftHatY) < 117)
    {
      digitalWrite(dir_1, HIGH);//North
      digitalWrite(dir_2, HIGH);
      analogWrite(pwm_1, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm_2, PS3.getAnalogHat(LeftHatY));
    }

    else if (PS3.getAnalogHat(LeftHatY) > 137)
    {
      digitalWrite(dir_1, LOW);
      digitalWrite(dir_2, LOW);
      analogWrite(pwm_1, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm_2, PS3.getAnalogHat(LeftHatY));
    }

    else if (PS3.getAnalogHat(RightHatX) > 137)
    {
      digitalWrite(dir_1, HIGH);
      //      digitalWrite(dir_3, HIGH);
      analogWrite(pwm_1, PS3.getAnalogHat(RightHatX));
      //      analogWrite(pwm_3, PS3.getAnalogHat(RightHatX));
    }
    else if (PS3.getAnalogHat(RightHatX) < 117)
    {
      digitalWrite(dir_2, HIGH);
      //      digitalWrite(dir_3, HIGH);
      analogWrite(pwm_2, PS3.getAnalogHat(RightHatX));
      //      analogWrite(pwm_3, PS3.getAnalogHat(RightHatX));
    }

  }
}


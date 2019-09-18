
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

          //Black Cytron
#define dir1 8
#define pwm1 9
#define dir2 10
#define pwm2 12

          //Raspberry
#define dir3 4
#define pwm3 5
#define dir4 6
#define pwm4 7

USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);


void setup()
{
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
    while (1);
  }
  Serial.println(F("\r\nPS3 Bluetooth Library Started"));

  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(dir4, OUTPUT);
 
  Serial.begin(115200);
}

void loop()
{
  int x;
  int pwm_value;
  Usb.Task();
  if (PS3.PS3Connected || PS3.PS3NavigationConnected)
  {
    if (PS3.getAnalogHat(LeftHatY) < 117)
    {
      Serial.println("North");
     
     // x = PS3.getAnalogHat(LeftHatY);
   //   Serial.println(x);
     
      digitalWrite(dir1, HIGH);
      digitalWrite(dir2, HIGH);
      digitalWrite(dir3, HIGH);
      digitalWrite(dir4, HIGH);
      analogWrite(pwm1, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm2, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm3, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm4, PS3.getAnalogHat(LeftHatY));
    }

    else if (PS3.getAnalogHat(LeftHatY) > 137)
    {
      Serial.println("South");
     
    //  x = PS3.getAnalogHat(LeftHatY);
      //Serial.println(x);
     
      digitalWrite(dir1, LOW);
      digitalWrite(dir2, LOW);
      digitalWrite(dir3, LOW);
      digitalWrite(dir4, LOW);
      analogWrite(pwm1, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm2, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm3, PS3.getAnalogHat(LeftHatY));
      analogWrite(pwm4, PS3.getAnalogHat(LeftHatY));
    }

    else if (PS3.getAnalogHat(RightHatX) > 137)
    {
   
      Serial.println("East");
   
       //x = PS3.getAnalogHat(RightHatX);
       //Serial.println(x);
       
      digitalWrite(dir1, HIGH);
      digitalWrite(dir2, LOW);
      digitalWrite(dir3, HIGH);
      digitalWrite(dir4, LOW);
      analogWrite(pwm1, PS3.getAnalogHat(RightHatX));
      analogWrite(pwm2, PS3.getAnalogHat(RightHatX));
      analogWrite(pwm3, PS3.getAnalogHat(RightHatX));
      analogWrite(pwm4, PS3.getAnalogHat(RightHatX));
    }
   
    else if (PS3.getAnalogHat(RightHatX) < 117)
    {
       Serial.println("West");
     
       //x = PS3.getAnalogHat(RightHatX);
       //Serial.println(x);
       
       digitalWrite(dir1, LOW);
       digitalWrite(dir2, HIGH);
       digitalWrite(dir3, LOW);
       digitalWrite(dir4, HIGH);
       analogWrite(pwm1, PS3.getAnalogHat(RightHatX));
       analogWrite(pwm2, PS3.getAnalogHat(RightHatX));
       analogWrite(pwm3, PS3.getAnalogHat(RightHatX));
       analogWrite(pwm4, PS3.getAnalogHat(RightHatX));
    }
   
   else if (PS3.getButtonClick(UP))
   {
        Serial.println("Up");        
   }
   
   else if (PS3.getButtonClick(DOWN))
   {
        Serial.println("Down");
   }
   
   else if (PS3.getButtonClick(LEFT))
   {
        Serial.println("Left");
   }
   
   else if (PS3.getButtonClick(RIGHT))
   {
        Serial.println("Right");
   }
   
   else if (PS3.getButtonClick(TRIANGLE) == HIGH)
   {
        Serial.println("Triangle");
   }
   
   else if (PS3.getButtonClick(SQUARE) == HIGH)
   {
        Serial.println("Square");
   }
   
   else if (PS3.getButtonClick(CIRCLE) == HIGH)
   {
        Serial.println("Circle");
   }
   
   else if (PS3.getButtonClick(CROSS) == HIGH)
   {
        Serial.println("Cross");
   }

   else if (PS3.getButtonClick(L1) == HIGH)
   {
        Serial.println("L1");
   }

   else if (PS3.getButtonClick(L2) == HIGH)
   {
        Serial.println("L2");
   }
 
   else if (PS3.getButtonClick(L3) == HIGH)
   {
        Serial.println("L3");
   }

   else if (PS3.getButtonClick(R1) == HIGH)
   {
        Serial.println("R1");
   }

   else if (PS3.getButtonClick(R2) == HIGH)
   {
        Serial.println("R2");
   }

   else if (PS3.getButtonClick(R3) == HIGH)
   {
        Serial.println("R3");
   }

   else if (PS3.getButtonClick(SELECT) == HIGH)
   {
        Serial.println("Select");
   }
     
   else if (PS3.getButtonClick(START) == HIGH)
   {
        Serial.println("Start");
   }
  }
}

void moveNorth()
{
   digitalWrite(dir1, HIGH);
   digitalWrite(dir2, HIGH);
   digitalWrite(dir3, HIGH);
   digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveSouth()
{
   digitalWrite(dir1, LOW);
   digitalWrite(dir2, LOW);
   digitalWrite(dir3, LOW);
   digitalWrite(dir4, LOW);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveEast()
{
   digitalWrite(dir1, HIGH);
   digitalWrite(dir2, LOW);
   digitalWrite(dir3, HIGH);
   digitalWrite(dir4, LOW);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveWest()
{
   digitalWrite(dir1, LOW);
   digitalWrite(dir2, HIGH);
   digitalWrite(dir3, LOW);
   digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveNorthWest()
{
   //digitalWrite(dir1, LOW);
   digitalWrite(dir2, HIGH);
   digitalWrite(dir3, HIGH);
   //digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 0);  
}

void moveNorthEast()
{
   digitalWrite(dir1, HIGH);
   //digitalWrite(dir2, HIGH);
   //digitalWrite(dir3, HIGH);
   digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 0);
   analogWrite(pwm3, 0);
   analogWrite(pwm4, 150);  
}

void moveSouthWest()
{
   digitalWrite(dir1, LOW);
   //digitalWrite(dir2, HIGH);
   //digitalWrite(dir3, HIGH);
   digitalWrite(dir4, LOW);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 0);
   analogWrite(pwm3, 0);
   analogWrite(pwm4, 150);  
}

void moveSouthEast()
{
   //digitalWrite(dir1, LOW);
   digitalWrite(dir2, LOW);
   digitalWrite(dir3, LOW);
   //digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 0);  
}

void wait()
{
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 0);
   analogWrite(pwm3, 0);
   analogWrite(pwm4, 0);
}

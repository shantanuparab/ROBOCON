
//servo pwm 3
//getButtonPress(ButtonEnum b) will return a true as long as a button is held down, while getButtonClick(ButtonEnum b) will return true once for each button press.
#include <PS3BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>

Servo myservo; 

 unsigned int pw=100;
             //Black Cytron
#define dir1 4
#define pwm1 5
#define dir2 8
#define pwm2 7

          //Raspberry
#define dir3 26
#define pwm3 11
#define dir4 28
#define pwm4 12
#define direlay 46
int i=0;
const byte outputPin1 = 19;  //Pin Number 14 is Output
const byte outputPin2 = 20;  // Pin Number 12 is Output
boolean j=true;
boolean S=true;



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
  pinMode(direlay, OUTPUT);

  pinMode(outputPin1, OUTPUT);   
  pinMode(outputPin2, OUTPUT);
  myservo.attach(8);
 
  Serial.begin(115200);
}

void loop()
{
  int x;
  int pwm_value;
  Usb.Task();
  if (PS3.PS3Connected || PS3.PS3NavigationConnected)
  {
   

      if(PS3.getButtonPress(UP)&&PS3.getButtonPress(TRIANGLE))
                    {
                     if(pw<=200)
                      {
                        pw=pw+10;
                        delay(500);
                        forward();
                        Serial.println(pw);
                      }
                    }
      else if(PS3.getButtonPress(UP)&&PS3.getButtonPress(CROSS))
                    {
                      if(pw>=70)
                      {
                        pw=pw-10;
                        delay(500);
                        forward();
                        Serial.println(pw);
                      }
                    }
     else if(PS3.getButtonPress(DOWN)&&PS3.getButtonPress(TRIANGLE))
                    {
                      if(pw<=200)
                      {
                        pw=pw+10;
                        delay(500);
                        backward();
                        Serial.println(pw);
                      }
                      
                    }
     else if(PS3.getButtonPress(DOWN)&&PS3.getButtonPress(CROSS))
                    {
                      if(pw>=70)
                      {
                        pw=pw-10;
                        delay(500);
                        backward();
                        Serial.println(pw);
                      }
                    }
     else if(PS3.getButtonPress(UP)&&PS3.getButtonPress(RIGHT))
                  {
                      diagonallyR();
                      Serial.println("diognally is working");
       
                  }
     else if(PS3.getButtonPress(UP)&&PS3.getButtonPress(LEFT))
                  {
                      Serial.println("diognall is working");
                      diagonallyL();
                   }
   else if(PS3.getButtonPress(DOWN)&&PS3.getButtonPress(LEFT))
                  {
                      Serial.println("diognallY is working");
                      diagonallyLD();
         
                  }
      
   else if(PS3.getButtonPress(DOWN)&&PS3.getButtonPress(RIGHT))
                  {
                     Serial.println("diognall is working");
                     diagonallyRD();
         
                  }
   
                    
    else if (PS3.getAnalogHat(LeftHatY) < 117)
                  {
                     Serial.println("North");
     
                      x = PS3.getAnalogHat(LeftHatY);
                      pw=90-map(x,0,255,0,90);
                        Serial.println(pw);
                        forward();
    
                  }

    else if (PS3.getAnalogHat(LeftHatY) > 137)
                  {
                     Serial.println("South");
                   
                     x = PS3.getAnalogHat(LeftHatY);
                     pw=map(x,0,255,0,90);
                      Serial.println(pw);
                      backward();
     
      
                   }

    else if (PS3.getAnalogHat(RightHatX) > 137)
                  {
                 
                     Serial.println("East");
                     x = PS3.getAnalogHat(RightHatX);
                     pw=map(x,0,255,0,90);
                      Serial.println(pw);
                      left();
                  }
   
    else if (PS3.getAnalogHat(RightHatX) < 117)
                  {
                     Serial.println("West");
                    x = PS3.getAnalogHat(RightHatX);
                    pw=90-map(x,0,255,0,90);
                      Serial.println(pw);
                      right();
                  }
   
   else if (PS3.getButtonClick(UP))
                 {
                      Serial.println("Up");        
                 }
   else if (PS3.getButtonPress(UP))
                 {
                      Serial.println("Up");
                      forward();        
                 }
   
  
   
  
    else if (PS3.getButtonPress(DOWN))
                 {
                      Serial.println("Down");
                      backward();
                 }
   
   else if (PS3.getButtonPress(LEFT))
                 {
                      Serial.println("Left");
                      left();
                      
                 }
   
   else if (PS3.getButtonPress(RIGHT))
                 {
                      Serial.println("Right");
                      right();
                 }
 
   else if(PS3.getButtonClick(SQUARE))
        {
   
                     
                Serial.println("square");
                  if(S==true)
                  {
                    relayHIGH();
                    S=false;
                  }


                  
                  else 
                  {
                    relayLOW();
                    S=true;
                  }
        }
    else if (PS3.getButtonPress(L1) == HIGH)
   {
                    Serial.println("SolenoidRelay     L1");
                    solenoidHIGH();
   }
   else if (PS3.getButtonPress(R1) == HIGH)
   {
                    Serial.println("SolenoidRelay     R1");
                    solenoidLOW();
   }
   
   else if (PS3.getButtonClick(CIRCLE) == HIGH)
             {
                  Serial.println("Circle");
                 
                  if(j==true)
                  {
                    servoHigh();
                    j=false;
                  }
                  else 
                  {
                    servoLow();
                    j=true;
                  }
                  
                  
             }
  
  
   else if (PS3.getButtonPress(L2) == HIGH)
   {
        Serial.println("L2");
        rotateR();
   }
 
   else if (PS3.getButtonPress(L3) == HIGH)
   {
        Serial.println("L3");
   }

   
   else if (PS3.getButtonPress(R2) == HIGH)
   {
        Serial.println("R2");
        rotateL();
   }

   else if (PS3.getButtonPress(R3) == HIGH)
   {
        Serial.println("R3");
   }

   else if (PS3.getButtonPress(SELECT) == HIGH)
   {
        Serial.println("Select");
   }
     
   else if (PS3.getButtonPress(START))
   {
        Serial.println("Start");
   }
   else if (PS3.getButtonPress(START))
   {
        Serial.println("Start");
   }
   
   else if (PS3.getButtonPress(UP)==LOW || PS3.getButtonPress(DOWN)==LOW || PS3.getButtonPress(LEFT)==LOW || PS3.getButtonPress(RIGHT)==LOW || PS3.getButtonPress(TRIANGLE)==LOW || PS3.getButtonPress(CROSS)==LOW || PS3.getButtonClick(CIRCLE)==LOW || PS3.getButtonPress(L1) == LOW ||PS3.getButtonPress(R1) == LOW)
                 {
                      stop();        
                 }
   
   
   
  }
}

void stop()
  {
    analogWrite(pwm1,0);
    analogWrite(pwm2,0);
    analogWrite(pwm3,0);
    analogWrite(pwm4,0);
    
    digitalWrite(outputPin1, LOW);
    digitalWrite(outputPin2, LOW);
    
    
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,LOW);
  }
void backward()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,LOW);
  }

  
   void forward()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,HIGH);
  }

  
  void rotateL()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,LOW);
  }

  
  void rotateR()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,HIGH);
  }

  
  void diagonallyL()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,00);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,0);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,HIGH);
  }

  
  void diagonallyR()
  {
    analogWrite(pwm1,00);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,0);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,HIGH);
    
  }


    void diagonallyLD()
  {
    analogWrite(pwm1,0);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,0);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,LOW);
  }

    void diagonallyRD()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,0);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,0);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,HIGH);
  }



    void left()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,LOW);
  }

   void right()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,HIGH);
  }

  /* void relay()
   {
    digitalWrite(direlay,HIGH);
   }*/
   void relayHIGH()
   {
    digitalWrite(direlay,HIGH);
    
       // First output pin is made high
      
   }
   
  void relayLOW()
  {

     digitalWrite(direlay,LOW);  // Second output pin is made high

   }
   void servoHigh()
   {
       myservo.write(85); 
       
   }
   void servoLow()
   {
       myservo.write(0);

   }
   void solenoidHIGH(){
      
      digitalWrite(outputPin1, HIGH); 
   }
   void solenoidLOW(){
    
      digitalWrite(outputPin2, HIGH); 
   }
    
   
   
   








#include <PS2X_lib.h>
#include <MadgwickAHRS.h>

#include <Wire.h>
#include <MPU6050.h>

PS2X ps2x; 
MPU6050 mpu;

// Timers
unsigned long timer = 0;
float timeStep = 0.01;

// Pitch, Roll and Yaw values
float pitch = 0;
float roll = 0;
int yaw = 0;
Madgwick filter;
unsigned long microsPerReading, microsPrevious,microsNow;
float accelScale, gyroScale;

//px2
int error = 0; 
byte type = 0;
byte vibrate = 0;
int pw=100;
  int pwm1=11;
  int pwm2=9;
  int pwm3=5;  
  int pwm4=7;
  int dir1=10;
  int dir2=8;
  int dir3=4;
  int dir4=6;
  



void setup()
{
  

  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);

  pinMode(dir3,OUTPUT);
  pinMode(dir4,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(pwm4,OUTPUT);
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);


 Serial.begin(57600);

  
 error = ps2x.config_gamepad(52,51,53,50,true, true);   //GamePad(clock, command, attention, data, Pressures?, Rumble?) 
 
 if(error == 0)
 {
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
   Serial.println("holding L1 or R1 will print out the analog stick values.");
   Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
      
   type = ps2x.readType(); 
     switch(type)
     {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }


 /*  filter.begin(25);

  // initialize variables to pace updates to correct rate
  microsPerReading = 600000 / 25;
  microsPrevious = micros();

 

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);*/
  


}

void loop() 
{   
  
/*//yaw code
  timer = millis();
   microsNow = micros();
   if (microsNow - microsPrevious >= microsPerReading) {

    // Output raw
    Vector ng = mpu.readNormalizeGyro();


  Vector na= mpu.readNormalizeAccel();

    // update the filter, which computes orientation
    filter.updateIMU(ng.XAxis,ng.YAxis,ng.ZAxis,na.XAxis,na.YAxis,na.ZAxis);

    // print the heading, pitch and roll
    
   yaw = filter.getYaw();
   // Serial.print("yaw ");
    //Serial.println(yaw);

   
    
   
    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;*/

 
//px2 code
    if(error == 1) 
      return; 
  
    if(type == 2)
      { 
   
          ps2x.read_gamepad();//read controller

               if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE-
                  {
                      Serial.print("Wammy Bar Position:");
                      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
                  } 
      }

     else 
        { 

               ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
               if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
                      Serial.println("Start is being held");
               if(ps2x.Button(PSB_SELECT))
                      Serial.println("Select is being held");

               if(ps2x.Button(PSB_PAD_UP)&&ps2x.Button(PSB_PAD_RIGHT))
                  {
                   diagonallyR();
                      Serial.println("diognally is working");
       
                  }
              else if(ps2x.Button(PSB_PAD_UP)&&ps2x.Button(PSB_PAD_LEFT))
                 {
                     Serial.println("diognall is working");
         
                     diagonallyL();
         
                 }
               else if(ps2x.Button(PSS_RX ))
            {
              Serial.println("stick is working");
            }
             
               else if(ps2x.Button(PSB_GREEN)&&ps2x.Button(PSB_PAD_UP))
                    {
                     if(pw<=200)
                      {
                        pw=pw+10;
                        delay(500);
                        forward();
                        Serial.println(pw);
                      }
                    }
               else if(ps2x.Button(PSB_BLUE)&&ps2x.Button(PSB_PAD_DOWN))
                    {
                      if(pw>=70)
                      {
                        pw=pw-10;
                        delay(500);
                        backward();
                        Serial.println(pw);
                      }
                    }
              else if(ps2x.Button(PSB_GREEN)&&ps2x.Button(PSB_PAD_DOWN))
                    {
                      if(pw<=200)
                      {
                        pw=pw+10;
                        delay(500);
                        backward();
                        Serial.println(pw);
                      }
                      
                    }
              else if(ps2x.Button(PSB_BLUE)&&ps2x.Button(PSB_PAD_UP))
                    {
                      if(pw>=70)
                      {
                        pw=pw-10;
                        delay(500);
                        forward();
                        Serial.println(pw);
                      }
                    }
                    
             
 
              
      
               else if(ps2x.Button(PSB_PAD_DOWN)&&ps2x.Button(PSB_PAD_LEFT))
                {
                      Serial.println("diognallY is working");
         
                      diagonallyLD();
         
                }
      
                else if(ps2x.Button(PSB_PAD_DOWN)&&ps2x.Button(PSB_PAD_RIGHT))
               {
                     Serial.println("diognall is working");
         
                     diagonallyRD();
         
                }
                 else if(ps2x.Button(PSB_PAD_UP)) 
                {
                  
     
                    Serial.print("Up held this hard: ");
                    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);

                    forward();
        
                 }

 
                else if(ps2x.Button(PSB_PAD_RIGHT))
                {
                    Serial.print("Right held this hard: ");
                    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
                    right();
         
        
                }
      
                else if(ps2x.Button(PSB_PAD_LEFT))
                {
                    Serial.print("LEFT held this hard: ");
                    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
                    left();
        
                }
      
                else if(ps2x.Button(PSB_PAD_DOWN))
                {
                    Serial.print("DOWN held this hard: ");
                    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
                    backward();
      
                } 
                else   if(ps2x.ButtonReleased(PSB_PAD_UP))             //will be TRUE if button was JUST released
                { 
                      Serial.print("Released");
                      stop();
                }    
                else if(ps2x.ButtonReleased(PSB_PAD_DOWN))             //will be TRUE if button was JUST released
                { 
                      Serial.print("Released");
                      stop();
                }   
               else if(ps2x.ButtonReleased(PSB_PAD_LEFT))             //will be TRUE if button was JUST released
                {     Serial.print("Released");
                      stop();
                }     
               else if(ps2x.ButtonReleased(PSB_PAD_RIGHT))           
                { 
                      Serial.print("Released");
                      stop();
                }   
              else  if(ps2x.ButtonReleased(PSB_PAD_UP)&&ps2x.ButtonReleased(PSB_PAD_LEFT))             //will be TRUE if button was JUST released
                { 
                      Serial.print("Release"); 
                      stop(); 
                }        
              else if(ps2x.ButtonReleased(PSB_PAD_UP)&&ps2x.ButtonReleased(PSB_PAD_RIGHT))            
                  {Serial.print("Release");
                  stop();}  
              else if(ps2x.ButtonReleased(PSB_PAD_DOWN)&&ps2x.ButtonReleased(PSB_PAD_RIGHT))            
                  {Serial.print("Release");
                  stop();} 
              else if(ps2x.ButtonReleased(PSB_PAD_DOWN)&&ps2x.ButtonReleased(PSB_PAD_LEFT))            
                  {Serial.print("Release");
                  stop();} 
              else if(ps2x.Button(PSB_L2))
                {

                    rotateL();
                }
              else if(ps2x.Button(PSB_R2))
                {
             
                   rotateR();
                }
             else if(ps2x.ButtonReleased(PSB_L2))           
                    { Serial.print("Released");
                     stop();}
             else if(ps2x.ButtonReleased(PSB_R2))           
                    { 
                      Serial.print("Released");
                     stop();
                    } 
             else if (ps2x.Button(PSS_LY))
             {
              backward();
              
             }
             else if (ps2x.Button(PSS_LX))
             {
              left();
             }
              else if (ps2x.Button(PSS_RY))
             {
              forward();
             }
              else if (ps2x.Button(PSS_RX))
             {
              right();
             }       

              else if (ps2x.ButtonReleased(PSS_LY))
             {
              stop();
             }
             else if (ps2x.ButtonReleased(PSS_LX))
             {
              stop();
             }
              else if (ps2x.ButtonReleased(PSS_RY))
             {
              stop();
             }
              else if (ps2x.ButtonReleased(PSS_RX))
             {
              stop();
             }       
             /*else if(ps2x.Button(PSB_PINK))
             {
              flapup();
             }
             else if(ps2x.Button(PSB_RED))
             {
              flapdown();
              
             }
             else if((ps2x.ButtonReleased(PSB_PINK))||(ps2x.ButtonReleased(PSB_RED))
             {
              stop1();
             }*/
             
        }

}




         
     


void stop()
  {
    analogWrite(pwm1,0);
    analogWrite(pwm2,0);
    analogWrite(pwm3,0);
    analogWrite(pwm4,0);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,LOW);
  }
void forward()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,HIGH);
  }

  
  void backward()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,LOW);
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
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,LOW);
  }

  
  void left()
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

  
  void diagonallyR()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,00);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,0);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,HIGH);
  }

  
  void diagonallyL()
  {
    analogWrite(pwm1,00);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,0);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,HIGH);
    digitalWrite(dir4,HIGH);
    
  }


    void diagonallyRD()
  {
    analogWrite(pwm1,0);
    analogWrite(pwm2,pw);
    analogWrite(pwm3,0);
    analogWrite(pwm4,pw);
    digitalWrite(dir1,LOW);
    digitalWrite(dir2,LOW);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,LOW);
  }

    void diagonallyLD()
  {
    analogWrite(pwm1,pw);
    analogWrite(pwm2,0);
    analogWrite(pwm3,pw);
    analogWrite(pwm4,0);
    digitalWrite(dir1,HIGH);
    digitalWrite(dir2,HIGH);
    digitalWrite(dir3,LOW);
    digitalWrite(dir4,HIGH);
  }



    void rotateL()
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

   void rotateR()
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

  /*void flapup()
  {
    analogWrite(pwm,30);
    digitalWrite(dir,HIgh);
  }
  void flapdown()
  {
    analogWrite(pwm,30);
    digitalWrite(dir,LOW);
  }
  void stop1()
  {
    analogWrite(pwm,0);
  }*/
  
 

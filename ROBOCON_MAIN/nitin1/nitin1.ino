#include <MPU9250.h>

#include <MadgwickAHRS.h>
#include <Wire.h>
#include <MPU6050.h>

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




void setup() 
{


  
   Serial.begin(9600);

  filter.begin(25);

  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();

 

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
   // delay(500);
  }
  
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();

  // Set threshold sensivty. Default 3.
  // If you don't want use threshold, comment this line or set 0.
  mpu.setThreshold(3);
}

void loop()
{
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
    Serial.print("yaw ");
    Serial.println(yaw);

   // move_left_right();
    
   
    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;

  // Read normalized values
  

 
 
 
}

}
void forward()
  {
    /*digitalWrite(dir1,HIGH);
    digitalWrite(dir2,LOW);
       digitalWrite(dir3,HIGH);
    digitalWrite(dir4,LOW);*/
    
  }               // making all dir pin high to move the bot forward:

  void move_left_right()
  {
    while(yaw<179)
  
     {/*digitalWrite(pwm1,0);
      digitalWrite(pwm2,100);
      digitalWrite(dir3,HIGH);
      digitalWrite(dir4,LOW);
      digitalWrite(dir1,LOW);
      digitalWrite(dir2,LOW);*/

  /* timer = millis();
   microsNow = micros();
   if (microsNow - microsPrevious >= microsPerReading) {

    // Output raw
    Vector ng = mpu.readNormalizeGyro();


  Vector na= mpu.readNormalizeAccel();

    // update the filter, which computes orientation
    filter.updateIMU(ng.XAxis,ng.YAxis,ng.ZAxis,na.XAxis,na.YAxis,na.ZAxis);

    // print the heading, pitch and roll
      
      yaw = filter.getYaw();*/
       Serial.print(yaw);
       Serial.println("L");
       break;
      }
     
     
   while(yaw>=180)
     {/*digitalWrite(pwm2,0);
      digitalWrite(pwm1,100);
      digitalWrite(dir1,HIGH);
      digitalWrite(dir2,LOW);
      digitalWrite(dir3,LOW);
      digitalWrite(dir4,LOW);*/

    /* timer = millis();
   microsNow = micros();
   if (microsNow - microsPrevious >= microsPerReading) {

    // Output raw
    Vector ng = mpu.readNormalizeGyro();


  Vector na= mpu.readNormalizeAccel();

    // update the filter, which computes orientation
    filter.updateIMU(ng.XAxis,ng.YAxis,ng.ZAxis,na.XAxis,na.YAxis,na.ZAxis);

    // print the heading, pitch and roll
      
      yaw = filter.getYaw();*/
       Serial.print(yaw);
    Serial.println("R");
    break;
     }
     
     
     
   while (yaw==179)
     {
      //forward();
      Serial.print(yaw);
    Serial.println("F");
    break;
    
      }
   }
  
     
  //digital pwm 6 5 3 direction digital 4//
      
       
         

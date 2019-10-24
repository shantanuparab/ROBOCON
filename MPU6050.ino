#include<Wire.h>
const int MPU_addr=0x68;// I2C address of the MPU-6050
int16_t accelerationX,accelerationY,accelerationZ,pitch,roll,yaw;
int16_t ax,ay,az,Tmp,GyX,GyY,GyZ;
unsigned long microsNow;
#include <MadgwickAHRS.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;
void setup() {
// put your setup code here, to run once:
Wire.begin();
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);// PWR_MGMT_1 register
 Wire.write(0);// set to zero (wakes up the MPU-6050)
 Wire.endTransmission(true);
 Serial.begin(9600);
 filter.begin(70);

  microsPerReading = 1000000 / 70;
  microsPrevious = micros();
}

void loop() {
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {
  // put your main code here, to run repeatedly:
Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);// starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);// request a total of 14 registers
ax=Wire.read()<<8|Wire.read();// 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)   
ay=Wire.read()<<8|Wire.read();// 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
az=Wire.read()<<8|Wire.read();// 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
Tmp=Wire.read()<<8|Wire.read();// 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
GyX=Wire.read()<<8|Wire.read();// 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
GyY=Wire.read()<<8|Wire.read();// 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
GyZ=Wire.read()<<8|Wire.read();// 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

ax=ax/16384.0;
ay=ay/16384.0;
az=az/16384.0;
GyX=GyX/131.0;
GyY=GyY/131.0;  
GyZ=GyZ/131.0;
filter.updateIMU(GyX,GyY ,GyZ , ax, ay, az);

    // print the  yaw, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();
    Serial.print("yaw: ");
    Serial.print( yaw);
    Serial.print("pitch: ");
    Serial.print(pitch);
    Serial.print("roll: ");
    Serial.println(roll);

    // increment previous time, so we keep proper pace
    microsPrevious = micros();
}
}

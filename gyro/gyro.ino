#include<SparkFunMPU9250-DMP.h>
#include <MadgwickAHRS.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;
MPU9250_DMP imu;

void setup() {
  Serial.begin(9600);
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial
      .println();
      delay(5000);
    }
  }
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setGyroFSR(250);
  imu.setAccelFSR(2); // Set accel to +/-2g
  imu.setLPF(5); // Set LPF corner frequency to 5Hz
  imu.setSampleRate(25); // Set sample rate to 10Hz
  imu.setCompassSampleRate(25); // Set mag rate to 10Hz
  filter.begin(25);
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
}
int16_t old_angle = 1;
uint32_t old_time;
void loop() {
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {
    if ( imu.dataReady() )
    {
      // Call update() to update the imu objects sensor data.
      // You can specify which sensors to update by combining
      // UPDATE_ACCEL, UPDATE_GYRO, UPDATE_COMPASS, and/or
      // UPDATE_TEMPERATURE.
      // (The update function defaults to accel, gyro, compass,
      //  so you don't have to specify these values.)
      imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
      float accelX = imu.calcAccel(imu.ax);
      float accelY = imu.calcAccel(imu.ay);
      float accelZ = imu.calcAccel(imu.az);
      float gyroX = imu.calcGyro(imu.gx);
      float gyroY = imu.calcGyro(imu.gy);
      float gyroZ = imu.calcGyro(imu.gz);
      float magX = imu.calcMag(imu.mx);
      float magY = imu.calcMag(imu.my);
      float magZ = imu.calcMag(imu.mz);
      //filter.update(gyroX, gyroY, gyroZ, accelX, accelY, accelZ, magX, magY, magZ);
      filter.updateIMU(gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
      //      Serial.print("Roll : ");
      //      Serial.print(filter.getRoll());
      //      Serial.print("\t Pitch : ");
      //      Serial.print(filter.getPitch());
      Serial.print("\t Yaw : ");

      Serial.print(int(filter.getYaw()));
      Serial.println();
//      if (((int16_t)(filter.getYaw())) != old_angle and ((int16_t)(filter.getYaw())) != old_angle + 1 and ((int16_t)(filter.getYaw())) != old_angle - 1 )
//      {
//        old_angle = (int16_t)(filter.getYaw());
//        Serial.print("Time:\t");
//        Serial.print(( micros() - old_time) / 1000000);
//        old_time = micros();
//
//
//
//      }
      //  Serial.print("\t Yaw : ");

      //Serial.print(int(filter.getYaw()));
      //Serial.println();
    }
    microsPrevious = microsPrevious + microsPerReading;
  }

}

#include<SparkFunMPU9250-DMP.h>
#include <MadgwickAHRS.h>

class gyro9250 {
	private:
		MPU9250_DMP imu;
		unsigned long microsPerReading, microsPrevious;
		Madgwick filter;
		int16_t old_angle = 1;
		uint32_t old_time;
		float yaw;
	public:
		
		void init() {
			if (imu.begin() != INV_SUCCESS)
			{
        Serial.println("1");
				while (1)
				{
				Serial.println("Unable to communicate with MPU-9250");
				Serial.println("Check connections, and try again.");
				Serial.println();
				delay(5000);
				}
			}
      Serial.println("2");
			imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
			imu.setGyroFSR(250);
			imu.setAccelFSR(2); // Set accel to +/-2g
			imu.setLPF(5); // Set LPF corner frequency to 5Hz
			imu.setSampleRate(25); // Set sample rate to 10Hz
			imu.setCompassSampleRate(25); // Set mag rate to 10Hz
      Serial.println("3");
			filter.begin(25);
			microsPerReading = 1000000 / 25;
			microsPrevious = micros();
		}
		void readGyro() {
			unsigned long microsNow;
      if(millis() > 2000) {
        filter.changeBeta(0.2);
      }
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
					imu.update(UPDATE_ACCEL | UPDATE_GYRO);
					float accelX = imu.calcAccel(imu.ax);
					float accelY = imu.calcAccel(imu.ay);
					float accelZ = imu.calcAccel(imu.az);
					float gyroX = imu.calcGyro(imu.gx);
					float gyroY = imu.calcGyro(imu.gy);
					float gyroZ = imu.calcGyro(imu.gz);
//					float magX = imu.calcMag(imu.mx);
//					float magY = imu.calcMag(imu.my);
//					float magZ = imu.calcMag(imu.mz);
					//filter.update(gyroX, gyroY, gyroZ, accelX, accelY, accelZ, magX, magY, magZ);
					filter.updateIMU(gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
					//      Serial.print("Roll : ");
					//      Serial.print(filter.getRoll());
					//      Serial.print("\t Pitch : ");
					//      Serial.print(filter.getPitch());
					yaw = filter.getYaw();
				}
				microsPrevious = microsPrevious + microsPerReading;
			}
		}
		float readYaw() {
			return yaw;
		}
};

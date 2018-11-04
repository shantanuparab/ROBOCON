#include "SensorMPU9250.h"

// Connect to Sensor
// Returns True When Connection Successful

bool SensorMPU9250::begin()
{
   if (m_sensor.begin() != INV_SUCCESS)
      return false;
   // Set the Sensors Started
   if (m_sensor.setSensors(m_default_sensors) != INV_SUCCESS)
      return false;

   // Set the Full Scale Range of Gyro And Accelerometer
   if (m_sensor.setGyroFSR(m_gyro_fsr) != INV_SUCCESS ||
       m_sensor.setAccelFSR(m_accel_fsr) != INV_SUCCESS)
      return false;

   // setLPF() can be used to set the digital low-pass filter
   // of the accelerometer and gyroscope.
   // Can be any of the following: 188, 98, 42, 20, 10, 5
   // (values are in Hz).
   if (m_sensor.setLPF(m_lpf_rate_hz) != INV_SUCCESS)
      return false;

   // Set the Sample Rate of the Accelerometer, Magnetometer, GyroScope and Compass
   if (m_sensor.setSampleRate(m_sample_rate_hz) != INV_SUCCESS ||
       m_sensor.setCompassSampleRate(m_sample_rate_hz) != INV_SUCCESS)
      return false;
   // Set Sample Rate of Filter
   m_filter.begin(m_sample_rate_hz);

   // Set the Previous Time from which to start counting to now
   m_micros_prev_reading_time = micros();

   return true;
}

// Do Not Call Forse Update Unless Required
// Use the More Safer Functions such as GetAngle, GetYaw, GetRoll, GetPitch
// Calling ForceUpdate on your own might prove dangerous

bool SensorMPU9250::ForceUpdate(const uint8_t p_sensors_update)
{
   // Check if Sensor is Ready to Send Data
   if (!m_sensor.dataReady())
      return false;

   // Update Values
   if (m_sensor.update(p_sensors_update) != INV_SUCCESS)
      return false;

   // Get Updated Readings from Sensor
   // Pass them to Madgwick
   {
      const float accelX = m_sensor.calcAccel(m_sensor.ax);
      const float accelY = m_sensor.calcAccel(m_sensor.ay);
      const float accelZ = m_sensor.calcAccel(m_sensor.az);
      const float gyroX  = m_sensor.calcGyro(m_sensor.gx);
      const float gyroY  = m_sensor.calcGyro(m_sensor.gy);
      const float gyroZ  = m_sensor.calcGyro(m_sensor.gz);
      const float magX   = m_sensor.calcMag(m_sensor.mx);
      const float magY   = m_sensor.calcMag(m_sensor.my);
      const float magZ   = m_sensor.calcMag(m_sensor.mz);

      // Pass Readings to Filter
      m_filter.update(gyroX, gyroY, gyroZ, accelX, accelY, accelZ, magX, magY, magZ);
   }
   // Extract Readings from Filter
   {
      m_angle.Pitch = m_filter.getPitch();
      m_angle.Roll  = m_filter.getRoll();
      m_angle.Yaw   = m_filter.getYaw();
   }

   return true;
}

// Do Not Call Forse Update Unless Required
// Use the More Safer Functions such as GetAngle, GetYaw, GetRoll, GetPitch

void SensorMPU9250::Update(const uint8_t p_sensors_update)
{
   const uint32_t micros_per_reading          = (1000000 / m_sample_rate_hz);
   const auto     micros_current_reading_time = micros();
   if (micros_current_reading_time - m_micros_prev_reading_time >= micros_per_reading)
   {
      // Force Update Will Update the Angle Value
      ForceUpdate(p_sensors_update);
      m_micros_prev_reading_time = micros();
   }
}

AngleType SensorMPU9250::ReadAngle(const uint8_t p_sensors_update)
{
   // Calling this will Update the Readings
   Update(p_sensors_update);
   return m_angle;
}

float SensorMPU9250::ReadPitch(const uint8_t p_sensors_update)
{
   return ReadAngle(p_sensors_update).Pitch;
}

float SensorMPU9250::ReadRoll(const uint8_t p_sensors_update)
{
   return ReadAngle(p_sensors_update).Roll;
}

float SensorMPU9250::ReadYaw(const uint8_t p_sensors_update)
{
   return ReadAngle(p_sensors_update).Yaw;
}

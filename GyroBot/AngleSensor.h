// This class defines the Angle Sensor
// It Makes writing code more easier than before
// As the more irrelevant parts can be hidden in the class Library

#ifndef _ANGLE_SENSOR.H_h
#define _ANGLE_SENSOR.H_h

// Portions of this code have been taken from
// https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library/blob/master/examples/MPU9250_Basic/MPU9250_Basic.ino
// Thanks to Sparkfun.com


// We are going to use MPU9250
// As such we use this library to
// Interact with it
// This Library makes interaction potentially more simpler
#include <SparkFunMPU9250-DMP.h>

// Used for Constants
// Note: Please read Constants.h before reading this file
#include "Constants.h"

// Used for Angle
// Note: Please read AngleType.h before reading this file
#include "AngleType.h"

// Used Madgwick Library
// For further details, read http://x-io.co.uk/res/doc/madgwick_internal_report.pdf
// Read Document and Header at Own Risk
#include "MadgwickAHRS.h"

// We shall use this to find the Angle from the sensor
struct AngleSensor
{
	// Protected Ensures Values not visible from Created Object
	// Only Visible here and in the class that inherits it
	// Refer to https://stackoverflow.com/questions/5447498/what-are-access-specifiers-should-i-inherit-with-private-protected-or-public
protected:
	Madgwick m_filter;
	MPU9250_DMP m_sensor;

	// Public ensures Visible from Created Object, Inheritor Class and here
	// Refer to https://stackoverflow.com/questions/5447498/what-are-access-specifiers-should-i-inherit-with-private-protected-or-public
public:
	// Call this function to Enable Sensor
	// Returns true if Enabled
	// False Otherwise
	bool connect()
	{
		// As initialising the Sensor failed, return false
		if (m_sensor.begin() != INV_SUCCESS)
			return false;

		// Enables both Gyro and Accelerometer
		m_sensor.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);

		// Sets the Gyro Full Scale Range
		m_sensor.setGyroFSR(CONSTANTS::GYRO::FULL_SCALE_RANGE);
		m_sensor.setAccelFSR(CONSTANTS::ACCEL::FULL_SCALE_RANGE);

		// Set LPF corner frequency to 5Hz
		// TODO: Find out WHAT is LPF Frequency in more simpler terms
		//	https://en.wikipedia.org/wiki/Low-pass_filter
		m_sensor.setLPF(5);

		// Both Sample Rates must be at 25 Hz
		m_sensor.setSampleRate(CONSTANTS::SAMPLE_RATE);
		m_filter.begin(CONSTANTS::SAMPLE_RATE);

		return true; // Initialising the Library was successful
	}

	// The Angle is calculated by first 
	// Obtaining values from Gyroscope
	// Then from Accelerometer
	// And then balancing them via Madgwick Filter

	Type::Angle<int16_t> getAngle()
	{
		// We would only use Accelerometer and Gyroscope Values
		// Call this function to extract values from Sensor
		// These values are stored in m_sensor
		m_sensor.update(UPDATE_ACCEL | UPDATE_GYRO);

		const float gyroX = m_sensor.calcGyro(m_sensor.gx);
		const float gyroY = m_sensor.calcGyro(m_sensor.gy);
		const float gyroZ = m_sensor.calcGyro(m_sensor.gz);
		
		const float accelX = m_sensor.calcAccel(m_sensor.ax);
		const float accelY = m_sensor.calcAccel(m_sensor.ay);
		const float accelZ = m_sensor.calcAccel(m_sensor.az);

		// Initial Values are from Gyroscope
		// Later values are from Accelerometer
		// This way the filter updates and obtains the Angle
		m_filter.updateIMU(gyroX, gyroY, gyroZ, accelX, accelY, accelZ);

		// Extract Angle Values from filter
		// Before implementing, please verify
		// What PITCH ROLL YAW is
		Type::Angle<int16_t> angle;
		angle.pitch = m_filter.getPitch();
		angle.roll = m_filter.getRoll();
		angle.yaw = m_filter.getYaw();

		return angle;
	}
};

#endif

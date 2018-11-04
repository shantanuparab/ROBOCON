#pragma once

#include <SparkFunMPU9250-DMP.h>
// Angle Sensor Uses MadgwickAHRS for Calculation Purposes
#include <MadgwickAHRS.h>

struct AngleType
{
	float Roll;
	float Pitch;
	float Yaw;

	AngleType(const float p_roll = 0.0f, const float p_pitch = 0.0f, const float p_yaw = 0.0f) : Roll{ p_roll }, Pitch{ p_pitch }, Yaw{ p_yaw }
	{

	}
};

struct SensorMPU9250
{
private:
	// Store the Angle Values
	AngleType m_angle{};
	// The MPU9250 Library
	MPU9250_DMP m_sensor;
	// Store the Filter Type
	Madgwick m_filter;

	// Previous Time At which reading taken
	uint32_t m_micros_prev_reading_time;

	// Store the Sample Rate
	const uint16_t m_sample_rate_hz;
	// Store the LPF Sample Rate
	// This maybe used to cut down the noise
	// https://community.emlid.com/t/mpu9250-configuration/176
	const uint16_t m_lpf_rate_hz;
	// Set the Default Sensors
	const uint8_t m_default_sensors;
	// The Gyro Full Scale Range
	const uint16_t m_gyro_fsr;
	// The Accelerometer Full Scale Range
	const uint8_t m_accel_fsr;

public:
	// Use the Constructor to Set the Default Values for Various Components
	// Including But not Limited to
	// Sample Rate (in Hz)
	// Gyro Full Scale Range
	// Accel Full Scale Range
	// Sensors Used
	// LPF Rate (in Hz)
	SensorMPU9250(
		const uint16_t p_sample_rate_hz = 10, /*Value Chosen Arbitrarily. Probably Wrong*/
		const uint16_t p_gyro_fsr = 250, /*Value used in Sample Code*/
		const uint8_t  p_accel_fsr = 2,   /*Value used in Sample Code*/
		const uint8_t  p_default_sensors = INV_XYZ_ACCEL | INV_XYZ_COMPASS |
		INV_XYZ_GYRO, /*Enable All Sensors by Default*/
		const uint16_t p_lpf_rate_hz = 5 /*Value used in Sample Code*/) :
		m_sample_rate_hz{ p_sample_rate_hz },
		m_lpf_rate_hz{ p_lpf_rate_hz }, m_default_sensors{ p_default_sensors },
		m_gyro_fsr{ p_gyro_fsr }, m_accel_fsr{ p_accel_fsr }
	{
	}
	// Connect to Sensor
	// Returns True When Connection Successful
	bool begin();
	// Do Not Call Forse Update Unless Required
	// Use the More Safer Functions such as GetAngle, GetYaw, GetRoll, GetPitch
	// Calling ForceUpdate on your own might prove dangerous
private:
	bool ForceUpdate(
		const uint8_t p_sensors_update = UPDATE_ACCEL | UPDATE_GYRO |
		UPDATE_COMPASS /*By Default Update All 3*/);
	// Do Not Call Forse Update Unless Required
	// Use the More Safer Functions such as GetAngle, GetYaw, GetRoll, GetPitch
private:
	void Update(
		const uint8_t p_sensors_update = UPDATE_ACCEL | UPDATE_GYRO |
		UPDATE_COMPASS /*By Default Update All 3*/);
public:
	AngleType ReadAngle(
		const uint8_t p_sensors_update = UPDATE_ACCEL | UPDATE_GYRO |
		UPDATE_COMPASS /*By Default Update All 3*/);
	float ReadPitch(
		const uint8_t p_sensors_update = UPDATE_ACCEL | UPDATE_GYRO |
		UPDATE_COMPASS /*By Default Update All 3*/);
	float ReadRoll(
		const uint8_t p_sensors_update = UPDATE_ACCEL | UPDATE_GYRO |
		UPDATE_COMPASS /*By Default Update All 3*/);
	float ReadYaw(
		const uint8_t p_sensors_update = UPDATE_ACCEL | UPDATE_GYRO |
		UPDATE_COMPASS /*By Default Update All 3*/);
};
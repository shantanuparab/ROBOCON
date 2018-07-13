#include <Arduino.h>

/*End of auto generated code by Atmel studio */

// Visual Micro is in vMicro>General>Tutorial Mode
//
/*
  Name:       GyroBot.ino
  Created:	30-06-2018 20:13:14
  Author:     Pratik
*/

//#include "AEMptyFile.h"

// ROSARIO Effect is similar to this video
//https://www.youtube.com/watch?v=APb0C52izGs
// Disables straight line motion
// Write #undef ROSARIO_EFFECT
// To Disable ROSARIO Effect
#define ROSARIO_EFFECT

// Used to control the Motor Driver
// Note: Please read MotorDriver.h before reading this file
#include "MotorDriver.h"

// Used to control the Sensor for Angle
// Note: Please read AngleSensor.h before reading this file
#include "AngleSensor.h"

// Global Value for the
// Angle Sensor
// That helps us obtain change in Angle and other components
AngleSensor g_angle_sensor;

// Global Value for the
// Motor Driver
// Helps act as middleman in driving vehicle

// Left Motor Driver
MotorDriver g_left_motor_driver(CONSTANTS::PIN::MOTOR::LEFT_DIRECTION1,
	CONSTANTS::PIN::MOTOR::LEFT_DIRECTION2, CONSTANTS::PIN::MOTOR::LEFT_PWM_SPEED);
// Right Motor Driver
MotorDriver g_right_motor_driver(CONSTANTS::PIN::MOTOR::RIGHT_DIRECTION1,
	CONSTANTS::PIN::MOTOR::RIGHT_DIRECTION2, CONSTANTS::PIN::MOTOR::RIGHT_PWM_SPEED);

Type::Angle<int16_t> g_initial_angle;

CONSTEXPR_VAR const uint32_t microsPerReading = (1000000 / CONSTANTS::SAMPLE_RATE);
uint32_t microsPrevious;

//TODO: Do Something with Proportionality
//Move Right
void moveRight(const float proportionality)
{
	// On a Right Turn
	// Left Wheel moves at Higher Speed
	// Than Right
	// As Left Wheel covers more distance
	// Than Right Wheel

	// TODO:-Values Might need to be adjusted
	g_left_motor_driver.setSpeed(100.0f* proportionality);
	g_right_motor_driver.setSpeed(-100.0f*proportionality);
}
//TODO: Do Something with Proportionality
//Move Left
void moveLeft(const float proportionality)
{
	// On a Left Turn
	// Right Wheel moves at Higher Speed
	// Than Left
	// As Right Wheel covers more distance
	// Than Left Wheel

	// TODO:-Values Might need to be adjusted
	g_left_motor_driver.setSpeed(-100.0f* proportionality);
	g_right_motor_driver.setSpeed(100.0f*proportionality);
}

#ifndef ROSARIO_EFFECT
void moveStraight()
{
	// On Straight both
	// wheels
	// Move with Same Speed
	// As both must cover equal distance

	g_left_motor_driver.setSpeed(90);
	g_right_motor_driver.setSpeed(90);
}
#endif // !ROSARIO_EFFECT

// The setup() function runs once each time the micro-controller starts
void setup()
{
	// initialize serial communication
	// (38400 chosen because it works as well at 8MHz as it does at 16MHz
	// Please set Arduino Serial Monitor and Serial Plotter
	// To this Baud rate when looking at Output
	// If not done, behaviour is undefined
	Serial.begin(38400/*Baud Rate*/);

	// Loop till connection with Sensor established
	while (!g_angle_sensor.connect())
	{
		// Display Error Message
		Serial.println("Connection with MPU9250 Failed");
		// Delay for 2 seconds
		// Sent to check Test Connection Next Time
		// Best not to attack the sensor too many times
		delay(2000 /*2 seconds sent as Milliseconds*/);
	}

	// initialize variables to pace updates to correct rate
	// micros reads the number of seconds since the Arduino
	// Board began running the Program
	g_initial_angle = g_angle_sensor.getAngle();
	microsPrevious = micros();

	#ifndef ROSARIO_EFFECT
	moveStraight();
	#else
	g_left_motor_driver.setSpeed(0);
	g_right_motor_driver.setSpeed(0);
	#endif // !ROSARIO_EFFECT
}

//TODO: Implement this entire function
float calculateProportionalityOfSpeedFromYaw(const int16_t p_change_in_yaw)
{
	return 0.0f;
}

// Add the main program code into the continuous loop() function
void loop()
{
	// check if it's time to read data and update the filter
	const auto microsNow = micros();
	if ((microsNow - microsPrevious) >= microsPerReading)
	{
		const Type::Angle<int16_t> current_angle = g_angle_sensor.getAngle();
		const int16_t change_in_yaw = current_angle.yaw - g_initial_angle.yaw;

		if (abs(change_in_yaw) > 1)
		{
			Serial.println(change_in_yaw);
			const float proportionality_speed_by_yaw = calculateProportionalityOfSpeedFromYaw(change_in_yaw);
			if (change_in_yaw < 1)
				moveLeft(proportionality_speed_by_yaw);
			else if (change_in_yaw > 1)
				moveRight(proportionality_speed_by_yaw);
		}
		else
		{
			Serial.println("Stable");
			#ifndef ROSARIO_EFFECT
			moveStraight();
			#endif // !ROSARIO_EFFECT
		}
		microsPrevious = microsPrevious + microsPerReading;
	}
}



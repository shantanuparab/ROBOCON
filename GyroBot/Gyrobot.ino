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

// NO STRAIGHTLINE MOTION is similar to this video
//https://www.youtube.com/watch?v=APb0C52izGs
// Disables straight line motion
// To Enable STRAIGHTLINE MOTION
// Write #undef DISABLE_STRAIGHTLINE_MOTION
#define DISABLE_STRAIGHTLINE_MOTION

// Used to control the Motor Driver
// Note: Please read MotorDriver.h before reading this file
#include "MotorDriver.h"

// Used to control the Sensor for Angle
// Note: Please read AngleSensor.h before reading this file
#include "AngleSensor.h"

// Global Value for the
// Angle Sensor
// That helps us obtain change in Angle
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

//Move Right
void moveRight(const int8_t p_turn_speed)
{
	// On a Right Turn
	// Left Wheel moves at Higher Speed
	// Than Right
	// As Left Wheel covers more distance
	// Than Right Wheel

	g_left_motor_driver.setSpeed(p_turn_speed);
	g_right_motor_driver.setSpeed(-p_turn_speed);
}
//Move Left
void moveLeft(const int8_t p_turn_speed)
{
	// On a Left Turn
	// Right Wheel moves at Higher Speed
	// Than Left
	// As Right Wheel covers more distance
	// Than Left Wheel

	g_left_motor_driver.setSpeed(-p_turn_speed);
	g_right_motor_driver.setSpeed(p_turn_speed);
}

//Halt
void moveHalt()
{
	g_left_motor_driver.setSpeed(0);
	g_right_motor_driver.setSpeed(0);
}

#ifndef DISABLE_STRAIGHTLINE_MOTION
void moveStraight()
{
	// On Straight both
	// wheels
	// Move with Same Speed
	// As both must cover equal distance

	g_left_motor_driver.setSpeed(90);
	g_right_motor_driver.setSpeed(90);
}
#endif // !DISABLE_STRAIGHTLINE_MOTION

// The setup() function runs once each time the micro-controller starts
void setup()
{
	// initialize serial communication
	// (38400 chosen because it works as well at 8MHz as it does at 16MHz)
	// Reasons Unknown
	// Please set Arduino Serial Monitor and Serial Plotter
	// To this Baud rate when looking at Output
	// If not done, behaviour is undefined
	Serial.begin(38400/*Baud Rate*/);

	// Loop till connection with Sensor established
	while (!g_angle_sensor.connect())
	{
		// Display Error Message
		Serial.println("Connection with MPU9250 Failed... Trying Again");
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

	#ifndef DISABLE_STRAIGHTLINE_MOTION
	moveStraight();
	#else
	g_left_motor_driver.setSpeed(0);
	g_right_motor_driver.setSpeed(0);
	#endif // !DISABLE_STRAIGHTLINE_MOTION
}

// Get Appropriate Speed Percentage for Appropriate Angle
// Using inline to recommend optimiser to substitute the function logic to the point where it's used
// For Further Details, read https://en.cppreference.com/w/cpp/language/inline
inline int8_t calculateTurnSpeedFromYaw(const int16_t p_change_in_yaw)
{
	// ONCHANGE:- Whoever changes the code
	// in this function also needs to edit these comments

	// The Yaw is being converted to Speed using a predefined table
	// Currently Taken With Respect to 15
	// As such let us see how many angles can 0-180 be 
	// divided with respect to table of 15

	// SR No.    FROM - TO
	//   1          0 - 15
	//   2         15 - 30
	//   3         30 - 45
	//   4         45 - 60
	//   5         60 - 75
	//   6         75 - 90
	//   7         90 - 105
	//   8        105 - 120
	//   9        120 - 135
	//  10        135 - 150
	//  11        150 - 165
	//  12        165 - 180

	// As such we find that the code can be divided into 12 divisions.
	// Now as Max Speed has to be 100%
	// Hence each division can be assumed to be equally divided
	// into 12 parts for each SR No.
	// As such if   0-15 has speed of 100/12
	// Then we see 15-30 has speed of (100/12)*2
	// And so on and so forth
	// A Mathematical Formula would be 100/12*(SR No. of Angle By Table)

	// Please check documentation related to abs function at
	// https://www.arduino.cc/reference/en/language/functions/math/abs/
	// basically abs(-15) == abs(15) == 15

	// The absolute value is then Integer Divided by 15
	// Now in Integer Divided, the decimal points are discarded
	// In Effect
	// 200/10 is 20 and 209/10 is also 20
	// Hence, On getting Angle of 14
	// And using Integer Division 14/15, we obtain 0
	// However, upon checking the table you would
	// find that 14 lies in SR No. 1(0-15 range)
	// As such the obtained value is incremented by 1

	// Then the formula Mentioned Above is Implemented

	// This code could also have been written in an if-else plan
	// But it might have created maintenance issues if 15 is later changed to 7.5
	// Also this version looks like Magic

	return abs(p_change_in_yaw) * 2.0f;
}

// Add the main program code into the continuous loop() function
void loop()
{
	// check if it's time to read data and change direction of Motor
	const auto microsNow = micros();
	if ((microsNow - microsPrevious) >= microsPerReading)
	{
		const Type::Angle<int16_t> current_angle = g_angle_sensor.getAngle();
		const int16_t change_in_yaw = current_angle.yaw - g_initial_angle.yaw;

		if (abs(change_in_yaw) > 1)
		{
			Serial.print("Angular Change =");
			Serial.print(change_in_yaw);

			Serial.print(" : PWM = ");
			const int8_t turn_speed_from_yaw = calculateTurnSpeedFromYaw(change_in_yaw);
			Serial.println(turn_speed_from_yaw);

			if (change_in_yaw < -1)
				moveLeft(turn_speed_from_yaw);
			else if (change_in_yaw > 1)
				moveRight(turn_speed_from_yaw);
		}
		else
		{
			Serial.println("Stable");
			#ifndef DISABLE_STRAIGHTLINE_MOTION
			moveStraight();
			#else
			moveHalt();
			#endif // !DISABLE_STRAIGHTLINE_MOTION
		}
		microsPrevious = microsPrevious + microsPerReading;
	}
}



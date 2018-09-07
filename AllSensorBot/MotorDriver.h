// MotorDriver.h
// This given file is required to abstract away the
// controls of a given motor driver away from the suer
// of this library

// Known as Include Guards
// Present to ensure that the given Header is not included twice
// For further details, read https://stackoverflow.com/a/8020211 (Thanks Shahbaz!)
#ifndef _MOTORDRIVER.H_h 
#define _MOTORDRIVER.H_h 

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include "WProgram.h"
#endif

struct MotorDriver
{
	// Protected Ensures Values not visible from Created Object
	// Only Visible here and in the class that inherits it
	// Refer to https://stackoverflow.com/questions/5447498/what-are-access-specifiers-should-i-inherit-with-private-protected-or-public
protected:
	//Note: Declared const as the values would not get changed
	// These store the values of PIN Numbers where motor is connected

	const byte m_dir1_pin;
	const byte m_dir2_pin;
	//PIN that takes analog values and controls speed. Also called PWM Pin
	const byte m_speed_pin;

	// Public ensures Visible from Created Object, Inheritor Class and here
	// Refer to https://stackoverflow.com/questions/5447498/what-are-access-specifiers-should-i-inherit-with-private-protected-or-public
public:
	MotorDriver(const byte p_dir1_pin, const byte p_dir2_pin, const byte p_speed_pin) :
		// These Statements Are used to Initialise the PIN Values
		m_dir1_pin{ p_dir1_pin },
		m_dir2_pin{ p_dir2_pin },
		m_speed_pin{ p_speed_pin }
	{
		//Initialise all PINS as Output
		pinMode(m_dir1_pin, OUTPUT);
		pinMode(m_dir2_pin, OUTPUT);
		pinMode(m_speed_pin, OUTPUT);

		// Motor Driver must initially be halted
		stop();
	}

	// The Speed must be provided in Percentage Form
	void setSpeed(const int8_t p_speed_percent)
	{
		// Percentage must always be less than or equal to 100
		// If not, do nothing
		// Please see documentation related to abs Macro
		//	https://www.arduino.cc/reference/en/language/functions/math/abs/
		if (abs(p_speed_percent) > 100)
			return;

		// If Speed Percent is Zero, HALT
		if (p_speed_percent == 0)
			return stop();
		// If Speed percent is Negative Reverse
		else if (p_speed_percent < 0)
			reverse();
		// if Speed Percent is Positive, Straight
		else if (p_speed_percent > 0)
			straight();
		
		// A PWM Pin on L298N maps to max value of 255
		// For further details check
		//https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
		//For details about what map does, please check
		//https://www.arduino.cc/reference/en/language/functions/math/map/
		// It basically converts value in range of 0-100 to value in range of 0-255

		// TODO: On the Basis of Recommendation,
		// Change value of 0-255 to 
		// Some Value found while Running Motor to 255

		// Please see documentation related to abs Macro
		//	https://www.arduino.cc/reference/en/language/functions/math/abs/

		const uint8_t speed_val = map(abs(p_speed_percent), 0, 100, 200/*TODO:- CHANGE VALUE*/, 255);
		
		// This Sends the Calculated Speed value to the Motor Driver
		analogWrite(m_speed_pin, speed_val);
	}

	// Public ensures Visible ONLY here
	// Refer to https://stackoverflow.com/questions/5447498/what-are-access-specifiers-should-i-inherit-with-private-protected-or-
private:
	// This function stops the motor from functioning
	// Using inline to recommend optimiser to substitute the function logic to the point where it's used
	// For Further Details, read https://en.cppreference.com/w/cpp/language/inline
	inline void stop()
	{
		// Setting both PINS to LOW Stops Motor from Working
		// For More information Please refer to Circuit Diagram of MOTOR DRIVER
		// Or refer to https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
		digitalWrite(m_dir1_pin, LOW);
		digitalWrite(m_dir2_pin, LOW);
	}

	// Call this function to Start the Motor Driver
	// For example setting HIGH-LOW may cause it to go straight or REVERSE
	inline void straight()
	{
		// Setting one PIN to LOW and OTHER to HIGH Starts Motor at Predefined Speed
		// For More information Please refer to Circuit Diagram of MOTOR DRIVER
		// Or refer to https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
		digitalWrite(m_dir1_pin, LOW);
		digitalWrite(m_dir2_pin, HIGH);
	}
	// Call this function to Reverse the Motor Driver
	inline void reverse()
	{
		// Setting one PIN to LOW and OTHER to HIGH Starts Motor at Predefined Speed
		// For More information Please refer to Circuit Diagram of MOTOR DRIVER
		// Or refer to https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
		digitalWrite(m_dir1_pin, HIGH);
		digitalWrite(m_dir2_pin, LOW);
	}
};

#endif





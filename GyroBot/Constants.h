// Constants.h
// Stores Constants Required in Implementation and Application 

#ifndef _BOT_CONSTANTS.H_h 
#define _BOT_CONSTANTS.H_h 

// Use this to access utilities useful for configuration
// Note: Please read Config.hxx before reading this file
#include "Config.h"

namespace CONSTANTS
{
	namespace PIN
	{
		namespace MOTOR
		{
			//TODO: Assign values by looking at board

			//This contains PIN Numbers required for operating
			//the LEFT Motor

			CONSTEXPR_VAR const byte LEFT_DIRECTION1 = 5; //NULL is Junk Value assigned. Please change it from Board Pins
			CONSTEXPR_VAR const byte LEFT_DIRECTION2 = 4;
			CONSTEXPR_VAR const byte LEFT_PWM_SPEED = 6;

			//This contains PIN Numbers required for operating
			//the RIGHT Motor

			CONSTEXPR_VAR const byte RIGHT_DIRECTION1 = 8;
			CONSTEXPR_VAR const byte RIGHT_DIRECTION2 = 9;
			CONSTEXPR_VAR const byte RIGHT_PWM_SPEED = 10;
		}//MOTOR PINS

	}//PIN
	namespace GYRO
	{
		CONSTEXPR_VAR const int8_t FULL_SCALE_RANGE = 250;
		// Full Scale Range of GYRO is 250 at FS_SEL=0
		// Please check data sheet
	}//GYRO
	namespace ACCEL
	{
		CONSTEXPR_VAR const int8_t FULL_SCALE_RANGE = 2;
		// Full Scale Range of Accel is 2g/s at AFS_SEL=0
		// Please check data sheet
	}//ACCEL
	
	CONSTEXPR_VAR const int8_t SAMPLE_RATE = 25;
}//CONSTANTS

#endif




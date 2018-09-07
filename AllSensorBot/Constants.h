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
    namespace MOTION
    {
      //TODO: Assign values by looking at board

      //This contains PIN Numbers required for operating
      //the LEFT Motor

/*      CONSTEXPR_VAR const byte LEFT_DIRECTION1 = 5; //NULL is Junk Value assigned. Please change it from Board Pins
      CONSTEXPR_VAR const byte LEFT_DIRECTION2 = 4;
      CONSTEXPR_VAR const byte LEFT_PWM_SPEED = 3;

      CONSTEXPR_VAR const byte RIGHT_DIRECTION1 = 8;
      CONSTEXPR_VAR const byte RIGHT_DIRECTION2 = 9;
      CONSTEXPR_VAR const byte RIGHT_PWM_SPEED = 2;
*/
      CONSTEXPR_VAR const byte LEFT_DIRECTION1 = 4; //NULL is Junk Value assigned. Please change it from Board Pins
      CONSTEXPR_VAR const byte LEFT_DIRECTION2 = 5;
      CONSTEXPR_VAR const byte LEFT_PWM_SPEED = 2;
      //This contains PIN Numbers required for operating
      //the RIGHT Motor

      CONSTEXPR_VAR const byte RIGHT_DIRECTION1 = 6;
      CONSTEXPR_VAR const byte RIGHT_DIRECTION2 = 7;
      CONSTEXPR_VAR const byte RIGHT_PWM_SPEED = 3;
    }//MOTOR MOTION PINS
    namespace PINNION
    {
      //TODO: Assign values by looking at board

      //This contains PIN Numbers required for operating
      //the LEFT Motor

      CONSTEXPR_VAR const byte LEFT_DIRECTION1 = 14; //NULL is Junk Value assigned. Please change it from Board Pins
      CONSTEXPR_VAR const byte LEFT_DIRECTION2 = 15;
      CONSTEXPR_VAR const byte LEFT_PWM_SPEED = 8;

      //This contains PIN Numbers required for operating
      //the RIGHT Motor

      CONSTEXPR_VAR const byte RIGHT_DIRECTION1 = 11;
      CONSTEXPR_VAR const byte RIGHT_DIRECTION2 = 12;
      CONSTEXPR_VAR const byte RIGHT_PWM_SPEED = 13;
    }//PINNION PINS91
    namespace PS2
    {
      CONSTEXPR_VAR const byte DATA = NULL;
      CONSTEXPR_VAR const byte COMMAND = NULL;
//      CONSTEXPR_VAR const byte  = NULL;
      CONSTEXPR_VAR const byte CLOCk = NULL;
    } // PS2
	}//PIN
}//CONSTANTS

#endif




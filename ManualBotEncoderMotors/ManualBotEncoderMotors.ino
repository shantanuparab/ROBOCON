// This file stores the List of Constants to be used
#include "Constants.h"

// This is an Encoder.h definition
// This Must Always be Defined Before Using Encoder.h
// Danger:- Delete this if using MORE than ONE Interrupt Pin
// Note:- Check Again if Multiple Interrupts Used
#define ENCODER_OPTIMIZE_INTERRUPTS

// Dare not Use Encoder without using Libraries
// For Details about this Library Check
// http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>

// Create Encoder Object
// NOTE
// Both PINS provided to Encoder
// Must be Interrupt Pins
// If they are not,
// We shall face a Performance Penalty
// Check List of Interrupt Pins for your board

Encoder g_encoder(Pin::Encoder::ENCODER_A, Pin::Encoder::ENCODER_B);

#include "SingleMotorController.h"

// FRONT LEFT
const SingleMotorController g_motor_fl(Pin::Motor::DIRECTION_FL, Pin::Motor::PWM_FL, LOW);
// FRONT RIGHT
const SingleMotorController g_motor_fr(Pin::Motor::DIRECTION_FR, Pin::Motor::PWM_FR, HIGH);
// BACK LEFT
const SingleMotorController g_motor_bl(Pin::Motor::DIRECTION_BL, Pin::Motor::PWM_BL, HIGH);
// BACK RIGHT
const SingleMotorController g_motor_br(Pin::Motor::DIRECTION_BR, Pin::Motor::PWM_BR, LOW);

// Stores the Previous Number of Pulses
// Volatile ensures that this variable is not 
// Optimised Away
volatile int32_t g_prev_number_of_pulses = 0;

// Converts Number Pulses to Distance Covered by Encoder
// Note that the Unit of Distance will be
// Same as Unit of Diameter
int32_t pulsesToDistance(const int32_t p_no_of_pulses)
{
	// distance = (PI * Diameter)*counts / (counts per complete revolution)
	return ((PI * Constants::OMNI_WHEEL_DIAMETER) * p_no_of_pulses) /
		Constants::ENCODER_COUNTS_PER_ROTATION;
}

// The setup() function runs once each time the micro-controller starts
void setup() {}

// Add the main program code into the continuous loop() function
void loop()
{
	// Get the Encoder Current Number of Pulses Detected
	const int32_t number_of_pulses = g_encoder.read();

	// If Changed then Act
	if (number_of_pulses != g_prev_number_of_pulses)
	{
      const int32_t distance_covered = pulsesToDistance(number_of_pulses);
		Serial.print("The Object Covered\t:");
		Serial.print(distance_covered);
		Serial.println();

	}

}

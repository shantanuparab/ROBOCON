// Ensures this Header File is only included Once while compiling
#pragma once

// This file Stores all the Constants Involved
// Example Pin Numbers
// Or OMNI Wheel's Diameter

// Please ensure that all NULL Statements have values

namespace Pin
{
   // Writing Pin would expand to this
   // For example
   // Pin pin;
   // and
   // byte pin;
   // would be the same statements
   using Pin = byte;

   namespace Motor
   {
		// FRONT LEFT
      const Pin DIRECTION_FL = NULL;
      const Pin PWM_FL       = NULL;
      // FRONT RIGHT
		const Pin DIRECTION_FR = NULL;
      const Pin PWM_FR       = NULL;
		// BACK LEFT
      const Pin DIRECTION_BL = NULL;
      const Pin PWM_BL       = NULL;
		// BACK RIGHT
      const Pin DIRECTION_BR = NULL;
      const Pin PWM_BR       = NULL;
   } // namespace Motor
   namespace Encoder
   {
      // NOTE
      // Both PINS provided to Encoder
      // Must be Interrupt Pins
      // If they are not,
      // We shall face a Performance Penalty
      // Check List of Interrupt Pins for your board

      const Pin ENCODER_A = NULL;
      const Pin ENCODER_B = NULL;
   } // namespace Encoder
} // namespace Pin

namespace Constants
{
   // Store the Omni Wheel DIAMETER here
   const int16_t OMNI_WHEEL_DIAMETER = NULL;

   // Store the Number of Pulses Per Revolution
   // Check Data Sheet for More details
   const int16_t ENCODER_COUNTS_PER_ROTATION = NULL;
} // namespace Constants

// Ensures this Header File is only included Once while compiling
#pragma once

// This file Stores all the Constants Involved
// Example Pin Numbers
// Or OMNI Wheel's Diameter

// Please ensure that all NULL Statements have values

// Writing Pin would expand to this
// For example
// Pin pin;
// and
// byte pin;
// would be the same statements
using Pin = byte;

const Pin IR_INPUT_LEFT  = NULL;
const Pin IR_INPUT_RIGHT = NULL;
const Pin IR_INPUT_FRONT = NULL;

// UART LSA08 PIN FORWARD Linked
const Pin SERIAL_ENABLE_FORWARD  = 28;
const Pin JUNCTION_PULSE_FORWARD = 26;
const Pin ADDRESS_FORWARD			= 3;
// UART LSA08 PIN SIDEWAYS Linked
const Pin SERIAL_ENABLE_SIDEWAYS  = NULL;
const Pin JUNCTION_PULSE_SIDEWAYS = NULL;
const Pin ADDRESS_SIDEWAYS			 = NULL;

// FRONT LEFT
const Pin DIRECTION_FL = 6;
const Pin PWM_FL		  = 7;
// FRONT RIGHT
const Pin DIRECTION_FR = 4;
const Pin PWM_FR		  = 5;
// BACK LEFT
const Pin DIRECTION_BL = 10;
const Pin PWM_BL		  = 11;
// BACK RIGHT
const Pin DIRECTION_BR = 8;
const Pin PWM_BR		  = 9;

/*
// NOTE
// Both PINS provided to Encoder
// Must be Interrupt Pins
// If they are not,
// We shall face a Performance Penalty
// Check List of Interrupt Pins for your board

const Pin ENCODER_A = 2;
const Pin ENCODER_B = 3;

// Store the Omni Wheel DIAMETER here
const int16_t OMNI_WHEEL_DIAMETER = 10;

// Store the Number of Pulses Per Revolution
// Check Data Sheet for More details
const int16_t ENCODER_COUNTS_PER_ROTATION = 2400;
*/

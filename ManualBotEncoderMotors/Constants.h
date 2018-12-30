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

// UART LSA08 PIN FORWARD Linked
Pin constexpr const FORWARD_ENABLE   = 24;
Pin constexpr const FORWARD_JUNCTION = 19;
Pin constexpr const FORWARD_ADDRESS  = 0;
// UART LSA08 PIN SIDEWAYS Linked
Pin constexpr const SIDEWAYS_ENABLE   = 28;
Pin constexpr const SIDEWAYS_JUNCTION = 26;
Pin constexpr const SIDEWAYS_ADDRESS  = 3;

// FRONT LEFT
Pin constexpr const DIRECTION_FL = 6;
Pin constexpr const PWM_FL       = 7;
// FRONT RIGHT
Pin constexpr const DIRECTION_FR = 4;
Pin constexpr const PWM_FR       = 5;
// BACK LEFT
Pin constexpr const DIRECTION_BL = 10;
Pin constexpr const PWM_BL       = 11;
// BACK RIGHT
Pin constexpr const DIRECTION_BR = 8;
Pin constexpr const PWM_BR       = 9;

// FRONT LEFT Encoder PINS
Pin constexpr const ENC_FL_U = 2;
Pin constexpr const ENC_FL_D = A7;
// FRONT RIGHT Encoder PINS
Pin constexpr const ENC_FR_U = 3;
Pin constexpr const ENC_FR_D = A4;
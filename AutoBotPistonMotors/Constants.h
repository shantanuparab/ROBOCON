// Ensures this Header File is only included Once while compiling
#pragma once

// This file Stores all the Constants Involved
// Example Pin Numbers

// Please ensure that all NULL Statements have values

// Writing Pin would expand to this
// For example
// Pin pin;
// and
// byte pin;
// would be the same statements
using Pin = byte;

// FRONT LEFT
Pin constexpr DIRECTION_FL = 6;
Pin constexpr PWM_FL       = 7;
// FRONT RIGHT
Pin constexpr DIRECTION_FR = 5;
Pin constexpr PWM_FR       = 4;
// BACK LEFT
Pin constexpr DIRECTION_BL = 8;
Pin constexpr PWM_BL       = 9;
// BACK RIGHT
Pin constexpr DIRECTION_BR = 3;
Pin constexpr PWM_BR       = 2;

// FRONT LEFT Encoder PINS
Pin constexpr ENC_FL_U = 24;
Pin constexpr ENC_FL_D = 25;
// FRONT RIGHT Encoder PINS
Pin constexpr ENC_FR_U = 29;
Pin constexpr ENC_FR_D = 28;
// BACK LEFT Encoder PINS
Pin constexpr ENC_BL_U = 27;
Pin constexpr ENC_BL_D = 26;
// BACK RIGHT Encoder PINS
Pin constexpr ENC_BR_U = 31;
Pin constexpr ENC_BR_D = 30;

// Encoder Counts
int32_t constexpr const ENC_PER_REV = 120240;

Pin constexpr const RESTART_MICRO_CONTROLLER_PIN = 33;

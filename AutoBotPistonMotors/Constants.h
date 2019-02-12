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
Pin constexpr DIRECTION_FL = 4;
Pin constexpr PWM_FL       = 5;
// FRONT RIGHT
Pin constexpr DIRECTION_FR = 2;
Pin constexpr PWM_FR       = 3;
// BACK LEFT
Pin constexpr DIRECTION_BL = 6;
Pin constexpr PWM_BL       = 7;
// BACK RIGHT
Pin constexpr DIRECTION_BR = 8;
Pin constexpr PWM_BR       = 9;

// FRONT LEFT Encoder PINS
Pin constexpr ENC_FL_U = 10;
Pin constexpr ENC_FL_D = 11;
// FRONT RIGHT Encoder PINS
Pin constexpr ENC_FR_U = 24;
Pin constexpr ENC_FR_D = 12;
// BACK LEFT Encoder PINS
Pin constexpr ENC_BL_U = 32;
Pin constexpr ENC_BL_D = 31;
// BACK RIGHT Encoder PINS
Pin constexpr ENC_BR_U = 30;
Pin constexpr ENC_BR_D = 29;

// Encoder Counts
int32_t constexpr ENC_PER_REV = 60'000;

Pin constexpr RESTART_MICRO_CONTROLLER_PIN = 33;

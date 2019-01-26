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
Pin constexpr const DIRECTION_FL = 8;
Pin constexpr const PWM_FL       = 9;
// FRONT RIGHT
Pin constexpr const DIRECTION_FR = 5;
Pin constexpr const PWM_FR       = 4;
// BACK LEFT
Pin constexpr const DIRECTION_BL = 3;
Pin constexpr const PWM_BL       = 2;
// BACK RIGHT
Pin constexpr const DIRECTION_BR = 6;
Pin constexpr const PWM_BR       = 7;

// FRONT LEFT Encoder PINS
Pin constexpr const ENC_FL_U = 30;
Pin constexpr const ENC_FL_D = 31;
// FRONT RIGHT Encoder PINS
Pin constexpr const ENC_FR_U = 29;
Pin constexpr const ENC_FR_D = 28;
// BACK LEFT Encoder PINS
Pin constexpr const ENC_BL_U = 26;
Pin constexpr const ENC_BL_D = 27;
// BACK RIGHT Encoder PINS
Pin constexpr const ENC_BR_U = 24;
Pin constexpr const ENC_BR_D = 25;

// Encoder Counts
int32_t constexpr const ENC_PER_REV = 120240;

// Please set Value for this PIN
Pin constexpr const RESTART_MICRO_CONTROLLER_PIN = NULL;

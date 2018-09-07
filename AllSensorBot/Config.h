//Config.h
// Stores the various defines that may or may or not help us make certain configuration processes easier

// Known as Include Guards
// Present to ensure that the given Header is not included twice
#ifndef _CONFIG.H_h 
#define _CONFIG.H_h 

// The constexpr specifier declares that it is possible to evaluate the value of the function or variable at compile time.
// Such variables and functions can then be used where only 
// compile time constant expressions are allowed (provided that appropriate function arguments are given)
// As the values stored as CONSTEXPR might be compile time constants, this is a nice help for an optimiser
// For Further details, read https://en.cppreference.com/w/cpp/language/constexpr

// As Constexpr was added in C++11
#if __cplusplus >= 201103L
#define CONSTEXPR_VAR constexpr
#else
// CONSTEXPR added in C++11
// If not C++11, do NOTHING
#define CONSTEXPR_VAR 
#endif

#endif // !_CONFIG_BOT_H




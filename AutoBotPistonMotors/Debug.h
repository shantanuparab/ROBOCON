// Ensures Header File is only included once
#pragma once

// Include the Arduino Header File
// We would use this to access the Serial.print function
#include <Arduino.h>

// Prints the Single Provided Statement
// If Debugging is Enabled
template<typename T>
inline void debug(T const p_show)
{
#ifdef DEBUG_ENABLED
   Serial.print(p_show);;
#endif
}

// Prints All Provided Statements
// If Debugging is Enabled
template<typename T, typename ...Args>
inline void debug(T const show, Args... rest)
{
   // Even though it's not required
   // Keeping this here
   // Might help save us a few iterations
#ifdef DEBUG_ENABLED
   debug(show);
   // Recursively calls this function again at compile time
   debug(rest...);
#endif
}

// Print All Provided Statements
// With New Line in the end
// If Debugging is Enabled
template<typename ...Args>
inline void debugln(Args... show)
{
   // Even though it's not required
   // Keeping this here
   // Might help save us a few iterations
#ifdef DEBUG_ENABLED
   debug(show..., '\n'/*Newline Character*/);
#endif
}
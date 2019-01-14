// This is Required to
// Enable Debugging
// This must always be placed
// before the library is included
#define DEBUG_ENABLED

// Note that the Library
// Must always be included
// After the Debugging Enabled Statement
// Is Written
#include <Debug.h>

void setup() {
  // It is required to set the baud rate
  // So that the display works
  // As in any Application
  Serial.begin(9600);

  debugln("Debugging is On");
  // Notice how we can use multiple types
  // In the Same Statement
  // Something Serial.print or Serial.println
  // Do not Support
  debug("I like the number ", 3, " very much");
}

void loop() {
  // put your main code here, to run repeatedly:

}

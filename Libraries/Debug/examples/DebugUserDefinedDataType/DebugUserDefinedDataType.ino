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

// Let us now define a type of our own
// It is a simple data type
struct X
{
  int val;
};

// In order to display this
// We shall create our own function
// That takes X as a parameter
// With the name Debug
void debug(X x)
{
  // Simply pass the value to debug
  // Notice that you could modify this to anything else as well
  debug(x.val);
}

void setup() {
  // It is required to set the baud rate
  // So that the display works
  // As in any Application
  Serial.begin(9600);

  debugln("Debugging is On");

  X x;
  x.val = 45;

  // Writing this with Serial.print
  // Results in an Error
  // Serial.print(x);

  // Notice however that 
  // As we defined debug function above
  // The code now works
  debugln("The Value of x is ",x);
}

void loop() {
  // put your main code here, to run repeatedly:

}

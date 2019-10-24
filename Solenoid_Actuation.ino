const byte outputPin1 = 14;  //Pin Number 14 is Output
const byte outputPin2 = 12;  // Pin Number 12 is Output

void setup() 
{
  pinMode(outputPin1, OUTPUT);   
  pinMode(outputPin2, OUTPUT);
}

void loop()
{
   digitalWrite(outputPin1, HIGH);   // First output pin is made high
   digitalWrite(outputPin2, LOW);    // Second output pin is made low

   delay(1000);

   digitalWrite(outputPin1, LOW);  // First ouput pin is made low
   digitalWrite(outputPin2, HIGH);  // Second output pin is made high

   delay(1000);
}
 

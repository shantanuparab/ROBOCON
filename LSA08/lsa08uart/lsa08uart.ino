const byte rx = 15;
const byte tx = 14;
const byte UART_serial_enable = 2;
const byte left_wheel_dir = 12;
const byte right_wheel_dir = 13;
const byte left_wheel_pwm = 10;
const byte right_wheel_pwm = 11;

void setup()
{
  pinMode(UART_serial_enable, OUTPUT);

  for (byte i = 10; i <= 13; i++)             //Output pins 10-11-12-13 for dir and pwm
  {
    pinMode(i, OUTPUT);
  }

  digitalWrite(UART_serial_enable, HIGH);

  digitalWrite(left_wheel_pwm, LOW);
  digitalWrite(right_wheel_pwm, LOW);

  digitalWrite(left_wheel_dir, HIGH);
  digitalWrite(right_wheel_dir, HIGH);

  Serial3.begin(9600);                      //For LSA08 to Arduino communication (Mega only)
  Serial.begin(9600);                       //For comp to Arduino communication
}

void loop() {

  byte dummy = 0;                           //Dummy variable to read input from LSA08

  digitalWrite(UART_serial_enable, LOW);    //Enable pin set LOW to request data from LSA08

  while (Serial3.available() > 0)           //Check whether data from LSA08 is available
  {
    dummy = Serial3.read();                 //Read data from LSA08
    Serial.println(dummy);
  }

  digitalWrite(UART_serial_enable, HIGH);    //Enable pin set HIGH to stop requesting data from LSA08

  //Move left - sensor 1-2
  if ((dummy > 0) && (dummy < 21))
  {
    moveLeft();
    //readVal();
  }
  //To elimenate range of non working sensor 3
  else if ((dummy > 20) && (dummy < 31))
  {
    Serial.println("N/A");
    //readVal();
  }
  //Move straight - sensor 4-5
  else if ((dummy > 30) && (dummy < 51))
  {
    moveStraight();
    //readVal();
  }
  //Move right - sensor 6-7-8
  else if ((dummy > 52) && (dummy < 71))
  {
    moveRight();
    //readVal();
  }
  
  //Stop bot if no line detected 
  /*else
    {
    digitalWrite(left_wheel_pwm,0);
    digitalWrite(right_wheel_pwm,0);
    Serial.println("Waiting");
    }*/
  
  //Delay to stabilize movement of bot
  delay(100);
}
void moveLeft()
{
  //Left wheel stopped for turn, right wheel pwm increased
  analogWrite(left_wheel_pwm, 0);
  analogWrite(right_wheel_pwm, 140);
  //Serial.println("Left");
}
void moveRight()
{
  //Right wheel stopped for turn, left wheel pwm increased
  analogWrite(left_wheel_pwm, 140);
  analogWrite(right_wheel_pwm, 0);
  //Serial.println("Right");
}
//Function to read pwm value of both wheels - NOT FUNCTIONAL
/*void readVal()
  {
  int pVal1;
  int pVal2;
  pVal1 = analogRead(left_wheel_pwm);
  //map values from analog to pwm
  //pVal1 = map(pVal1, 0 , 1023, 0, 255);
  pVal2 = analogRead(right_wheel_pwm);
  //map values from analog to pwm
  //pVal2 = map(pVal2, 0, 1023, 0, 255);
  //Serial.println(pVal1);
  //Serial.println(pVal2);
  //Serial.println("---");
  }*/
//Move straight - self explanatory
void moveStraight()
{
  analogWrite(left_wheel_pwm, 140);
  analogWrite(right_wheel_pwm, 140);
  //Serial.println("Straight");
}



const byte rx = 15;
const byte tx = 14;
const byte UART_serial_enable = 2;
const byte right_wheel_dir= 12;
const byte left_wheel_dir = 13;
const byte right_wheel_pwm = 10;
const byte left_wheel_pwm = 11;

//Halt the bot - no line sensed
void dontMove()
{
  analogWrite(left_wheel_pwm, 0);
  analogWrite(right_wheel_pwm, 0);
}

void moveRight(int control)
{
  analogWrite(right_wheel_pwm,127-control);
  analogWrite(left_wheel_pwm,127+control);
}

void moveLeft(int control)
{
  analogWrite(right_wheel_pwm,127+control);
  analogWrite(left_wheel_pwm,127-control);
}

void moveStraight()
{
  analogWrite(right_wheel_pwm,127);
  analogWrite(left_wheel_pwm,127);
}

//P implementation - takes in output from sensor and returns control signal
int lineP(int output)
{
  int setPoint = 35;                          //line center at 35
  int error;
  int control_signal;
  float Kp = 0.0;                             //arbitary value 255/(2*35)=3.63
  error = setPoint - output;
  control_signal = Kp * error;
  return control_signal;
}


void setup()
{
  pinMode(UART_serial_enable, OUTPUT);

  for (byte i = 10; i <= 13; i++)            //Output pins 10-11-12-13 for dir and pwm
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

void loop()
{

  byte dummy = 0;                           //Dummy variable to read input from LSA08
  int control;

  digitalWrite(UART_serial_enable, LOW);    //Enable pin set LOW to request data from LSA08

  while (Serial3.available() > 0)           //Check whether data from LSA08 is available
  {
    dummy = Serial3.read();                 //Read data from LSA08
    if (dummy != 255)                       //LSA08 returns 255 for no line detected
    {
      control = lineP(dummy);               //P control function for input
      Serial.println(dummy);
      //When bot is to the extreme right dummy=0 and error = 35-0 = 35
      //Hence it needs to take a left turn
      //Max value for control = (35*Kp) = +127
      //So right wheel is sped up (right+127) while left wheel stops (left-127)
      //When bot is to the extreme left dummy=70 and error = 35-70 = -35
      //Hence it needs to take a right turn
      //Min value for control = (-35*Kp) = -127
      //So left wheel is sped up (left-(-127)) while right wheel stops (right+(-127))
      control=lineP(dummy);
      if(dummy<0)
      {
        moveLeft(abs(control));
      }
      else if(dummy>0)
      {
        moveRight(abs(dummy));
      }
      else
      moveStraight();
    }
    else
      dontMove();                             //Halt when dummy = 255

  }

  digitalWrite(UART_serial_enable, HIGH);    //Enable pin set HIGH to stop requesting data from LSA08


  //Delay to stabilize movement of bot - replace with millis?
  delay(100);
}

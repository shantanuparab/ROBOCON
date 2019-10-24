
          //Black Cytron
#define dir1 8
#define pwm1 9
#define dir2 10
#define pwm2 12

          //Raspberry
#define dir3 4
#define pwm3 5
#define dir4 6
#define pwm4 7
void setup()
{
 pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(dir4, OUTPUT);
  
  Serial.begin(115200);
}
void loop()
{
}
}
void moveNorth()
{
   digitalWrite(dir1, HIGH);
   digitalWrite(dir2, HIGH);
   digitalWrite(dir3, HIGH);
   digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveSouth()
{
   digitalWrite(dir1, LOW);
   digitalWrite(dir2, LOW);
   digitalWrite(dir3, LOW);
   digitalWrite(dir4, LOW);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveEast()
{
   digitalWrite(dir1, HIGH);
   digitalWrite(dir2, LOW);
   digitalWrite(dir3, HIGH);
   digitalWrite(dir4, LOW);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveWest()
{
   digitalWrite(dir1, LOW);
   digitalWrite(dir2, HIGH);
   digitalWrite(dir3, LOW);
   digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 150);
}

void moveNorthWest()
{
   //digitalWrite(dir1, LOW);
   digitalWrite(dir2, HIGH);
   digitalWrite(dir3, HIGH);
   //digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 0);  
}

void moveNorthEast()
{
   digitalWrite(dir1, HIGH);
   //digitalWrite(dir2, HIGH);
   //digitalWrite(dir3, HIGH);
   digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 0);
   analogWrite(pwm3, 0);
   analogWrite(pwm4, 150);  
}

void moveSouthWest()
{
   digitalWrite(dir1, LOW);
   //digitalWrite(dir2, HIGH);
   //digitalWrite(dir3, HIGH);
   digitalWrite(dir4, LOW);
   analogWrite(pwm1, 150);
   analogWrite(pwm2, 0);
   analogWrite(pwm3, 0);
   analogWrite(pwm4, 150);  
}

void moveSouthEast()
{
   //digitalWrite(dir1, LOW);
   digitalWrite(dir2, LOW);
   digitalWrite(dir3, LOW);
   //digitalWrite(dir4, HIGH);
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 150);
   analogWrite(pwm3, 150);
   analogWrite(pwm4, 0);  
}

void wait()
{
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 0);
   analogWrite(pwm3, 0);
   analogWrite(pwm4, 0);
}

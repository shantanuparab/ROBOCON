const byte dir1=8;  //br
const byte pwm1=9;
const byte dir2=10; //fl
const byte pwm2=12;
const byte dir3=3;  //fr
const byte pwm3=4;
const byte dir4=
const byte pwm4=

void setup() {
  // put your setup code here, to run once:
  pinMode(dir1, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir3, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(dir4, OUTPUT);
  pinMode(pwm4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  rotate(50);
}

void rotate(int pwm)
{
  digitalWrite(dir1,HIGH);
  digitalWrite(dir2,HIGH);
  digitalWrite(dir3,HIGH);
  digitalWrite(dir4,HIGH);
  analogWrite(pwm1,pwm);
  analogWrite(pwm2,pwm);
  analogWrite(pwm3,pwm);
  analogWrite(pwm4,pwm);
}

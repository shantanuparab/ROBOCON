<<<<<<< Updated upstream
const byte dirbr=6;  //br
const byte pwmbr=7;
const byte dirfl=10; //fl
const byte pwmfl=11;
const byte dirfr=8;  //fr
const byte pwmfr=9;
const byte dirbl= 4;// bl
const byte pwmbl= 5;
=======
const byte dir1=4;  //br
const byte pwm1=5;
const byte dir2=6; //fl
const byte pwm2=7;
const byte dir3=8;  //fr
const byte pwm3=9;
const byte dir4=10;
const byte pwm4=12;
>>>>>>> Stashed changes

void setup() {
  // put your setup code here, to run once:
  pinMode(dirbr, OUTPUT);
  pinMode(pwmbr, OUTPUT);
  pinMode(dirfl, OUTPUT);
  pinMode(pwmfl, OUTPUT);
  pinMode(dirfr, OUTPUT);
  pinMode(pwmfr, OUTPUT);
  pinMode(dirbl, OUTPUT);
  pinMode(pwmbl, OUTPUT);

  Serial.begin(9600);
  analogWrite(pwmfl,0);
  analogWrite(pwmfr,0);
  analogWrite(pwmbl,0);
  analogWrite(pwmbr,0);
  delay(3000);
  halt();
}

void loop() {
  // put your main code here, to run repeatedly:
  halt();
  rotateDiag1(150);
  delay(1650);
  halt();
  rotateDiag2(150);
  delay(1650);
}
void halt()
{
  analogWrite(pwmfl,0);
  analogWrite(pwmfr,0);
  analogWrite(pwmbl,0);
  analogWrite(pwmbr,0);
  
}
void rotateDiag1(int pwm)
{
  Serial.print(F("Dir FL\t:"));
  Serial.println(dirfl);
//  analogWrite(pwmfl,0);
//  analogWrite(pwmfr,0);
//  analogWrite(pwmbl,0);
//  analogWrite(pwmbr,0);
  digitalWrite(dirfl,LOW);
  analogWrite(pwmfl,pwm);
//  delay(3000);
  Serial.print(F("Dir BR\t:"));
  Serial.println(dirbr);
  digitalWrite(dirbr,LOW);
//  analogWrite(pwmfl,0);
//  analogWrite(pwmfr,0);
//  analogWrite(pwmbl,0);
//  analogWrite(pwmbr,0);
  analogWrite(pwmbr,pwm);
//  delay(3000);
}
void rotateDiag2(int pwm)
{
  Serial.print(F("Dir FR\t:"));
  Serial.println(dirfr);
  digitalWrite(dirfr,HIGH);
//  analogWrite(pwmfl,0);
//  analogWrite(pwmfr,0);
//  analogWrite(pwmbl,0);
//  analogWrite(pwmbr,0);
  analogWrite(pwmfr,pwm);
//  delay(3000);
  Serial.print(F("Dir BL\t:"));
  Serial.println(dirbl);
  digitalWrite(dirbl,HIGH);
//  analogWrite(pwmfl,0);
//  analogWrite(pwmfr,0);
//  analogWrite(pwmbl,0);
//  analogWrite(pwmbr,0);
  analogWrite(pwmbl,pwm);
//  delay(3000);
}

#define dir1c 13
#define pwm1c 12
#define dir2c 11
#define pwm2c 10

         //Raspberry
#define dir1r 4
#define pwm1r 5
#define dir2r 3
#define pwm2r 6
int i,p;

void setup()
{
  Serial.begin(115200);
  pinMode(pwm1r,OUTPUT);
  pinMode(pwm2r,OUTPUT);

  pinMode(dir1r,OUTPUT);
  pinMode(dir2r,OUTPUT);
  pinMode(pwm1c,OUTPUT);
  pinMode(pwm2c,OUTPUT);
  pinMode(dir1c,OUTPUT);
  pinMode(dir2c,OUTPUT);

  // put your setup code here, to run once:

}

void loop() 
{
  for(i=0;i<=200;)
  {
  forward();
  delay(500);
  Serial.println("yes");
  Serial.println(i);
  i=i+10;
  p=i;
  }
  for(i=200;i>0;)
  {
    forward();
  delay(500);
  Serial.println("yes");
  Serial.println(i);
  i=i-10;
  p=i;
  }
  /*backward();
    delay(2000);
      Serial.println("yes");
    left();
      delay(2000);
        Serial.println("yes");
      right();
        delay(2000);
          Serial.println("yes");
        diagonallyL();
          delay(2000);
            Serial.println("yes");
          diagonallyR();
            delay(2000);
              Serial.println("yes");*/
  
  
}
  void forward()
  {
    /*analogWrite(pwm1r,100);
    analogWrite(pwm2r,100);
    
    digitalWrite(dir1r,HIGH);
    digitalWrite(dir2r,HIGH);*/
    analogWrite(pwm1c,p);
    analogWrite(pwm2c,p);
    digitalWrite(dir1c,HIGH);
   
    digitalWrite(dir2c,LOW);
    delay(1000);
     digitalWrite(dir1c,LOW);
     
    digitalWrite(dir2c,HIGH);
    delay(1000);
  }

  
  void backward()
  {
    analogWrite(pwm1r,70);
    analogWrite(pwm2r,70);
    analogWrite(pwm1c,70);
    analogWrite(pwm2c,70);
    digitalWrite(dir1r,HIGH);
    digitalWrite(dir2r,HIGH);
    digitalWrite(dir1c,HIGH);
    digitalWrite(dir2c,HIGH);
  }

  
  void diagonallyR()
  {
    analogWrite(pwm1r,0);
    analogWrite(pwm2r,70);
    analogWrite(pwm1c,70);
    analogWrite(pwm2c,0);
    digitalWrite(dir1r,HIGH);
    digitalWrite(dir2r,HIGH);
    digitalWrite(dir1c,HIGH);
    digitalWrite(dir2c,HIGH);
  }

  
  void diagonallyL()
  {
    analogWrite(pwm1r,70);
    analogWrite(pwm2r,0);
    analogWrite(pwm1c,0);
    analogWrite(pwm2c,70);
    digitalWrite(dir1r,HIGH);
    digitalWrite(dir2r,HIGH);
    digitalWrite(dir1c,HIGH);
    digitalWrite(dir2c,HIGH);
  }

  
  void right()
  {
    analogWrite(pwm1r,70);
    analogWrite(pwm2r,70);
    analogWrite(pwm1c,70);
    analogWrite(pwm2c,70);
    digitalWrite(dir1r,LOW);
    digitalWrite(dir2r,HIGH);
    digitalWrite(dir1c,HIGH);
    digitalWrite(dir2c,LOW);
  }

  
  void left()
  {
   analogWrite(pwm1r,70);
    analogWrite(pwm2r,70);
    analogWrite(pwm1c,70);
    analogWrite(pwm2c,70);
    digitalWrite(dir1r,HIGH);
    digitalWrite(dir2r,LOW);
    digitalWrite(dir1c,LOW);
    digitalWrite(dir2c,HIGH);
  }

  // put your main code here, to run repeatedly:

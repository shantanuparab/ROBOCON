char a=0;
char b=0;
int c=0;
 int e,d=0;
void setup() {
  
  Serial.begin(9600);
  pinMode(e,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  
  Serial.println("enter the pin number:");
  if (Serial.available() > 0)
  {
    
   Serial.println("enter the first digit"); 
   delay(2000);
   a=Serial.read();
   
   Serial.println("enter the second digit"); 
   delay(2000);
   b=Serial.read();
   c=a-'0';
   d=b-'0';
   e=(c*10)+d;
   
    //c=d.toInt();
    Serial.println(e);
    digitalWrite(e,HIGH);
    Serial.println("e is high");
    delay(7000);
    digitalWrite(e,LOW);
    Serial.println("e is low");
    delay(1000);
   

    
  }
  // put your main code here, to run repeatedly:

}


int slotPin = 2;
int motDir = 10;
int motPwm = 9;
volatile unsigned long counter = 0;

unsigned long now,last,dt;
void _ISR() {
  now = micros();
  dt = now-last;
  last = now;
  if(dt>500){
    counter++;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(slotPin,INPUT);
  pinMode(motDir,OUTPUT);
  pinMode(motPwm,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(slotPin),_ISR,CHANGE);
  digitalWrite(motDir,HIGH);
  
}
long time1 = 0;
unsigned long pulses;
float rotate = 0;
unsigned long pulsesToRotate;
int motSpeed = 0,baseSpeed = 50;
float Kp = 0.8,Kp1 = 0.8;
void loop() {
  pulses = counter / 2;
  if(Serial.available()) {
    rotate = Serial.parseFloat();
    counter = 0;
    pulses = 0;
    motSpeed = 0;
    time1 = micros();
    pulsesToRotate = rotate * 100 - pulses;
  }
  if(pulsesToRotate <= 0) {
    analogWrite(motPwm,0);
  }
  else {
    pulsesToRotate = rotate * 100 - pulses;
    if(pulses < (rotate * 100)*0.75 && motSpeed < 255 ) {
      //Accelerate
      motSpeed = (micros()-time1)*Kp/1000.0;
    }
    else if(pulses > (rotate * 100)*0.75) {
      //Deccelerate
      motSpeed = baseSpeed + (Kp1 * pulsesToRotate);
    }
    else {
      //constant Speed  
    }
    motSpeed = motSpeed>255?255:motSpeed;
    motSpeed = motSpeed<0?0:motSpeed;
    analogWrite(motPwm,motSpeed);
  }
  Serial.print(pulsesToRotate);
  Serial.print(" ");
  Serial.print(pulses);
  Serial.print(" ");
  Serial.println(motSpeed);
    
}

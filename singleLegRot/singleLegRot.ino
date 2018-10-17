
int slotPin = 2;
int motDir = 9;
int motPwm = 8;
volatile bool f = false;
volatile unsigned long counter = 0;
volatile unsigned long abcd = 0;
volatile unsigned long xyz = 0;
volatile unsigned long now,last,dt; 
volatile unsigned long maxDt=0;

void _ISR() {
  now = micros();
  dt = now-last;
  last = now;
  if(dt>30){
    counter++;
    abcd = 0;
  }
  else {
    if(dt>maxDt) {
      maxDt = dt;
    }
    abcd++;
    xyz++;
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
long motSpeed = 0,baseSpeed = 15;
//leg Kp = 0.8 Kp1 = 0.8 base speed = 60
float Kp = 0.8,Kp1 = 0.35;
void loop() {
  pulses = counter / 2;
  if(Serial.available()) {
    rotate = Serial.parseFloat();
    counter = 0;
    pulses = 0;
    motSpeed = 0;
    abcd=0;
    xyz=0;
    maxDt=0;
    time1 = micros();
    pulsesToRotate = rotate * 100 - pulses;
  }
  if(pulsesToRotate <= 0) {
    analogWrite(motPwm,0);
  }
  else {
    pulsesToRotate = rotate * 100 - pulses;
    if(pulses < (rotate * 100)*0.25 && motSpeed < 255 ) {
      //Accelerate
      motSpeed = (micros()-time1)*Kp/1000.0;
    }
    else if(pulses > (rotate * 100)*0.25) {
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
  Serial.print(abcd);
  Serial.print(" ");
  Serial.print(xyz);
  Serial.print(" ");
  Serial.print(pulsesToRotate);
  Serial.print(" ");
  Serial.print(pulses);
  Serial.print(" ");
  Serial.print(motSpeed);
  Serial.print(" ");
  Serial.print(maxDt);
  Serial.println();
}

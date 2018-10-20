volatile unsigned long count1 = 1;
volatile unsigned long count2 = 1;
volatile unsigned long dt1, now1, last1 = 0;
volatile unsigned long dt2, now2, last2 = 0;
volatile unsigned long dt, now, last = 0;
int pwmFor1;
int pwmFor2;
int diff;

int dir_1 = 4;
int pwm_1 = 5;
int dir_2 = 6;
int pwm_2 = 7;
int slot1 = 2;
int slot2 = 3;
void setup() {
  pinMode(dir_1, OUTPUT);
  pinMode(pwm_1, OUTPUT);
  pinMode(dir_2, OUTPUT);
  pinMode(pwm_2, OUTPUT);
  attachInterrupt(0, counting1, RISING);
  attachInterrupt(1, counting2, RISING);
  Serial.begin(9600);
}
int prev_count1 = 0;
int prev_count2 = 0;

void loop() {
  now = millis();
  dt = now - last;
  if (dt > 20) {
    diff = count1-count2;
    pwmFor1 = 100;
    pwmFor1 = (100 - (5*diff));
    pwmFor2 = (100 + (5*diff));
    pwmFor2 = constrain(pwmFor2, 0, 255);
    pwmFor1 = constrain(pwmFor1, 0, 255);
      
  }
  
  digitalWrite(dir_1, HIGH);
  analogWrite(pwm_1, pwmFor1);
  digitalWrite(dir_2, HIGH);
  analogWrite(pwm_2, pwmFor2);
  Serial.print(pwmFor1);
  Serial.print(" ");
  Serial.print(pwmFor2);
  Serial.print(" ");
  Serial.print(diff);
  Serial.print(" ");
  Serial.println();
  
  
}





void counting1() {
  now1 = micros();
  dt1 = now1 - last1;
  last1 = now1;
  if (dt1 > 30) {
    count1 = count1 + 1;
  }
}
void counting2() {
  now2 = micros();
  dt2 = now2 - last2;
  last2 = now2;
  if (dt2 > 30) {
    count2 = count2 + 1;
  }
}

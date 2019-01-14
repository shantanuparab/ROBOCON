#include <Encoder.h>

volatile unsigned long dt1, now1, last1 = 0;
volatile unsigned long dt2, now2, last2 = 0;
volatile unsigned long long last = 0;

int dir_1 = 4;
int pwm_1 = 5;
int dir_2 = 6;
int pwm_2 = 7;

Encoder enc1(19, 18);
//Encoder enc2(,);

void setup() {
  /*  pinMode(dir_1, OUTPUT);
    pinMode(pwm_1, OUTPUT);
    pinMode(dir_2, OUTPUT);
    pinMode(pwm_2, OUTPUT);*/
  Serial.begin(9600);
}
int prev_count1 = 0;
int prev_count2 = 0;

void loop() {
  auto const now = micros();
  auto const dt = now - last;
  if (dt > 200) {
    auto const count1 = enc1.read();
    //  auto const count2 = enc2.read();
    auto const diff = count1;// - count2;
    /*   auto pwmFor1 = (100 - (5 * diff));
       auto pwmFor2 = (100 + (5 * diff));
       pwmFor2 = constrain(pwmFor2, 0, 255);
       pwmFor1 = constrain(pwmFor1, 0, 255);
    */
    last = micros();
    Serial.println(diff);
    /*
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
    */
  }
}



void north(int speedrun) {
  digitalWrite(mot1, cw_1);
  digitalWrite(mot2, cw_2);
  digitalWrite(mot3, cw_3);
  digitalWrite(mot4, cw_4);
  //setSpeed();
 

  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, calibrated_pwm4(speedrun));
}

void south(int speedrun) {
  digitalWrite(mot1, acw_1);
  digitalWrite(mot2, acw_2);
  digitalWrite(mot3, acw_3);
  digitalWrite(mot4, acw_4);
//setSpeed();
 
  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, calibrated_pwm4(speedrun));
}
void east(int speedrun) {
  digitalWrite(mot1, cw_1);
  digitalWrite(mot2, acw_2);
  digitalWrite(mot3, cw_3);
  digitalWrite(mot4, acw_4);
  //setSpeed();
 

  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, calibrated_pwm4(speedrun));
}
void west(int speedrun) {
  digitalWrite(mot1, acw_1);
  digitalWrite(mot2, cw_2);
  digitalWrite(mot3, acw_3);
  digitalWrite(mot4, cw_4);
  //setSpeed();
 

  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, calibrated_pwm4(speedrun));
}
void northeast(int speedrun) {
  digitalWrite(mot1, cw_1);
  digitalWrite(mot4, cw_4);
//  setSpeed();
 

  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
//  analogWrite(pwm4, calibrated_pwm4(speed));
}
void northwest(int speedrun) {
  digitalWrite(mot2, cw_2);
  digitalWrite(mot3, cw_3);
 // setSpeed();
 

  analogWrite(pwm1, 0);
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, 0);
}
void southeast(int speedrun) {
  digitalWrite(mot2, acw_2);
  digitalWrite(mot3, acw_3);
//setSpeed();
 
  analogWrite(pwm1, 0);
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, 0);
}
void southwest(int speedrun) {
  digitalWrite(mot1, acw_1);
  digitalWrite(mot4, acw_4);
//setSpeed();
 
  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, calibrated_pwm4(speedrun));
}

void stop_moving() {
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
}

void clockwise(int speedrun) {
  digitalWrite(mot1, acw_1);
  digitalWrite(mot2, cw_2);
  digitalWrite(mot3, cw_3);
  digitalWrite(mot4, acw_4);
  //setSpeed();
 
  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, calibrated_pwm4(speedrun));
}

void counterclockwise(int speedrun) {
  digitalWrite(mot1, cw_1);
  digitalWrite(mot2, acw_2);
  digitalWrite(mot3, acw_3);
  digitalWrite(mot4, cw_4);
  //setSpeed();
 
  analogWrite(pwm1, calibrated_pwm1(speedrun));
  analogWrite(pwm2, calibrated_pwm2(speedrun));
  analogWrite(pwm3, calibrated_pwm3(speedrun));
  analogWrite(pwm4, calibrated_pwm4(speedrun));
}




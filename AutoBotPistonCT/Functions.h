void moveMot(char motNum, int spd) {
  digitalWrite(motDir[motNum], spd >= 0 ? LOW : HIGH);
  spd = abs(spd);
  spd = map(spd, 0, 255, 255, 0);
  analogWrite(motPwn[motNum], spd);
}

void moveAllMot(int pwm0, int pwm1, int pwm2, int pwm3) {
  moveMot(0, -pwm0);
  moveMot(1, pwm1);
  moveMot(2, pwm2);
  moveMot(3, -pwm3);
}

void stopAllMot() {
  moveMot(0, 0);
  moveMot(1, 0);
  moveMot(2, 0);
  moveMot(3, 0);
  enc0.write(0);
  enc1.write(0);
  enc2.write(0);
  enc3.write(0);
}

void motorsInit() {
  for (int i = 0; i < 4; i++) {
    pinMode(motPwn[i], OUTPUT);
    pinMode(motDir[i], OUTPUT);
    moveMot(i, 0);
  }
}

void leg12Sync(bool dir,long countToMove) {
  auto enc1Count = enc1.read();
  auto enc2Count = enc2.read();
  long error,correction,masterPwm;
  if (dir) {
    //Serial.print(" FORWARD ");
    error = enc2Count - enc1Count;
    correction = basePwm + Kp * error;
    masterPwm = basePwm;
    correction = constrain(correction,0,255);
  }
  else {
    //Serial.print(" BACKWARD ");
    error = enc2Count - enc1Count;
    correction = -basePwm + Kp * error;
    masterPwm = -basePwm;
    correction = constrain(correction,-255,0);
  }
  if(abs(enc1Count) > countToMove) {
    correction = 0;
  }
  if(abs(enc2Count) > countToMove) {
    masterPwm = 0;
  }
  
  moveAllMot(0,correction,masterPwm,0);
  Serial.print(" Error : ");
  Serial.print(error);
  Serial.print(" Correction : ");
  Serial.print(correction);
  Serial.print(" MasterPWM : ");
  Serial.print(masterPwm);
}

void leg03Sync(bool dir,long countToMove) {
  auto enc0Count = enc0.read() * 41;
  auto enc3Count = enc3.read();
  long error,correction,masterPwm;
  if (dir) {
    //Serial.print(" FORWARD ");
    error = enc3Count - enc0Count;
    correction = basePwm + Kp * error;
    masterPwm = basePwm;
    correction = constrain(correction,0,255);
  }
  else {
    //Serial.print(" BACKWARD ");
    error = enc3Count - enc0Count;
    correction = -basePwm + Kp * error;
    masterPwm = -basePwm;
    correction = constrain(correction,-255,0);
  }
  if(abs(enc3Count) > countToMove) {
    masterPwm = 0;
  }
  if(abs(enc0Count) > countToMove) {
    correction = 0;
  }
  
  moveAllMot(correction,0,0,masterPwm);
  Serial.print(" Error : ");
  Serial.print(error);
  Serial.print(" Correction : ");
  Serial.print(correction);
  Serial.print(" MasterPWM : ");
  Serial.print(masterPwm);
}

void motTest() {
  Serial.println("-------All motors and encoders test-------");
  Serial.println("Moving Motors Forward");

  moveAllMot(80, 80, 80, 80);
  delay(200);

  Serial.print(enc0.read());
  Serial.print(" ");
  Serial.print(enc1.read());
  Serial.print(" ");
  Serial.print(enc2.read());
  Serial.print(" ");
  Serial.print(enc3.read());
  Serial.println();

  moveAllMot(-80, -80, -80, -80);
  delay(200);

  Serial.println("Moving Motors Backward");
  Serial.print(enc0.read());
  Serial.print(" ");
  Serial.print(enc1.read());
  Serial.print(" ");
  Serial.print(enc2.read());
  Serial.print(" ");
  Serial.print(enc3.read());
  Serial.println();

  stopAllMot();
}
void moveSingleMotBack(byte motNum,int delayms) {
  if(motNum == 1 || motNum == 2) 
    moveMot(motNum, -80);
  if(motNum == 0 || motNum == 3) 
    moveMot(motNum, 80);  
  delay(delayms);
  moveMot(motNum,0);
}

void moveSingleMotForward(byte motNum,int delayms) {
  if(motNum == 1 || motNum == 2) 
    moveMot(motNum, 80);
  if(motNum == 0 || motNum == 3) 
    moveMot(motNum, -80);  
  delay(delayms);
  moveMot(motNum,0);
}
void singleMotTest(byte motNum) {
  Serial.println("-------Single motor and encoder test-------");
  Serial.println("Moving Motor Forward");
  if(motNum == 1 || motNum == 2) 
    moveMot(motNum, 80);
  if(motNum == 0 || motNum == 3) 
    moveMot(motNum, -80);  
  delay(200);

  switch (motNum) {
    case 0: Serial.println(enc0.read());
      break;
    case 1: Serial.println(enc1.read());
      break;
    case 2: Serial.println(enc2.read());
      break;
    case 3: Serial.println(enc3.read());
      break;
  }
  if(motNum == 1 || motNum == 2) 
    moveMot(motNum, -80);
  if(motNum == 0 || motNum == 3) 
    moveMot(motNum, 80);
  delay(200);

  switch (motNum) {
    case 0: Serial.println(enc0.read());
      break;
    case 1: Serial.println(enc1.read());
      break;
    case 2: Serial.println(enc2.read());
      break;
    case 3: Serial.println(enc3.read());
      break;
  }

  moveMot(motNum, 0);
}

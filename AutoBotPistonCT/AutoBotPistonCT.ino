#include <Encoder.h>
#include "pins.h"
#include "Functions.h"

void setup() {
  //initialize serial communication with pc
  Serial.begin(9600);

  // Initialize motor pins as output
  motorsInit();

  //set reset button as interrupt
  pinMode(TEENSY_RESET_PIN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(TEENSY_RESET_PIN), resetTeensy, RISING);

  // Set encoder values to 0
  enc0.write(0);
  enc1.write(0);
  enc2.write(0);
  enc3.write(0);

  delay(1000);
  //    while(!Serial.available());
  //    if(Serial.available()) {
  //      selectPair = Serial.parseInt();
  //      syncMovDir = Serial.parseInt();
  //      moveCount = Serial.parseInt();
  //      Kp = Serial.parseFloat();
  //      basePwm = Serial.parseInt();
  //    }
  //motTest(); // test motors... all of them must go forward for short duration then move approx same distance in opposite direction.
  //singleMotTest(0);
  //singleMotTest(1);
  //singleMotTest(2);
  //singleMotTest(3);
  //moveSingleMotBack(3,200);
  //moveSingleMotForward(3,200);
}

// Interrupt Routine to reset Teensy.
void resetTeensy() {
  Serial.println("RESET");
  noInterrupts();
  Serial.end();
  SCB_AIRCR = 0x05FA0004;

}
int temp = 0;
int mot = 0;
void loop() {
  //aloop();
  //bloop();
  //moveAllLegs();
  
}
bool leg12SyncStat = true, leg03SyncStat = true, legsDir = false;
void moveAllLegs() {
  long enc0Count = enc0.read();
  long enc1Count = enc1.read();
  long enc2Count = enc2.read();
  long enc3Count = enc3.read();
  Serial.print(" Enc0Count : ");
  Serial.print(enc0Count);
  Serial.print(" Enc1Count : ");
  Serial.print(enc1Count);
  Serial.print(" Enc2Count : ");
  Serial.print(enc2Count);
  Serial.print(" Enc3Count : ");
  Serial.print(enc3Count);
  Serial.print(" leg12SyncStat : ");
  Serial.print(leg12SyncStat);
  Serial.print(" leg03SyncStat : ");
  Serial.print(leg03SyncStat);
  Serial.print(" legsDir : ");
  Serial.print(legsDir);
  if (leg12SyncStat) {
    leg12Sync(legsDir, basePwm, moveCount, &leg12SyncStat);
  }
  if (leg03SyncStat) {
    leg03Sync(!legsDir, basePwm, moveCount, &leg03SyncStat);
  }
  if (!leg03SyncStat && !leg12SyncStat) {
    stopAllMot();
    legsDir = !legsDir;
    leg03SyncStat = true;
    leg12SyncStat = true;
  }
  Serial.println();
}

// singleLegMoveLoop
void bloop() {
  if (Serial.available()) {
    temp = Serial.parseInt();

  }
  if (temp == -99) {
    Serial.println("Reseting...");
    delay(1000);
    resetTeensy();
  }
  else if (temp == 1) {
    motStopped = false;
    mot = Serial.parseInt();
    temp = Serial.parseInt();
  }
  else if (!motStopped) {
    Serial.print(" Mot : ");
    Serial.print(mot);
    Serial.print(" Temp : ");
    Serial.print(temp);
    Serial.println();
  }
  moveSingleMot(mot, temp);
}
// diagonalLegSyncMoveLoop
void aloop() {
  long enc0Count = enc0.read();
  long enc1Count = enc1.read();
  long enc2Count = enc2.read();
  long enc3Count = enc3.read();
  Serial.print(" Enc0Count : ");
  Serial.print(enc0Count);
  Serial.print(" Enc1Count : ");
  Serial.print(enc1Count);
  Serial.print(" Enc2Count : ");
  Serial.print(enc2Count);
  Serial.print(" Enc3Count : ");
  Serial.print(enc3Count);
  Serial.print(" syncMovDir : ");
  Serial.print(syncMovDir);
  Serial.print(" motStopped : ");
  Serial.print(motStopped);
  Serial.print(" selectPair : ");
  Serial.print(selectPair);
  Serial.print(" Kp : ");
  Serial.print(Kp);
  if (motStopped) {
    while (!Serial.available());
  }
  if (Serial.available()) {
    selectPair = Serial.parseInt();
    if (selectPair == -1) {
      resetTeensy();
    }
    syncMovDir = Serial.parseInt();
    moveCount = Serial.parseInt();
    Kp = Serial.parseFloat();
    basePwm = Serial.parseInt();
    motStopped = false;
  }
  if (selectPair == 1) {
    if (abs(enc1Count) < moveCount && abs(enc2Count) < moveCount && !motStopped) {
      leg12Sync(syncMovDir, basePwm, moveCount, &motStopped);
    }
    else {
      stopAllMot();
      motStopped = true;
    }
  }
  if (selectPair == 2) {
    if (abs(enc0Count) < moveCount && abs(enc3Count) < moveCount && !motStopped) {
      leg03Sync(syncMovDir, basePwm, moveCount, &motStopped);
    }
    else {
      stopAllMot();
      motStopped = true;
    }
  }
  Serial.println();
}

#include"pins.h"

void pinsInit() {
  pinMode(IR1Pin, INPUT);
  pinMode(IR2Pin, INPUT);
  pinMode(HallSensorPin, INPUT);
  pinMode(SlotSensor, INPUT);
  pinMode(shootFlash,OUTPUT);
  pinMode(tz1Tz2RackIr,INPUT);
  pinMode(tz3RackIr,INPUT);
  for (int i = 0 ; i < 4; i++) {
    pinMode(irArray[i], INPUT);
  }

  pinMode(TRANSFER_MOTOR_DIR, OUTPUT);
  pinMode(TRANSFER_MOTOR_PWM, OUTPUT);
  pinMode(LINEAR_ACTUATOR_DIR1, OUTPUT);
  pinMode(LINEAR_ACTUATOR_DIR2, OUTPUT);
  pinMode(FORWARD_PISTON, OUTPUT);
  pinMode(BACKWARD_PISTON, OUTPUT);
  pinMode(ROTATING_PISTON, OUTPUT);
  pinMode(EXTENDING_PISTON, OUTPUT);

  pinMode(MOT0_directionPin, OUTPUT);
  pinMode(MOT0_pwmPin, OUTPUT);
  pinMode(MOT1_directionPin, OUTPUT);
  pinMode(MOT1_pwmPin, OUTPUT);
  pinMode(MOT2_directionPin, OUTPUT);
  pinMode(MOT2_pwmPin, OUTPUT);
  digitalWrite(MOT0_directionPin, MOT0_CW);
  digitalWrite(MOT0_pwmPin, LOW);
  digitalWrite(MOT1_directionPin, MOT1_ACW);
  digitalWrite(MOT1_pwmPin, LOW);
  digitalWrite(MOT2_directionPin, MOT2_CW);
  digitalWrite(MOT2_pwmPin, LOW);
  digitalWrite(FORWARD_PISTON, LOW);
  digitalWrite(BACKWARD_PISTON, LOW);
}
void resetEncoders() {
  encoder0.write(0);
  encoder1.write(0);
  encoder2.write(0);
}
void stopAllLoops() {
  //TODO stop all loops
  goToTz1Junction = false;
  readLine = false;
  Junction = false;
  rotateccw = false;
  goToTransferLocation = false;
  moveToThrowingJunction = false;
  moveSideways = false;
  junctionCount = 0;
  checkJunction = false;
}
void StopDrives() {
  stopAllLoops();
  digitalWrite(MOT0_directionPin, MOT0_CW);
  digitalWrite(MOT2_directionPin, MOT2_CW);
  digitalWrite(MOT1_directionPin, MOT1_ACW);
  analogWrite(MOT0_pwmPin, 0);
  analogWrite(MOT1_pwmPin, 0);
  analogWrite(MOT2_pwmPin, 0);
}

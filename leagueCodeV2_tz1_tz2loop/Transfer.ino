void initialize_transfer_pistons()    {
  digitalWrite(EXTENDING_PISTON, !EXTENDING_PISTON_EXTEND);
  digitalWrite(ROTATING_PISTON,  !ROTATING_PISTON_EXTEND);
}

void extend_piston_extending()  {
  digitalWrite(EXTENDING_PISTON, EXTENDING_PISTON_EXTEND);
}
void retract_piston_extending()  {
  digitalWrite(EXTENDING_PISTON, !EXTENDING_PISTON_EXTEND);
}
void toggle_piston_extending()  {
  extending_piston_state = !extending_piston_state;
  ////Serial.println('Extend');
  digitalWrite(EXTENDING_PISTON, extending_piston_state);
}


void closeGripper()  {
  digitalWrite(ROTATING_PISTON, ROTATING_PISTON_EXTEND);
}
void openGripper()  {
  digitalWrite(ROTATING_PISTON, !ROTATING_PISTON_EXTEND);
}
void toggleGripper()  {
  ////Serial.println('rotate');

  rotating_piston_state = !rotating_piston_state;
  digitalWrite(ROTATING_PISTON, rotating_piston_state);
}

void gripper_motor_cw() {
  digitalWrite(TRANSFER_MOTOR_DIR, TRANSFER_MOTOR_CW);
  analogWrite(TRANSFER_MOTOR_PWM, 255);
}

void gripper_motor_acw() {
  digitalWrite(TRANSFER_MOTOR_DIR, TRANSFER_MOTOR_ACW);
  analogWrite(TRANSFER_MOTOR_PWM, 255);
}

void gripper_motor_stop() {
  analogWrite(TRANSFER_MOTOR_PWM, 0);
}
void transferInitBoolSet() {
  motorSet = true;
  motorOnPos = false;
  openingGripper = false;
  gripperOpened = false;
  platformExtended = false;
  transferInitComplete = false;
}
void transferInit() {
  hallSensorStatus = digitalRead(HallSensorPin);
//  //Serial.println(hallSensorStatus);
  if (!hallSensorStatus) {
    motorOnPos = true;
  }
  if (hallSensorStatus && !motorOnPos) {
    if (motorSet) {
      gripper_motor_cw();
      motorSet = false;
    }
  }
  if (!openingGripper && motorOnPos) {
    gripper_motor_stop();
    openingGripper = true;
    openGripperTimer = micros();
  }
  if (openingGripper && !gripperOpened) {
    if (micros() - openGripperTimer < 500000) {
      openGripper();
    }
    else {
      gripperOpened = true;
      extendPlatformTimer = micros();
    }
  }
  if (gripperOpened && !platformExtended) {
    if (micros() - extendPlatformTimer < 500000) {
      extend_piston_extending();
    }
    else {
      platformExtended = true;
    }
  }
  if (platformExtended) {
    transferStart = false;
    transferInitComplete = true;
  }
}
void transferCompBoolSet() {
  motorSet = true;
  pistonPartDone = false;
  pistonPartDone2 = false;
  motorToIr2 = false;
  gripperClosed = false;
  gripperOpened = false;
  platformRetracted = false;
  openingGripper = false;
  transferCompCalledFirstTime = true;
  readIr = false;
}
void transferComp() {
  if (readIr) {
    Ir1Status = digitalRead(IR1Pin);
    Ir2Status = digitalRead(IR2Pin);
  }
  if (!pistonPartDone2) {
    if (readIr) {
      if (Ir1Status) {
        motorToIr2 = false;
      }
      if (!Ir2Status) {

        motorToIr2 = true;
      }
    }
    if (!pistonPartDone) {
      if (transferCompCalledFirstTime) {
        transferCompCalledFirstTime = false;
        closeGripperTimer = micros();
      }
      if (!gripperClosed) {
        if (micros() - closeGripperTimer < 500000) {
          closeGripper();
        }
        else {
          gripperClosed = true;
          //////Serial.println("GripperClosed");
          retractPlatformTimer = micros();
        }
      }
      if (gripperClosed && !platformRetracted) {
        if (micros() - retractPlatformTimer < 500000) {
          retract_piston_extending();
        }
        else {
          //////Serial.println("PlatformRetracted");
          platformRetracted = true;
          platformRetractDelay = micros();
        }
      }
      if (platformRetracted) {
        if (micros() - platformRetractDelay > 500000) {
          pistonPartDone = true;
          readIr = true;
          transferMotorTimer = micros();
          return;
        }
      }
    }
    if (!motorToIr2 && (micros() - transferMotorTimer < 400000) && pistonPartDone) {
      //////Serial.println("Move Motor Back");
      gripper_motor_cw();
      gripperOpenDelay = micros();
      return;
    }
    else if (!motorToIr2 && (micros() - transferMotorTimer > 400000) && pistonPartDone) {
      motorToIr2 = true;
      //Serial.println("B");
      gripperOpenDelay = micros();
    }
    if (motorToIr2 && !openingGripper) {
      //////Serial.println("motorReachedBack");
      gripper_motor_stop();
      //Serial.println(micros() ); //Serial.print("\t");
      //Serial.println(gripperOpenDelay  ); //Serial.print("\t");
      //Serial.println(micros() - gripperOpenDelay );
      if (micros() - gripperOpenDelay < 1000000) {
      }
      else {
        openingGripper = true;
        detectShuttleCock = true;
        SlotSensorTimeOut = micros();
        openGripperTimer = micros();
      }
    }
    if (openingGripper && !gripperOpened) {
      if (micros() - openGripperTimer < 1000000) {
        openGripper();
      }
      else {
        //////Serial.println("GripperOpened");
        gripperOpened = true;
      }
    }
    if (gripperOpened) {
      pistonPartDone2 = true;
      transferMotorOutTimeOut = micros();
    }
  }
  else {
    if (pistonPartDone) {
      Ir1Status = digitalRead(IR1Pin);
      Serial.print(" IR1 Status : ");
      Serial.println(Ir1Status);
      if (Ir1Status) {
//        Serial.println(micros() - transferMotorOutTimeOut );
        if (micros() - transferMotorOutTimeOut < 1600000) {
          if (motorSet) {
            gripper_motor_acw();
            motorSet = false;
          }
        }
        else {
          Ir1Status = false;
          readIr = false;
          return;
        }
      } else {
        gripper_motor_stop();
        transferEnd = false;
        transferComplete = true;
      }
    }
  }
}


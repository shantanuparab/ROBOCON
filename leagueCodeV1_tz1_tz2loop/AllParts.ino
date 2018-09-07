void Tz1() {
  if (goToTz1Junction) {
        Serial.println("Started going to TZ1");
    location = 1;
    if (!changeAngle && !angleJustChanged) {
      //      Serial.println("Started Changing Angle in TZ1");
      changeAngle = true;
      mega1.setMode(1);
      mega1.setGlaAngle(1);
      angleJustChanged = true;
    }
    Serial.print(" \t Enc0 : ");
    Serial.print(encoder0Reading);
    Serial.print(" \t Enc1 : ");
    Serial.print(encoder1Reading);
    Serial.print(" \t Enc2 : ");
    Serial.print(encoder2Reading);
    // calculated 32150
    moveStraightAhead(straightSpeed);
    //linefollow();
    if (abs(encoder2Reading) > 35000 || abs(encoder1Reading) > 35000) {
      Serial.println("Failed in Junction Detection, Stop");
      //        goToTz1Junction = false;
      //        reachedTz1Junction = true;
      //        haltTimer = micros();
      StopDrives();
    }
    if ((abs(encoder2Reading) > 28150 || abs(encoder1Reading) > 28150)) {
      //Serial.println("ReadLine = true");

      readLine = true;
      checkJunction = true;
    }
    if (checkJunction && junctionCount == 1) {
      // reached junction
      Serial.println("Found TZ1 Junction, Stop Drives");
      goToTz1Junction = false;
      reachedTz1Junction = true;
      haltTimer = micros();
    }
  }
  if (reachedTz1Junction) {
    Serial.println("reachedTz1Junction");
    StopDrives();
    if (micros() - haltTimer > pause) {
      rotateFunctionCalledFirstTime = true;
      reachedTz1Junction = false;
    }
  }
  if (rotateFunctionCalledFirstTime) {
        Serial.println("Rotating to align in TZ1 Junction");
    rotateFunctionCalledFirstTime = false;
    resetEncoders();
    rotateccw = true;
    readLine = false;
    timer = micros();
  }
  if (rotateccw) {
    if (!readLine) {
      rotateCcw(4911);
      if ((abs(encoder0Reading) > 4891 && abs(encoder0Reading) < 4931) || (abs(encoder1Reading) > 4891 && abs(encoder1Reading) < 4931) || (abs(encoder2Reading) > 4891  && abs(encoder2Reading) < 4931)) {
        StopDrives();
        rotateccw = true;
        readLine = true;
        resetEncoders();
        rotateDirChangeTime = micros();
        return;
      }
    }
    else if (readLine) {
      if (sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 0 && sensor[4] == 0 && sensor[5] == 0 && sensor[6] == 0 && sensor[7] == 0) {
        readLine = true;
        rotateccw = true;
        if (micros() - rotateDirChangeTime < 1000000) {
          digitalWrite(MOT0_directionPin, MOT0_CW);
          digitalWrite(MOT1_directionPin, MOT1_ACW);
          digitalWrite(MOT2_directionPin, MOT2_ACW);
          analogWrite(MOT0_pwmPin, 90);
          analogWrite(MOT1_pwmPin, 45);
          analogWrite(MOT2_pwmPin, 45);
        }
        else {
          digitalWrite(MOT0_directionPin, MOT0_ACW);
          digitalWrite(MOT1_directionPin, MOT1_CW);
          digitalWrite(MOT2_directionPin, MOT2_CW);
          analogWrite(MOT0_pwmPin, 90);
          analogWrite(MOT1_pwmPin, 45);
          analogWrite(MOT2_pwmPin, 45);
        }
        return;
      }
      else if (sensor[3] == 1 || sensor[4] == 1) {
        Serial.println("Rotation Complete");
        straight.resetPID();
        StopDrives();
        resetEncoders();
        if (location == 1) {
          readLine = true;
          checkJunction = true;
          resetEncoders();
          goToTransferLocation = true;
          return;
        }
        else if ((location == 3)) {
          readyToThrow = true;
          forwardDelay = micros();
          haltBeforeThrowing = micros();
          throwTime = 280;
          //readLine = true;
          //readLine1 = true;
          //adjustForShoot = true;
        }
        else if (location == 4) {
          readLine = true;
          checkJunction = true;
          goBackToTransferJunction = true;
          angleJustChanged = false;
        }
        else if (location == 5) {
          transferInitBoolSet();
          Ir2Status = digitalRead(IR2Pin);
          transferStart = true;
          readLine = false;
        }
        else if (location == 6) {
          //          straight.resetPID();
          //          resetEncoders();
          readLine = true;
          checkJunction = true;
          junctionCount = 0;
          moveToThrowingJunction = true;
          angleJustChanged = false;
        }
        else if (location == 7) {
          readyToThrow = true;
          forwardDelay = micros();
          haltBeforeThrowing = micros();
          throwTime = 340;
        }
        else if (location == 8) {
          goBackToTransferJunction = true;
          angleJustChanged = false;
          readLine = true;
          checkJunction = true;
        }
        else if (location == 9) {
          transferInitBoolSet();
          Ir2Status = digitalRead(IR2Pin);
          transferStart = true;
          readLine = false;
        }
        else if (location == 10) {
          straight.resetPID();
          resetEncoders();
          readLine = true;
          checkJunction = true;
          junctionCount = 0;
          moveToThrowingJunction = true;
          angleJustChanged = false;
        }
        else if (location == 11) {
          //          readyToThrow = true;
          //          forwardDelay = micros();
          //          haltBeforeThrowing = micros();
          //          throwTime = 220000;
          xAlign = false;
          Vertical.setUartMode(0x02);
          Horizontal.setUartMode(0x02);
          adjustForShoot = true;
          alignmentTimeOut = micros();
        }
        timer = micros();
        analogWrite(MOT0_pwmPin, 0);
        return;
      }
      else if ((sensor[0] == 1 || sensor[1] == 1 || sensor[2] == 1)) {
        readLine = true;
        rotateccw = true;
        digitalWrite(MOT0_directionPin, MOT0_CW);
        analogWrite(MOT0_pwmPin, 60);
        //////Serial.println("Clockwise Chutiya");
        return;
      }
      else if ((sensor[5] == 1 || sensor[6] == 1 || sensor[7] == 1)) {
        readLine = true;
        rotateccw = true;
        digitalWrite(MOT0_directionPin, MOT0_ACW);
        analogWrite(MOT0_pwmPin, 60);
        //////Serial.println("Counter Clockwise Chutiya");
        return;
      }
    }
  }
  if (goToTransferLocation) {
    Serial.println("Moving to TransferJunction");
    location = 2;
    //7800
    straight.setOutMax(150);
    straight.setOutMin(50);
    moveStraightAhead(100);
    linefollow();
    //      if(checkJunction && Junction) {
    //        resetEncoders();
    //        analogWrite(MOT0_pwmPin,0);
    //        junctionWasDetectedAfterRotation = true;
    //        readLine = true;
    //        checkJunction = false;
    //        return;
    //      }
    if ((abs(encoder1Reading) > 5030 || abs(encoder2Reading) > 5030)) {
      StopDrives();
      transferInitBoolSet();
      Ir2Status = digitalRead(IR2Pin);
      transferStart = true;
      readLine = false;

    }
  }
  if (transferStart && !changeAngle) {
    Serial.println("transferStart");
    transferInit();
  }
  if (transferInitComplete) {
    Serial.println("transferInitComplete");
    checkIrArray = true;
    transferInitComplete = false;
  }
  if (checkIrArray) {
    Serial.println("checkIrArray");
    int irArStat[4];
    for (int i = 0; i < 4 ; i++)
    {
//      Serial.print(" Sensor [");
//      Serial.print(i);
//      Serial.print("] : ");
      irArStat[i] = !digitalRead(irArray[i]);
//      Serial.print(irArStat[i]);
    }
//    Serial.println();
    //    if (Serial.available()) {
    //      char a = Serial.read();
    //      if (a == 'c' || a == 'C') {
    //        ////Serial.println("transferEnd = true");
    //        checkIrArray = false;
    //        transferCompBoolSet();
    //        transferEnd = true;
    //        //detectShuttleCock = true;
    //        closeGripperTimer = micros();
    //      }
    //    }
    if (irArStat[1] == 1 || irArStat[2] == 1 || irArStat[0] == 1 || irArStat[3] == 1) {
      checkIrArray = false;
      transferCompBoolSet();
      transferEnd = true;
      //detectShuttleCock = true;
      closeGripperTimer = micros();
    }
  }
  if (transferEnd) {
    transferComp();
  }
  if (transferComplete) {
    //detectShuttleCock = true;
    //        transferInitBoolSet();
    //        Ir2Status = digitalRead(IR2Pin);
    //        transferStart = true;
    transferComplete = false;
  }
  if (detectShuttleCock) {
    Serial.println("detectShuttleCock");
    shuttleCockInSlot = digitalRead(SlotSensor);
    if (micros() - SlotSensorTimeOut < 3000000) {
      if (shuttleCockInSlot) {
        detectShuttleCock = false;
        shuttleCockDetected = true;
        shuttleCockHalt = micros();
      }
    }
    else {
      transferInitBoolSet();
      Ir2Status = digitalRead(IR2Pin);
      transferStart = true;
      detectShuttleCock = false;
    }
  }
  if (shuttleCockDetected) {
    Serial.println("shuttleCockDetected");
    
    if (micros() - shuttleCockHalt > 1000000) {
      if (location == 5) {
        moveToTz2Junction = true;
      }
      else if (location == 9) {
        moveLittleAheadDesiJugad = true;
      }
      else if (location == 2 || location == 6 || location == 10) {
        moveToThrowingJunction = true;
        angleJustChanged = false;

      }
      junctionCount = 0;
      readLine = true;
      checkJunction = true;
      shuttleCockDetected = false;
      straight.resetPID();
      resetEncoders();
      timer = micros();
    }
  }
  if (moveToThrowingJunction) {
    Serial.println("========================Moving To Tz Zone");
    if (location == 2) {
      x1 = 0;
      location = 3;
      if (!changeAngle && !angleJustChanged) {
        changeAngle = true;
        mega1.setMode(1);
        angleJustChanged = true;
        mega1.setGlaAngle(2);
      }
    }
    else if (location == 6) {
      location = 7;
      if (!changeAngle && !angleJustChanged) {
        changeAngle = true;
        mega1.setMode(1);
        angleJustChanged = true;
        mega1.setGlaAngle(2);
      }
    }
    else if (location == 10) {
      location = 11;
      if (!changeAngle && !angleJustChanged) {
        changeAngle = true;
        mega1.setMode(1);
        angleJustChanged = true;
        mega1.setGlaAngle(3);
      }
    }
    straight.setOutMax(255);
    straight.setOutMin(150);
    moveStraightAhead(230);
    linefollow();
    readLine = true;
    checkJunction = true;

    //14000 - 3900 20000 - 3900

    //      if(abs(encoder2Reading) > 15100 || abs(encoder1Reading) > 15100) {
    //        moveToThrowingJunction = false;
    //        //reachedThrowingJunction = true;
    //      }
    //      else if(abs(encoder2Reading) > 10100 || abs(encoder1Reading) > 10100) {
    //        checkJunction = true;
    //      }
    if (location == 11) {
      if (checkJunction && junctionCount == 5) {
        moveToThrowingJunction = false;
        reachedThrowingJunction = true;
        haltTimer = micros();
      }
    }
    else {
      if (checkJunction && junctionCount == 2) {
        moveToThrowingJunction = false;
        reachedThrowingJunction = true;
        haltTimer = micros();
      }
    }
  }
  if (reachedThrowingJunction) {
    Serial.println("reachedThrowingJunction");
    StopDrives();
    if (micros() - haltTimer > pause) {
      rotateFunctionCalledFirstTime = true;
      reachedThrowingJunction = false;
    }
  }
}

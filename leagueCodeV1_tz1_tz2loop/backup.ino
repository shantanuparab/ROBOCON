//void loop1() {
//  if(readHallSensorForFirstTime) {
//    MagnetPresent = !digitalRead(HallSensorPin);          //negation of the reading because the sensor gives 0 when magnet is present
//  }
//  if (readLine) {
//    readLineSensor();
//    if (LedCount >= 4) {
//      Junction = true;
//    } else {
//      Junction = false;
//    }
//  }
//  if (MagnetPresent && readHallSensorForFirstTime) {    // HallSensor will be removed in the end
//    readHallSensorForFirstTime = false;                 //read Hallsensor only in beginning
//    resetEncoders();                                    //reset encoders, just in case bot was moved before hall sensor detects magnet
//    goToTz1Junction = true;
//    
//    
//    timer = micros();
//    safetyTimer = micros();
//    readLine = false;
//    Junction = false;
//  }
//  if (goToTz1Junction) {
//    location = 1;
//    if (micros() - safetyTimer < 7000000) {
//      moveStraightAhead(230);
//      if (abs(encoder2Reading) > 32150 || abs(encoder1Reading) > 32150) {
//        goToTz1Junction = false;
//        reachedTz1Junction = true;
//        haltTimer = micros();
//      }
//      else if ((abs(encoder2Reading) > 28150 || abs(encoder1Reading) > 28150) && !readLine) {
//        //readLine = true;
//        delayCount = micros();
//      }
//    }
//    else {
//      StopDrives();
//    }
//    if (readLine && Junction) {
//      // reached junction
//      if(micros() - delayCount > delayStop) {
//        goToTz1Junction = false;
//        reachedTz1Junction = true;
//        haltTimer = micros();
//      }
//    }
//  }
//  if (reachedTz1Junction) {
//    StopDrives();
//    if (micros() - haltTimer > pause) {
//      rotateFunctionCalledFirstTime = true;
//      reachedTz1Junction = false;
//    }
//  }
//  if (rotateFunctionCalledFirstTime) {
//    
//    rotateFunctionCalledFirstTime = false;
//    resetEncoders();
//    rotateccw = true;
//    safetyTimer = micros();
//    timer = micros();
//  }
//  if (rotateccw) {
//    if (micros() - safetyTimer < 3000000) {
//      if (!readLine) {
//        rotateCcw(5021);
//      }
//      if (((abs(encoder0Reading) > 4842 && abs(encoder0Reading) < 5200) || (abs(encoder1Reading) > 4842 && abs(encoder1Reading) < 5200) || (abs(encoder2Reading) > 4842 && abs(encoder2Reading) < 5200)) && !readLine) {
//        StopDrives();
//        rotateccw = true;
//        readLine = true;
//      }
//      if (readLine) {
//        if(1) {
//          ////Serial.println("Rukh Bhosdike");
//          straight.resetPID();
//          StopDrives();
//          resetEncoders();
//          if(location == 1) {
//            goToTransferLocation = true;
//          }
//          timer = micros();
//          safetyTimer = micros();
//        }
//        if ((sensor[0] == 1 || sensor[1] == 1 || sensor[2] == 1) && (sensor[5] == 0 && sensor[6] == 0 && sensor[7] == 0)) {
//          digitalWrite(MOT0_directionPin, MOT0_CW);
//          analogWrite(MOT0_pwmPin, 60);
//          ////Serial.println("Clockwise Chutiya");
//        }
//        else if ((sensor[0] == 0 && sensor[1] == 0 && sensor[2] == 0) && (sensor[5] == 1 || sensor[6] == 1 || sensor[7] == 1)) {
//          digitalWrite(MOT0_directionPin, MOT0_ACW);
//          analogWrite(MOT0_pwmPin, 60);
//          ////Serial.println("Counter Clockwise Chutiya");
//        }
//      }
//    }
//    else {
//      straight.resetPID();
//      StopDrives();
//      resetEncoders();
//      if(location == 1){
//        goToTransferLocation = true;
//      }
//      timer = micros();
//      safetyTimer = micros();
//    }
//  }
//  if(goToTransferLocation) {
//    location = 2;
//    //7800
//    if(micros() - safetyTimer < 5000000) {
//      straight.setOutMax(150);
//      straight.setOutMin(50);
//      moveStraightAhead(100);
//      if(abs(encoder2Reading) > 7800 || abs(encoder1Reading) > 7800) {
//        StopDrives();
//        detectShuttleCock = true;
//      }
//    }
//  }
//  if(detectShuttleCock) {
//    shuttleCockInSlot = !digitalRead(IR2Pin);
//    if(shuttleCockInSlot) {
//      detectShuttleCock = false;
//      shuttleCockDetected = true;
//      shuttleCockHalt = micros();
//    }
//  }
//  if(shuttleCockDetected) {
//    if(micros() - shuttleCockHalt > 1000000) {
//      moveToThrowingJunction = true;
//      
//      shuttleCockDetected = false;
//      straight.resetPID();
//      resetEncoders();
//      timer = micros();
//      safetyTimer = micros();
//    }
//  }
//  if(moveToThrowingJunction) {
//    location = 3;
//    //14000 - 3900 20000 - 3900
//    if(micros() - safetyTimer < 7000000) {
//      straight.setOutMax(200);
//      straight.setOutMin(100);
//      moveStraightAhead(150);
//      if(abs(encoder2Reading) > 15100 || abs(encoder1Reading) > 15100) {
//        moveToThrowingJunction = false;
//        reachedThrowingJunction = true;
//      }
//      else if(abs(encoder2Reading) > 10100 || abs(encoder1Reading) > 10100) {
//        //readLine = true;
//      }
//    }
//    else {
//      StopDrives();
//    }
//    if(readLine && Junction) {
//      moveToThrowingJunction = false;
//      reachedThrowingJunction = true;
//      haltTimer = micros();
//    }
//  }
//  if (reachedThrowingJunction) {
//    StopDrives();
//    if (micros() - haltTimer > pause) {
//      rotateFunctionCalledFirstTime = true;
//      reachedThrowingJunction = false;
//    }
//  }
//}

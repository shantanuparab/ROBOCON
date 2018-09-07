#include"Methods.h"

/////////////////////////////////////////////
///////////////PID_Related///////////////////
PIDcalc rotateWheel0(0, 510 / 8800.0 * 0.7);
PIDcalc rotateWheel1(0, 510 / 8800.0 * 0.7);
PIDcalc rotateWheel2(0, 510 / 8800.0 * 0.7);
//5,0.5,0.2
//4.5,1,0.2 for straight
//4.5,1.2,0.2
PIDcalc straight(1, 4.5, 1.2, 0.2);
PIDcalc sideways(1, 4.5, 0);
PIDcalc sideways1(1, 4.5, 0);
/////////////////////////////////////////////
//TODO: Ramp the motors;

int straightSpeed = 230;
int lineCounter = 0;
int pidCounter = 0;
void setup() {
  Serial.begin(230400);
  pinsInit();
  rotateWheel0.setOutMax(150);
  rotateWheel1.setOutMax(150);
  rotateWheel2.setOutMax(150);
  rotateWheel0.setOutMin(-150);
  rotateWheel1.setOutMin(-150);
  rotateWheel2.setOutMin(-150);
  rotateWheel0.setSetPoint(5021);
  rotateWheel1.setSetPoint(5021);
  rotateWheel2.setSetPoint(5021);
  rotateWheel0.setBias(30);
  rotateWheel1.setBias(30);
  rotateWheel2.setBias(30);
  readLine = false;
  //  gyroInit();
  straight.setOutMax(255);
  straight.setOutMin(150);
  sideways.setOutMax(255);
  sideways.setOutMin(50);
  sideways1.setOutMax(255);
  sideways1.setOutMin(50);
  Horizontal.initialize();
  Vertical.initialize();
  Vertical.setUartMode(0x01);
  Horizontal.setUartMode(0x01);
  changeAngle = false;
  //  transferInitBoolSet();
  //  Ir2Status = !digitalRead(IR2Pin);
  //  transferStart = true;

}

long delayStop = 00000;
long delayCount = 0;
void loop() {
  Serial.print("\t Location : ");
  Serial.print(location);
  Serial.print("\t");
  //  mega1.requestColor();
  ////Serial.println(hallSensorStatus);
  //  ////Serial.println(digitalRead(SlotSensor));
  //  ////Serial.print("Location : ");
  //  ////Serial.print(location);
  //  ////Serial.print("\tJunction Count : ");
  //  //  ////Serial.println(junctionCount);
  //  getPitch();
  //  ////Serial.println(curAng);
  if (readHallSensorForFirstTime) {
    MagnetPresent = !digitalRead(HallSensorPin);          //negation of the reading because the sensor gives 0 when magnet is present
    //    //Serial.println(MagnetPresent);
  }
  if (readLine) {
    Vertical.setUartMode(0x01);
    Horizontal.setUartMode(0x01);
    Horizontal.readLineSensor(sensor);
    //readLineSensor();
    Junction = Horizontal.readJunction();
    if (Junction && !prevJunction) {
      junctionCount++;
    }
  }

  if (readLine1) {
    Vertical.readLineSensor(sensor1);
  }
  if (changeAngle) {
    angleIsSet = mega1.getGlaStatus();
    if (angleIsSet) {
      changeAngle = false;
//     if (location == 1) {
////         Serial.print("= 20 Angle Is Set =");
//     }
    }

  }
//  Serial.print("ChangeGlaAngle = ");
//  Serial.print(changeAngle);
//  Serial.print(" Angle Is Set : ");
//  Serial.println(angleIsSet);
  if (MagnetPresent && readHallSensorForFirstTime) {    // HallSensor will be removed in the end
    Serial.println("Bot Started");
    readHallSensorForFirstTime = false;                 //read Hallsensor only in beginning
    resetEncoders();                                    //reset encoders, just in case bot was moved before hall sensor detects magnet
    goToTz1Junction = true;
    //checkAngle = true;
    //    readyToThrow = true;
    //    throwTime = 170000;
    //    forwardDelay = micros();
    //    haltBeforeThrowing = micros();
    //            transferInitBoolSet();
    //            Ir2Status = digitalRead(IR2Pin);
    //            transferStart = true;
    //    Vertical.setUartMode(0x02);
    //    Horizontal.setUartMode(0x02);
    //    adjustForShoot = true;
    //    shuttleCockThrowed = true;
    timer = micros();
    readLine = false;
    Junction = false;
  }
  //  ////Serial.println(curAng);
  //  if (changeAngle) {
  //    getPitch();
  //    if (int(curAng) > setAng) {
  //      digitalWrite(LINEAR_ACTUATOR_DIR1, HIGH);
  //      digitalWrite(LINEAR_ACTUATOR_DIR2, LOW);
  //    }
  //    else if (int(curAng) < setAng) {
  //      digitalWrite(LINEAR_ACTUATOR_DIR1, LOW);
  //      digitalWrite(LINEAR_ACTUATOR_DIR2, HIGH);
  //    }
  //    else {
  //      digitalWrite(LINEAR_ACTUATOR_DIR1, LOW);
  //      digitalWrite(LINEAR_ACTUATOR_DIR2, LOW);
  //      changeAngle = false;
  //    }
  //  }
  Tz1();
  //Serial.println(throwTime);
  if (readyToThrow) {
    Serial.println("Ready to throw! ");
    bool checkClearance = !digitalRead(IR1Pin);
    if (checkClearance && changeAngle) {
      haltBeforeThrowing = micros();
    }
    else if (checkClearance && !changeAngle) {
      Serial.println("Clear to throw!");
      if (micros() - haltBeforeThrowing < 5000000) {
        //////Serial.println("Wait for throw!");
        forwardDelay = micros();
        throwStart = false;
        if(micros() - haltBeforeThrowing > 4000000) {
          digitalWrite(shootFlash,HIGH);
        }
      }
      else if (!throwStart) {
        Serial.println(" wait is over!");
        throwStart = true;
        forwardDelay = micros();
      }
      else if (throwStart) {
        Serial.println("ThrowStart = true!");
        
        digitalWrite(FORWARD_PISTON, HIGH);
        delay(throwTime);
        //Serial.println(throwTime);
        digitalWrite(shootFlash,LOW);
        digitalWrite(FORWARD_PISTON, LOW);
        //Serial.println("throw off!");
        shuttleCockThrowed = true;
        readyToThrow = false;
        pullBackCounter = 0;
        backwardDelay = micros();
        haltBeforeThrowing = micros();
      }

    }
  }
  if (shuttleCockThrowed) {
    Serial.println("shuttle throwed");
    if (micros() - haltBeforeThrowing < 1000000) {
      pullStart = false;
    }
    else if (!pullStart) {
      straight.resetPID();
      resetEncoders();
      if (location == 11) {

      }
      else {
        moveLittleAheadDesiJugad = true;
      }
      pullStart = true;
      pullDelayRampSpeed = 90000;
      backwardDelay = micros();
    }
    else if (pullStart) {
      if (pullBackCounter < 15) {
        if (micros() - backwardDelay <  pullDelayRampSpeed) {
          pullDelay = false;
          digitalWrite(BACKWARD_PISTON, HIGH);
        }
        else if (!pullDelay) {
          pullDelay = true;
          pullDelayTimer = micros();
          Serial.println("PullBack");
          digitalWrite(BACKWARD_PISTON, LOW);
        }
        else if (pullDelay ) {
          if (micros() - pullDelayTimer < 300000) {
          }
          else {
            pullBackCounter++;
            pullDelayRampSpeed -= 3000;
            pullDelay = false;
            pullDelayTimer = micros();
            backwardDelay = micros();
          }
        }
      }
      else {
        shuttleCockThrowed = false;
      }
    }
  }
  if (moveLittleAheadDesiJugad) {
    Serial.println("moveLittleAheadDesiJugad");
    if (location == 3) {
      location = 4;
    }
    else if (location == 7) {
      location = 8;
    }
    else if (location == 9) {
      location = 6;
    }
    //14000 - 3900 20000 - 3900
    straight.setOutMax(100);
    straight.setOutMin(5);
    moveStraightAhead(50);
    linefollow();
    if (encoder1Reading > 50 || encoder2Reading > 50) {
      moveLittleAheadDesiJugad = false;
      rotateFunctionCalledFirstTime = true;
    }
  }
  if (goBackToTransferJunction) {
    Serial.println("goBackToTransferJunction");
    if (location == 4) {
      location = 5;
    }
    if (location == 8) {
      location = 9;
    }
    if (!changeAngle && !angleJustChanged) {
      changeAngle = true;
      mega1.setMode(1);
      angleJustChanged = true;
      mega1.setGlaAngle(1);
    }
    //14000 - 3900 20000 - 3900
    straight.setOutMax(200);
    straight.setOutMin(100);
    moveStraightAhead(150);
    linefollow();
    //      if(abs(encoder2Reading) > 15100 || abs(encoder1Reading) > 15100) {
    //        moveToThrowingJunction = false;
    //        //reachedThrowingJunction = true;
    //      }
    //      else if(abs(encoder2Reading) > 10100 || abs(encoder1Reading) > 10100) {
    //        checkJunction = true;
    //      }
    if (checkJunction && junctionCount == 2) {
      goBackToTransferJunction = false;
      rotateFunctionCalledFirstTime = true;
    }
  }
  if (moveToTz2Junction) {
    Serial.println("moveToTz2Junction");
    readLine = true;
    checkJunction = true;
    location = 6;

    straight.setOutMax(200);
    straight.setOutMin(100);
    moveStraightAhead(150);
    linefollow();
    if (checkJunction && junctionCount == 1) {
      moveToTz2Junction = false;
      rotateFunctionCalledFirstTime = true;
    }
  }
  if (adjustForShoot) {
    matchJunction();
  }
  prevJunction = Junction;
}


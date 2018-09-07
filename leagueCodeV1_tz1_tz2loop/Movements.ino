int counter = 0;
byte dummies[5];
int lineReadings[8];
void moveStraightAhead(int setFrontSpeed) {
  // TODO: in the end
  //disableAllOtherLoops
  if (micros() - timer > iterationTime) {
    digitalWrite(MOT1_directionPin, MOT1_ACW);
    digitalWrite(MOT2_directionPin, MOT2_CW);
    encoder0Reading = encoder0.read();
    encoder1Reading = encoder1.read();
    encoder2Reading = encoder2.read();
    //    //////Serial.print(" Encoder0 : ");
    //    //////Serial.print(encoder0Reading);
    //    //////Serial.print(" Encoder1 : ");
    //    //////Serial.print(encoder1Reading);
    //    //////Serial.print(" Encoder2 : ");
    //    //////Serial.print(encoder2Reading);
    //    //////Serial.println();
    straight.setSetPoint(abs(encoder2Reading));
    straight.Calculate(abs(encoder1Reading));
    int motor1pwm = abs(straight.out);
    //    if (motor0pwm > 0) digitalWrite(MOT0_directionPin, MOT0_CW );
    //    if (motor0pwm < 0) digitalWrite(MOT0_directionPin, MOT0_ACW );

    //    analogWrite(MOT0_pwmPin, abs(motor0pwm));
    //    if(encoder1Reading > 20000 || encoder2Reading > 20000) {
    //      if(x1<=x && x > 0) {
    //        x1 -= 1;
    //      }
    //      else {
    //        x1 = 100;
    //      }
    //      if(y1<=y && y > 0) {
    //        y1 -= (1.0 * y) / x;
    //      }
    //      else {
    //        y1 = 100;
    //      }
    //    }
    //    else {
    //      x1 = x;
    //      y1 = y;
    //    }
    if (location == 1) {
      if (encoder1Reading > 20000 || encoder2Reading > 20000) {
        if (counter == 1) {
          x = x + 1;
          y = y + 1;
          counter = 0;
        }
        else {
          counter++;
        }
      }
      else {
        x = 0;
        y = 0;
      }
    }
    if (location == 3 || location == 7) {
      if (encoder1Reading > 3500 || encoder2Reading > 3500) {
        if (counter == 1) {
          x = x + 3;
          y = y + 3;
          counter = 0;
        }
        else {
          counter++;
        }
      }
      else {
        x = 0;
        y = 0;
      }
    }
    if (location == 11) {
      if (encoder1Reading > 25000 || encoder2Reading > 25000 ) {
        if (counter == 1) {
          x = x + 2;
          y = y + 2;
          counter = 0;
        }
        else {
          counter++;
        }
      }
      else {
        x = 0;
        y = 0;
      }
    }
    x1 = setFrontSpeed - x;
    Y1 = motor1pwm - y;
    //////Serial.println(x1);
    if (x1 <= 70) x1 = 70;
    if (Y1 <= 70) Y1 = 70;
    analogWrite(MOT1_pwmPin, Y1);
    analogWrite(MOT2_pwmPin, x1);
    //timer = micros();
  }
}
void rotateCcw(int clicks) {
  if (micros() - timer > iterationTime) {

    rotateWheel0.setSetPoint(clicks);
    rotateWheel1.setSetPoint(clicks);
    rotateWheel2.setSetPoint(clicks);

    encoder0Reading = encoder0.read();
    encoder1Reading = encoder1.read();
    encoder2Reading = encoder2.read();

    rotateWheel0.Calculate(abs(encoder0Reading));
    rotateWheel1.Calculate(abs(encoder1Reading));
    rotateWheel2.Calculate(abs(encoder2Reading));

    int motor0pwm = rotateWheel0.out;
    int motor1pwm = rotateWheel1.out;
    int motor2pwm = rotateWheel2.out;

    if (motor0pwm > 0) digitalWrite(MOT0_directionPin, MOT0_CW );
    if (motor0pwm < 0) digitalWrite(MOT0_directionPin, MOT0_ACW );

    if (motor2pwm > 0) digitalWrite(MOT2_directionPin, MOT2_CW );
    if (motor2pwm < 0) digitalWrite(MOT2_directionPin, MOT2_ACW );

    if (motor1pwm > 0) digitalWrite(MOT1_directionPin, MOT1_CW );
    if (motor1pwm < 0) digitalWrite(MOT1_directionPin, MOT1_ACW );

    analogWrite(MOT0_pwmPin, abs(motor0pwm));
    analogWrite(MOT1_pwmPin, abs(motor1pwm));
    analogWrite(MOT2_pwmPin, abs(motor2pwm));
    //timer = micros();
  }
}
void rotateAboutOneWheel(int clicks) {
  if (micros() - timer > iterationTime) {

    rotateWheel0.setSetPoint(clicks);
    rotateWheel1.setSetPoint(clicks);
    rotateWheel2.setSetPoint(clicks);

    encoder0Reading = encoder0.read();
    encoder1Reading = encoder1.read();
    encoder2Reading = encoder2.read();

    rotateWheel0.Calculate(abs(encoder0Reading));
    //rotateWheel1.Calculate(abs(encoder1Reading));
    rotateWheel2.Calculate(abs(encoder2Reading));

    int motor0pwm = rotateWheel0.out;
    //int motor1pwm = rotateWheel1.out;
    int motor2pwm = rotateWheel2.out;

    if (motor0pwm > 0) digitalWrite(MOT0_directionPin, MOT0_CW );
    if (motor0pwm < 0) digitalWrite(MOT0_directionPin, MOT0_ACW );

    if (motor2pwm > 0) digitalWrite(MOT2_directionPin, MOT2_CW );
    if (motor2pwm < 0) digitalWrite(MOT2_directionPin, MOT2_ACW );

    //if (motor1pwm > 0) digitalWrite(MOT1_directionPin, MOT1_CW );
    //if (motor1pwm < 0) digitalWrite(MOT1_directionPin, MOT1_ACW );

    analogWrite(MOT0_pwmPin, abs(motor0pwm));
    //analogWrite(MOT1_pwmPin, abs(motor1pwm));
    analogWrite(MOT2_pwmPin, abs(motor2pwm));
    //timer = micros();
  }
}
void rotatecw(int clicks) {
  if (micros() - timer > iterationTime) {

    rotateWheel0.setSetPoint(clicks);
    rotateWheel1.setSetPoint(clicks);
    rotateWheel2.setSetPoint(clicks);

    encoder0Reading = encoder0.read();
    encoder1Reading = encoder1.read();
    encoder2Reading = encoder2.read();

    rotateWheel0.Calculate(abs(encoder0Reading));
    rotateWheel1.Calculate(abs(encoder1Reading));
    rotateWheel2.Calculate(abs(encoder2Reading));

    int motor0pwm = rotateWheel0.out;
    int motor1pwm = rotateWheel1.out;
    int motor2pwm = rotateWheel2.out;

    if (motor0pwm > 0) digitalWrite(MOT0_directionPin, MOT0_ACW );
    if (motor0pwm < 0) digitalWrite(MOT0_directionPin, MOT0_CW );

    if (motor2pwm > 0) digitalWrite(MOT2_directionPin, MOT2_ACW );
    if (motor2pwm < 0) digitalWrite(MOT2_directionPin, MOT2_CW );

    if (motor1pwm > 0) digitalWrite(MOT1_directionPin, MOT1_ACW );
    if (motor1pwm < 0) digitalWrite(MOT1_directionPin, MOT1_CW );

    analogWrite(MOT0_pwmPin, abs(motor0pwm));
    analogWrite(MOT1_pwmPin, abs(motor1pwm));
    analogWrite(MOT2_pwmPin, abs(motor2pwm));
    //timer = micros();
  }
}
void moveSideWays(int setSpeed2) {
  digitalWrite(MOT0_directionPin, MOT0_CW);
  digitalWrite(MOT1_directionPin, MOT1_ACW);
  digitalWrite(MOT2_directionPin, MOT2_ACW);
  analogWrite(MOT2_pwmPin, setSpeed2);
  if (micros() - timer > iterationTime) {

    encoder0Reading = encoder0.read();
    encoder1Reading = encoder1.read();
    encoder2Reading = encoder2.read();
    sideways.setSetPoint(abs(encoder2Reading));
    sideways1.setSetPoint(abs(encoder2Reading * 2));
    sideways.Calculate(abs(encoder1Reading));
    sideways1.Calculate(abs(encoder0Reading));
    //    frontWheel.Calculate(abs(encoder0Reading));
    int motor0pwm = abs(sideways1.out);
    int motor1pwm = abs(sideways.out);
    if (motor0pwm > 255) motor0pwm = 255;
    //    if (motor0pwm > 0) digitalWrite(MOT0_directionPin, MOT0_CW );
    //    if (motor0pwm < 0) digitalWrite(MOT0_directionPin, MOT0_ACW );

    analogWrite(MOT0_pwmPin, motor0pwm);
    analogWrite(MOT1_pwmPin, motor1pwm);
    //timer = micros();
  }
}
void linefollow() {
  readLine = true;
  if (sensor[3] == 1 && sensor[4] == 1) {
    analogWrite(MOT0_pwmPin, 0);
  }
  else if ( sensor[0] == 1 && sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 && sensor[4] == 1 && sensor[5] == 1 && sensor[6] == 1 && sensor[7] == 1) {

  }
  else {
    if (sensor[3] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_CW);
      analogWrite(MOT0_pwmPin, 60);
    }
    else if (sensor[3] == 1 && sensor[2] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_CW);
      analogWrite(MOT0_pwmPin, 65);
    }
    else if (sensor[2] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_CW);
      analogWrite(MOT0_pwmPin, 70);
    }
    else if (sensor[2] == 1 && sensor[1] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_CW);
      analogWrite(MOT0_pwmPin, 75);
    }
    else if (sensor[1] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_CW);
      analogWrite(MOT0_pwmPin, 80);
    }
    else if (sensor[1] == 1 && sensor[0]) {
      digitalWrite(MOT0_directionPin, MOT0_CW);
      analogWrite(MOT0_pwmPin, 85);
    }
    else if (sensor[0] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_CW);
      analogWrite(MOT0_pwmPin, 90);
    }
    if (sensor[4] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_ACW);
      analogWrite(MOT0_pwmPin, 60);
    }
    else if (sensor[4] == 1 && sensor[5] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_ACW);
      analogWrite(MOT0_pwmPin, 65);
    }
    else if (sensor[5] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_ACW);
      analogWrite(MOT0_pwmPin, 70);
    }
    else if (sensor[5] == 1 && sensor[6] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_ACW);
      analogWrite(MOT0_pwmPin, 75);
    }
    else if (sensor[6] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_ACW);
      analogWrite(MOT0_pwmPin, 80);
    }
    else if (sensor[6] == 1 && sensor[7]) {
      digitalWrite(MOT0_directionPin, MOT0_ACW);
      analogWrite(MOT0_pwmPin, 85);
    }
    else if (sensor[7] == 1) {
      digitalWrite(MOT0_directionPin, MOT0_ACW);
      analogWrite(MOT0_pwmPin, 90);
    }
  }
}
//void lineSensorAvg() {
//  for(int i = 0; i < 5 ; i++) {
//    readLineSensor();
//    dummies[i] = dummy;
//  }
//  LedCount = 0;
//  for(int i = 0; i < 8 ; i++) {
////    //////Serial.print("Sensor ");
////    //////Serial.print(i);
////    //////Serial.print(" : ");
//    lineReadings[i] = bitRead(dummies[0],i) + bitRead(dummies[1],i) + bitRead(dummies[2],i) + bitRead(dummies[3],i) + bitRead(dummies[4],i);
//    if(lineReadings[i] >= 3) {
//      sensor[i] = true;
//      LedCount++;
//    }
//    else {
//      sensor[i] = false;
//    }
////    //////Serial.print(sensor[i]);
////    //////Serial.print("\t");
//
//  }
//
////    //////Serial.print(" Junction Pulse : ");
////  //////Serial.print(digitalRead(junctionPulse));
////  //////Serial.println();
//}
void matchJunction() {
  if (micros() - alignmentTimeOut < 3000000) {
    Vertical.readLineSensor(sensor);
    Horizontal.readLineSensor(sensor);
    linePositionP = Vertical.getPosition();
    linePositionH = Horizontal.getPosition();
    if (!xAlign) {
      if (micros() - timer > iterationTime) {
        int CurrentPosition = ((linePositionH - linePositionP) > 0) ? linePositionP + (linePositionH - linePositionP) / 2.0 : linePositionH + (linePositionP - linePositionH) / 2.0;
        int error = SetPoint - CurrentPosition;
        int derivative = error - prevError;
        if (error == 0 && integral != 0) {
          integral = 0;
        }
        else if (error == 0 && integral == 0) {
          xAlign = true;
          integral = 0;
          prevError = 0;
        }
        else {
          integral += error;
        }
        prevError = error;
        //      //////Serial.print(" xAlign : ");
        //      //////Serial.print(xAlign);
        //      //////Serial.print(" CurPos : ");
        //      //////Serial.print(CurrentPosition);
        //      //////Serial.print(" error : ");
        //      //////Serial.print(error);
        //      //////Serial.print(" Derivative : ");
        //      //////Serial.print(derivative);
        //      //////Serial.print(" Integral : ");
        //      //////Serial.print(integral);
        //      //////Serial.println();
        int Out = abs(error) * 1.7 + abs(derivative) * 2.5 + abs(integral) * 0.05;
        if (error > 0) {
          //cw acw acw
          digitalWrite(MOT0_directionPin, MOT0_CW);
          digitalWrite(MOT1_directionPin, MOT1_ACW);
          digitalWrite(MOT2_directionPin, MOT2_ACW);
          analogWrite(MOT0_pwmPin, Out * 2);
          analogWrite(MOT1_pwmPin, Out);
          analogWrite(MOT2_pwmPin, Out);
        }
        else {
          //cw acw acw
          digitalWrite(MOT0_directionPin, MOT0_ACW);
          digitalWrite(MOT1_directionPin, MOT1_CW);
          digitalWrite(MOT2_directionPin, MOT2_CW);
          analogWrite(MOT0_pwmPin, Out * 2);
          analogWrite(MOT1_pwmPin, Out);
          analogWrite(MOT2_pwmPin, Out);
        }
        timer = micros();
      }
    }
    else {
      if (micros() - timer > iterationTime) {
        float error = sqrt((sq(linePositionP - 35 ) + sq(linePositionH - 32)) / 2.0);
        if ((35 - linePositionH) > 0 || (35 - linePositionP) < 0) {
          error = error;
        }
        else if ((35 - linePositionH) < 0 || (35 - linePositionP) > 0) {
          error = error * (-1);
        }
        float derivative = error - prevError;
        if (error == 0 && integral != 0) {
          integral = 0;
        }
        else if (error == 0 && integral == 0) {
          adjustForShoot = false;
          analogWrite(MOT0_pwmPin, 0);
          analogWrite(MOT1_pwmPin, 0);
          analogWrite(MOT2_pwmPin, 0);
          readyToThrow = true;
          forwardDelay = micros();
          haltBeforeThrowing = micros();
          throwTime = 370;
          Vertical.setUartMode(0x01);
          Horizontal.setUartMode(0x01);
          return;
        }
        else {
          integral += double(error);
        }

        //      //////Serial.print(" xAlign : ");
        //      //////Serial.print(xAlign);
        //      //////Serial.print(" CurPosH : ");
        //      //////Serial.print(linePositionH);
        //      //////Serial.print(" CurPosP : ");
        //      //////Serial.print(linePositionP);
        //      //////Serial.print(" error : ");
        //      //////Serial.print(error);
        //      //////Serial.print(" Derivative : ");
        //      //////Serial.print(derivative);
        //      //////Serial.print(" Integral : ");
        //      //////Serial.print(integral);
        //      //////Serial.println();
        int Out = 20 + abs(error) * 1.2 + abs(derivative) * 2.5 + abs(integral) * 0.01;

        if ((error) > 0) {
          //cw acw acw
          digitalWrite(MOT0_directionPin, MOT0_CW);
          digitalWrite(MOT1_directionPin, MOT1_CW);
          digitalWrite(MOT2_directionPin, MOT2_CW);
          analogWrite(MOT0_pwmPin, Out);
          analogWrite(MOT1_pwmPin, Out);
          analogWrite(MOT2_pwmPin, Out);
        }
        else {
          //cw acw acw
          digitalWrite(MOT0_directionPin, MOT0_ACW);
          digitalWrite(MOT1_directionPin, MOT1_ACW);
          digitalWrite(MOT2_directionPin, MOT2_ACW);
          analogWrite(MOT0_pwmPin, Out);
          analogWrite(MOT1_pwmPin, Out);
          analogWrite(MOT2_pwmPin, Out);
        }

        prevError = error;
        timer = micros();
      }
    }
  }
  else {
    StopDrives();
    adjustForShoot = false;
    analogWrite(MOT0_pwmPin, 0);
    analogWrite(MOT1_pwmPin, 0);
    analogWrite(MOT2_pwmPin, 0);
    readyToThrow = true;
    forwardDelay = micros();
    haltBeforeThrowing = micros();
    throwTime = 370;
    Vertical.setUartMode(0x01);
    Horizontal.setUartMode(0x01);

  }
}




void gyroInit() {
   Wire.begin();
//#if ARDUINO >= 157
//  Wire.setClock(400000UL); // Set I2C frequency to 400kHz
//#else
//  TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
//#endif

  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    ////Serial.print(F("Error reading sensor"));
    while (1);
  }
  delay(100); // Wait for sensor to stabilize
  while(samplesGyro <128) {
    readGyroRaw();
    gyroCalib();
  }
  readGyroRaw();
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  kalmanY.setAngle(pitch);
  Gyrotimer = micros();
}
void gyroCalib() {
  if (samplesGyro < 64) {
    samplesGyro++;
  }
  else if (samplesGyro < 128) {
    gyroOffset[0] += gyroX;
    gyroOffset[1] += gyroY;
    gyroOffset[2] += gyroZ;
    samplesGyro++;
  }
  else if (samplesGyro == 128) {
    gyroOffset[0] /= 64;
    gyroOffset[1] /= 64;
    gyroOffset[2] /= 64;
    ////Serial.println("Gyroscope Offsets Calculated!!!");
    samplesGyro++;
  }
}
void getPitch() {
  readGyroRaw();
  double dt = (double)(micros() - Gyrotimer) / 1000000;
  Gyrotimer = micros();
  double pitch = atan2(-accX,accZ) * RAD_TO_DEG;
  double gyroYrate = gyroY / 131.0;
  if((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    kalAngleY = pitch;
  }
  else {
    kalAngleY = kalmanY.getAngle(pitch,gyroYrate,dt);
  }
  curAng = kalAngleY;
}
void readGyroRaw() {
  while (i2cRead(0x3B, i2cData, 14));
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
  tempRaw = (int16_t)((i2cData[6] << 8) | i2cData[7]);
  gyroX = (int16_t)((i2cData[8] << 8) | i2cData[9]);
  gyroY = (int16_t)((i2cData[10] << 8) | i2cData[11]);
  gyroZ = (int16_t)((i2cData[12] << 8) | i2cData[13]);;
  if(samplesGyro > 128) {
    gyroX -= gyroOffset[0];
    gyroY -= gyroOffset[1];
    gyroZ -= gyroOffset[2];
  }
}


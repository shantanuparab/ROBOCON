

// I2Cdev and HMC5883L must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project


// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
HMC5883L mag;

int16_t mx, my, mz;

#define LED_PIN 13
bool blinkState = false;
void magInit() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    
    // initialize device
    Serial.println("Initializing I2C devices...");
    mag.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");

}
float magCalc() {
    // read raw heading measurements from device
    mag.getHeading(&mx, &my, &mz);
  
    // display tab-separated gyro x/y/z values
   
// To calculate heading in degrees. 0 degree indicates North
    float heading = atan2(my, mx);
    if(heading<0)
      heading += 2 * M_PI;
    return (heading * 180/M_PI);

}



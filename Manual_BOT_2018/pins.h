#include "PS2X_lib.h"  //for v1.6
#include <Wire.h>
#include "I2Cdev.h"
#include "HMC5883L.h"


//int mot4 = 26; int pwm4 = 2; 
//int mot3 = 28; int pwm3 = 3; 
//int mot2 = 30; int pwm2 = 6; 
//int mot1 = 5; int pwm1 = 4; 

int mot4 = 8; int pwm4 = 9; 
int mot3 = 10; int pwm3 = 11; 
int mot2 = 2; int pwm2 = 3; 
int mot1 = 4; int pwm1 = 5; 


// this is for clockwise, each motor 
#define cw_1 HIGH
#define cw_2 LOW 
#define cw_3 LOW 
#define cw_4 HIGH 
 
// for anti-clockwise, automactically defined on basis of clockwise values 
#define acw_1 (!cw_1) 
#define acw_2 (!cw_2) 
#define acw_3 (!cw_3) 
#define acw_4 (!cw_4) 

#define RAMP_FACTOR_X 1
#define RAMP_FACTOR_Y 1
#define RAMP_FACTOR_R 5
int calibrated_pwm4(int x) {return (x);} 
int calibrated_pwm3(int x) {return (x);}   
int calibrated_pwm2(int x) {return (x);}     
int calibrated_pwm1(int x) {return (x);} 

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "Classes.h"

//////////////////////////////////////////////////////
///////////////////////Timers/////////////////////////
long timer = 0;
long safetyTimer = 0;
long iterationTime = 10000;
long haltTimer = 0;
long pause = 500000;
long shuttleCockHalt = 0;
long openGripperTimer = 0;
long extendPlatformTimer = 0;
long closeGripperTimer = 0;
long retractPlatformTimer = 0;
long transferMotorTimer = 0;
long platformRetractDelay = 0;
long forwardDelay = 0;
long backwardDelay = 0;
long backHalt = 0;
long throwTime = 0;
long haltBeforeThrowing = 0;
long rotateDirChangeTime = 0;
long pullDelayTimer = 0;
long pullDelayRampSpeed = 70000;
long gripperOpenDelay = 0;
long alignmentTimeOut = 0;
long transferMotorOutTimeOut = 0;
long SlotSensorTimeOut = 0;
//////////////////////////////////////////////////////
/////////////////Line/////////////////////////////////
LSA08 Horizontal(3,0x01,22,24,115200);
LSA08 Vertical(3,0x02,26,28,115200);

int sensor[8];
int sensor1[8];
unsigned int junctionCount = 0;
int LedCount = 0;
int linePositionP = 0;
int linePositionH = 0;
bool xAlign = false;
double integral = 0;
float prevError = 0;
bool angAlign = false;
int SetPoint = 35;
//////////////////////////////////////////////////////
///////////////Encoders///////////////////////////////
Encoder encoder0(2, 3);
Encoder encoder1(18, 16);
Encoder encoder2(19, 17);

long encoder0Reading = 0;
long encoder1Reading = 0;
long encoder2Reading = 0;

long M0Position = -999;
long M1Position = -999;
long M2Position = -999;

/////////////////////////////////////////////////////
////////////////Motors//////////////////////////////
#define MOT0_directionPin  4
#define MOT1_directionPin  6
#define MOT2_directionPin  10

#define MOT0_pwmPin  5
#define MOT1_pwmPin  7
#define MOT2_pwmPin  11

#define MOT0_CW LOW
#define MOT1_CW HIGH
#define MOT2_CW HIGH

#define MOT0_ACW !MOT0_CW
#define MOT1_ACW !MOT1_CW
#define MOT2_ACW !MOT2_CW

/////////////////////////////////////////////////////
/////////////////Transfer////////////////////////////
#define TRANSFER_MOTOR_DIR 8
#define TRANSFER_MOTOR_PWM 9 
#define TRANSFER_MOTOR_CW HIGH
#define TRANSFER_MOTOR_ACW !TRANSFER_MOTOR_CW
#define LINEAR_ACTUATOR_DIR1 33
#define LINEAR_ACTUATOR_DIR2 35

#define FORWARD_PISTON 29
#define BACKWARD_PISTON 27
#define ROTATING_PISTON  25
#define EXTENDING_PISTON 23
#define ROTATING_PISTON_EXTEND HIGH
#define EXTENDING_PISTON_EXTEND HIGH

bool motorToIr2 = false;
bool pistonPartDone2 = false;
bool hallSensorStatus = false;
bool transferStart = false;
bool transferEnd = false;
bool Ir1Status = true;
bool Ir2Status = true;
bool pistonPartDone = false;
bool motorNextDir = false;            //false = next motor movement will be inwards
bool motorInside = false;             // false = motor is inside
bool transferInitComplete = false;    //true = transfermechanism is ready to detect shuttle cock
bool transferComplete = false;        // true = transfermechanism has successfully transfered the shuttlecock
                                      //        and is ready to shoot

int extending_piston_state = !EXTENDING_PISTON_EXTEND;
int rotating_piston_state = !ROTATING_PISTON_EXTEND;

/////////////////////////////////////////////////////
//////////////////////Gyro///////////////////////////
#include <WSWire.h>
#include <Kalman.h> // Source: https://github.com/TKJElectronics/KalmanFilter

#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf

Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;

/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter
double gyroOffset[3];
int samplesGyro = 0;
bool changeAngle = false;
bool checkAngle = false;
double curAng = 0;
int setAng = 0;
uint32_t Gyrotimer;
uint8_t i2cData[14]; // Buffer for I2C data

/////////////////////////////////////////////////////
//////////////SensorPins&Booleans////////////////////
#define IR1Pin A0
#define IR2Pin A7
#define HallSensorPin A4
#define SlotSensor A11
#define shootFlash A15
#define tz1Tz2RackIr 40
#define tz3RackIr 42
int irArray[4] = {30,31,37,39};
bool detectReady = true;
bool junctionWasDetectedAfterRotation = false;
bool motorSet = true;
long encoder0ReadingAtJunction = 0;
long encoder1ReadingAtJunction = 0;
bool checkIrArray = false;
bool MagnetPresent = false;
bool openingGripper = false;
bool gripperOpened = false;
bool platformExtended = false;
bool closingGripper = false;
bool gripperClosed = false;
bool platformRetracted = false;
bool motorOnPos = false;
bool shuttleCockInSlot = false;
bool shuttleCockDetected = false;
bool readHallSensorForFirstTime = true;
bool goToTz1Junction = false;
bool reachedTz1Junction = false;
bool readLine = false;
bool readLine1 = false;
bool Junction = false;
bool adjustForShoot = false;
bool rotateFunctionCalledFirstTime = false;
bool rotateccw = false;
bool goToTransferLocation = false;
bool detectShuttleCock = false;
bool moveToThrowingJunction = false;
bool reachedThrowingJunction = false;
bool motorStart = true;
bool moveSideways = false;
bool transferCompCalledFirstTime = true;
bool checkJunction = false;
int countJunction = 0;
bool prevJunction = false;
bool resetPid = false;
bool goBackToTransferLocation = false;
int location = 0;
bool readIr = false;
bool readyToThrow = false;
bool shuttleCockThrowed = false;
bool halt = false;
int pullBackCounter = 0;
bool pullDelay = false;
bool angleIsSet = false;
bool throwStart = false;
bool goBackToTransferJunction = false;
bool shuttleCockShot = false;
bool moveLittleAheadDesiJugad = false;
bool moveToTz2Junction = false;
bool glaIsSet = false;
bool angleJustChanged = false;
bool pullStart = micros();
int x = 0,x1,y = 0,Y1;
CommMega mega1;

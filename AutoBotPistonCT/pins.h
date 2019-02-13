
// pin 33 on teensy is used as reset
#define TEENSY_RESET_PIN 33

// one complete revolution of motor gives 120240 encoder readings
#define ENCODER_REVOLVE_COUNT 120240

Encoder enc0(10,11);
Encoder enc1(24,12);
Encoder enc2(32,31);
Encoder enc3(30,29);
long moveCount = 100000;

const byte motPwn[] = {5,3,7,9};
const byte motDir[] = {4,2,6,8};
//  Front of AutoBot
//  mot0        mot1
//
//
//  mot2        mot3

bool motStopped = false;
bool syncMovDir = true;
int selectPair = 0;

float Kp = 0.08;
int basePwm = 60;

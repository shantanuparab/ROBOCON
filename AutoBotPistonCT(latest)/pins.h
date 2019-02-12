
// pin 33 on teensy is used as reset
#define TEENSY_RESET_PIN 33

// one complete revolution of motor gives 120240 encoder readings
#define ENCODER_REVOLVE_COUNT 120240

Encoder enc0(24,25);
Encoder enc1(29,28);
Encoder enc2(27,26);
Encoder enc3(31,30);
long moveCount = 50000;

const byte motPwn[] = {7,4,9,2};
const byte motDir[] = {6,5,8,3};
//  Front of AutoBot
//  mot0        mot1
//
//
//  mot2        mot3

bool motStopped = false;
bool syncMovDir = true;
int selectPair = 0;

float Kp = 1;
int basePwm = 60;

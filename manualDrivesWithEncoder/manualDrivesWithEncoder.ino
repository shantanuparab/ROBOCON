#include"pins.h"
#include"LSA08.h"
#include"drives.h"
#include<Encoder.h>

long baudRate = 9600;
int flag = 1;

char Address1 = 0x00;
char UartMode1 = 0x02;
byte SerialPin1 = 24;
byte JunctionPin1 = 19;

char Address2 = 0x03;
char UartMode2 = 0x02;
byte SerialPin2 = 28;
byte JunctionPin2 = 20;

byte lineValue;
int setPoint = 35;
float Kp = 1.75, Kd = 36;
long pidTime;
bool junction1Enab = false;
bool junction2Enab = false;
float lastError = 0;
int PositionL = 0;
int PositionR = 0;

bool Align = false;

LSA08 lineSensor1(&Serial3, baudRate, Address1, UartMode1, SerialPin1, JunctionPin1);
LSA08 lineSensor2(&Serial3, baudRate, Address2, UartMode2, SerialPin2, JunctionPin2);
Encoder encodR(A7, 2);
Encoder encodL(3, A4);

struct Movements
{
	const byte Flag;
	const uint32_t Dist;
	const uint32_t RampDist;
	const uint32_t MaxDist;
public:
	constexpr Movements(const byte flag, const uint32_t p_dist, const int p_rampDist, const int p_maxDist) : Flag{ flag }, Dist{ p_dist }, RampDist{ p_rampDist }, MaxDist{ p_maxDist }
	{

	}
};

const Movements movements[] = { Movements{1, 250, 500, 700}, Movements{3, 2, 300, 900}, Movements{1, 100, 350, 550}, Movements{2, 2, 260, 400} };
byte cur_idx = 0;

template<typename T, uint8_t N>
constexpr uint8_t size_arr(const T(&)[N])
{
	return N;
}

void junctionCheck1()
{
	if (junction1Enab && flag == 1)
	{
		Align = true;
		junction1Enab = false;
	}
}

void junctionCheck2()
{
	if (junction2Enab && flag != 1)
	{
		Align = true;
		junction2Enab = false;
	}
}

void setup() {
	delay(3000);
	Serial.begin(9600);
	Serial.println("Start Setup");
	lineSensor1.initialize();
	lineSensor2.initialize();
	for (int i = 0; i < 3; i++) {
		pinMode(motorDir[i], OUTPUT);
		pinMode(motorPwm[i], OUTPUT);
	}
	Serial.println("End Setup");
	//attachInterrupt(digitalPinToInterrupt(JunctionPin1), junctionCheck1, FALLING);
	//attachInterrupt(digitalPinToInterrupt(JunctionPin2), junctionCheck2, FALLING);
	pidTime = micros();
	movePlatform(0, 0, 100);
}

void loop() {
}
//	Serial.print("movements[cur_idx].Flag");
//	Serial.println(movements[cur_idx].Flag);
//	PositionL = encodL.read();
//	PositionR = encodR.read();
//
//	Serial.print(PositionL);
//	Serial.print("  ");
//	Serial.print(PositionR);
//	Serial.print("  ");
//	Serial.print(((PositionL + PositionR) / 2));
//	Serial.println("  ");
//
//	if (!junction1Enab && flag == 1 && abs(PositionR) >= movements[cur_idx].Dist)
//	{
//		Serial.println(F("Junction Check 1"));
//		junction1Enab = true;
//	}
//	if (!junction2Enab && flag != 1 && abs(PositionR) >= movements[cur_idx].Dist)
//	{
//		Serial.println(F("Junction Check 2"));
//		junction2Enab = true;
//	}
//	else {}
//	if (flag == 1/*Previous*/ && Align)
//	{
//		if (cur_idx >= size_arr(movements))
//		{
//			Serial.println(F("Bot Work done"));
//			movePlatform(0, 0, 0);
//			while (true);
//		}
//		flag = movements[++cur_idx].Flag;
//		Align = false;
//		encodR.write(0);
//		movePlatform(0, 50, 0);
//		while (true) {
//			PositionR = abs(encodR.read());
//			Serial.print(F("Encoder align"));
//			Serial.println(PositionR);
//			if (PositionR > 25)
//				break;
//		}
//		encodR.write(0);
//		movePlatform(0, 0, 0);
//	}
//	if (Align && flag != 1/*Previous*/)
//	{
//		if (cur_idx >= size_arr(movements))
//		{
//			Serial.println(F("Bot Work done"));
//			movePlatform(0, 0, 0);
//			while (true);
//		}
//		flag = movements[++cur_idx].Flag;
//		Align = false;
//		movePlatform(0, 0, 0);
//		encodR.write(0);
//		Serial.println(F("Realign"));
//	}
//	else {}
//	if (flag == 1)
//		lineValue = lineSensor1.read();
//	else
//		lineValue = lineSensor2.read();
//
//	if (lineValue >= 0 && lineValue <= 70)
//	{
//		if (micros() - pidTime > 1000) {
//			Serial.print(F(" Line Value: "));
//			Serial.print(lineValue);
//			pidTime = micros();
//			int8_t error = lineValue - setPoint;
//			int32_t rampDist = movements[cur_idx].RampDist;
//			//      Serial.print(" Error: ");
//			//      Serial.print(error);
//			float Correction = Kp * error + Kd * (error - lastError);
//			lastError = error;
//			Serial.print(" flag: ");
//			Serial.println(flag);
//			uint32_t PositionAvg = abs((PositionL + PositionR) / 2);
//			if (flag == 1 && rampDist <= PositionAvg)
//				movePlatform(0, 100, Correction);
//			else if (flag == 1 && rampDist > PositionAvg)
//				ramp(Correction, flag, PositionAvg);
//
//			else if (flag == 2 && rampDist <= PositionAvg)
//				movePlatform(-40, 0, Correction);
//			else if (flag == 2 && rampDist > PositionAvg)
//				ramp(Correction, flag, PositionAvg);
//
//			else if (flag == 3 && rampDist <= PositionAvg)
//				movePlatform(100, 0, Correction);
//			else if (flag == 3 && rampDist > PositionAvg)
//				ramp(Correction, flag, PositionAvg);
//		}
//	}
//	else
//	{
//		movePlatform(0, 0, 0);
//	}
//
//}
//void ramp(float Correction, int flag, int PositionAvg)
//{
//	int rampPWM = movements[cur_idx].MaxDist - PositionAvg;
//	rampPWM = map(rampPWM, 0, rampPWM, 0, 255);
//	if (flag == 1)
//		movePlatform(0, rampPWM, Correction);
//	else if (flag == 2)
//		movePlatform(-rampPWM, 0, Correction);
//	else if (flag == 3)
//		movePlatform(rampPWM, 0, Correction);
//}
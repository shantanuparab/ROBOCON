#include <Encoder.h>

volatile unsigned long long last = 0;

constexpr int dir_1 = 4;
constexpr int pwm_1 = 5;
constexpr int dir_2 = 2;
constexpr int pwm_2 = 3;

Encoder enc1(22, 23);
Encoder enc2(20, 21);

void setup() {
	pinMode(dir_1, OUTPUT);
	pinMode(pwm_1, OUTPUT);
	pinMode(dir_2, OUTPUT);
	pinMode(pwm_2, OUTPUT);
	Serial.begin(115200);
}
int prev_count1 = 0;
int prev_count2 = 0;

void loop() {
	auto const now = micros();
	auto const dt = now - last;
	if (dt > 200) {
		auto const count1 = enc1.read();
		auto const count2 = enc2.read();
		auto const diff = (count1 - count2)/2000;
		auto pwmFor1 = (127);
		auto pwmFor2 = (127 + (0.5 * diff));
		pwmFor2 = constrain(pwmFor2, 0, 255);
		pwmFor1 = constrain(pwmFor1, 0, 255);

		Serial.print(count1);
		Serial.print(" ");
		Serial.print(count2);
		Serial.print(" ");

		if (count1 / 120240 > 10)
		{
			pwmFor1 = 0;
			pwmFor2 = 0;
		}

		last = micros();
		digitalWrite(dir_1, LOW);
		analogWrite(pwm_1, pwmFor1);
		digitalWrite(dir_2, HIGH);
		analogWrite(pwm_2, pwmFor2);
		Serial.print(pwmFor1);
		Serial.print(" ");
		Serial.print(pwmFor2);
		Serial.print(" ");
		Serial.print(diff);
		Serial.print(" ");
		Serial.println();
	}
}

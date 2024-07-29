#ifndef NECK_SERVO
#define NECK_SERVO

#include <Arduino.h>
#include <Servo.h>

class NeckServo {
	private:
		Servo neck;
		const int LEFT_SPD = 105;
		const int RIGHT_SPD = 80;
		const int STOP_POS = 90;
		const int RIGHT_DELAY = 1150;
		const int PAUSED = 1000;
		const int LEFT_DELAY = 2000;
		const int STOPPED = 5000;
		int position = 0;
		int neckRotationTimeMillis = 5;

	public:
    void attachNeck(int servoPin);
		void scan();
};

#endif

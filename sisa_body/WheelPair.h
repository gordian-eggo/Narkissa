#ifndef WHEEL_PAIR
#define WHEEL_PAIR

#include <Arduino.h>
#include "WheelMotor.h"

class WheelPair {
	private:
		const int leftSpeed = 100;
		const int rightSpeed = 70;
	
	public:
		WheelMotor leftWheel(AF_DCMotor leftMotor);
    WheelMotor rightWheel(AF_DCMotor rightMotor);
		
		WheelPair(WheelMotor left, WheelMotor right) {}
		void leftTurn();
		void rightTurn();
  
};

#endif

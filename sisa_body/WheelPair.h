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
		// do we really need to turn?
    // kung yung focus is yung image processing, i don't think it's super important that 
    // it moves left and right. good to have but not super important
		void leftTurn();
		void rightTurn();
  
};

#endif

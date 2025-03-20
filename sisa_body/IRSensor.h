#ifndef IR_SENSOR
#define IR_SENSOR

#define IR_1 A1
#define IR_2 A2
#define IR_3 A3
#define IR_4 A4

/*
  NOTES:

    14-3-2025: For the LM393 infrared sensor, one LED light is for the power, the other is for object location.
               I don't think it's necessary to have both lights on but I think as long as the object detection 
               light is on we can assume that the sensor is drawing power.
*/

#include <Arduino.h>

class IRSensor {

	private:
        		

	public:
    

};

#endif

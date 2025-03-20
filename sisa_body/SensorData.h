#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <Arduino.h>

class SensorData {

	private: 
		int wheel1RPM;
		int wheel2RPM;
		int wheel3RPM;
		int wheel4RPM;

	public:
		int getRPMValue(int wheelNum);
		void setRPMValue(int wheelNum, int rpm);

};

#endif
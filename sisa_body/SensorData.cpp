#include "SensorData.h"

int SensorData::getRPMValue(int wheelNum) {
	int selectedWheelRPM;
	switch (wheelNum) {
		case 1:
			selectedWheelRPM = this->wheel1RPM;
			break;
		case 2:
			selectedWheelRPM = this->wheel2RPM;
			break;
		case 3:
			selectedWheelRPM = this->wheel3RPM;
			break;
		case 4:
			selectedWheelRPM = this->wheel4RPM;
			break;
	}

	return selectedWheelRPM;
}


void SensorData::setRPMValue(int wheelNum, int rpm) {
	switch (wheelNum) {
		case 1:
			this->wheel1RPM = rpm;
			break;
		case 2:
			this->wheel2RPM = rpm;
			break;
		case 3:
			this->wheel3RPM = rpm;
			break;
		case 4:
			this->wheel4RPM = rpm;
			break;
	}
}

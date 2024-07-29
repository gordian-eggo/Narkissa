#include "WheelMotor.h"

void WheelMotor::setLeftTurnSpeed(int leftSpeed) {
  this->leftTurnSpeed = leftSpeed;
}

void WheelMotor::setRightTurnSpeed(int rightSpeed) {
  this->rightTurnSpeed = rightSpeed;
}

void WheelMotor::startCounter() { this->counter++; }
void WheelMotor::resetCounter() { this->counter = 0; }

// TODO: shuta lord pano ba yung timer neto pero sige hataw ang bakla gow
void WheelMotor::setupIRSensor(int irSensorPin) {
  pinMode(irSensorPin, OUTPUT);
}

void WheelMotor::calculateRPM() {
  this->RPMCounter = (this->counter / this->diskSlots) * 60;
  resetCounter();
}

void WheelMotor::forward() {
  startCounter();  
  this->motor.setSpeed(defaultForwardSpeed);
  this->motor.run(FORWARD);
}

void WheelMotor::reverse() {
  startCounter();  
  this->motor.setSpeed(defaultReverseSpeed);
  this->motor.run(BACKWARD);
}

void WheelMotor::pause() {
  resetCounter();
  this->motor.run(RELEASE);
}

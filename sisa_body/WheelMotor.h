#ifndef WHEEL_MOTOR
#define WHEEL_MOTOR

#include <Arduino.h>
#include <arduino-timer.h>        // documentation here: https://github.com/contrem/arduino-timer
#include "AFMotor.h"

// TODO: Re-plan yung OOP nito since marunong-ish na tayo mag OOP sa C++
class WheelMotor {
  private:
    const int defaultForwardSpeed = 200;
    const int defaultReverseSpeed = 200;
    const int scanForwardSpeed = 50;
    const int scanReverseSpeed = 50;
    const int diskSlots = 20;
    const int minuteInterval = 60000;
    int counter;
//    IRSensor irSensor;
    float rpmCounter;
    float motorRPM;

  public:
    Timer<2, millis> timer;
    volatile byte RPMCounter = 0;
    int leftTurnSpeed;
    int rightTurnSpeed;
    
    AF_DCMotor motor;
    WheelMotor(int motorNum, int motorPWMRate)
    :motor(motorNum, motorPWMRate) {}
    WheelMotor(AF_DCMotor motor)
    :motor(motor) {}
    // getters and setters
    int getLeftTurnSpeed();
    int getRightTurnSpeed();
 
    void setLeftTurnSpeed(int leftSpeed);
    void setRightTurnSpeed(int rightSpeed);
    
    // main functions
    void setupIRSensor(int irSensorPin);
    void startCounter();
    void resetCounter();
    void calculateRPM();
    void forward();
    void reverse();
    void pause();
    
};

#endif

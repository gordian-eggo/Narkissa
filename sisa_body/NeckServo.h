#ifndef NECK_SERVO
#define NECK_SERVO

#include <Arduino.h>
#include <Servo.h>

class NeckServo {
  private:
    Servo neck;
    const int LEFT_SPD = 180;
    const int RIGHT_SPD = -180;
    const int DEFAULT_POS = 90;
    const int RIGHT_DELAY = 2000;
    const int LEFT_DELAY = 2000;
    const int SCAN_DELAY = 5000;
    const int RESET_DELAY = 2000;
    int position = 0;
    int neckRotationTimeMillis = 5;

  public:
    void attachNeck(int servoPin);
    void neckReset();
    void scan();
    void defaultMovementLoop();
};

#endif

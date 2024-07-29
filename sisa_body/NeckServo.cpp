#include "NeckServo.h"

void NeckServo::attachNeck(int servoPin) {
  this->neck.attach(servoPin);
}

void NeckServo::scan() {

  /*
    Limitation: manually resetting the position of the neck using the 360-degree
    servo is more difficult than with the 180-degree servo because it's reliant
    on timing.
  */
   
   neck.write(RIGHT_SPD);
   delay(RIGHT_DELAY);
   neck.write(STOP_POS);
   delay(PAUSED);
   neck.write(LEFT_SPD);
   delay(LEFT_DELAY);
   neck.write(STOP_POS);
   delay(PAUSED);
   neck.write(RIGHT_SPD);
   delay(RIGHT_DELAY);
   neck.write(STOP_POS);
   delay(STOPPED);

}

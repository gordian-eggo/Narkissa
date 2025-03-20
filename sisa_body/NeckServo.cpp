#include "NeckServo.h"

/*
  16-3-2025: okay pwede sana ako mag-stepper motor kas 1) baka kailangan ko pa ng
               isa pang Arduino para i-control yun eh bulky na nga yung setup as it
               is, and 2)focus po tayo sa mirror detection code kasi katawan lang po
               talaga 'to, somebody can make a better body than I can for sure but
               the mirror detecting? That's mine, that's my thing. Okay? Okay.
*/

void NeckServo::attachNeck(int servoPin) {
  this->neck.attach(servoPin);
}

void NeckServo::neckReset() {
  this->neck.write(DEFAULT_POS);
}

void NeckServo::scan() {
   
   this->neck.write(LEFT_SPD);
   delay(LEFT_DELAY);
   this->neckReset();
   this->neck.write(RIGHT_SPD);
   delay(RIGHT_DELAY);
   this->neckReset();
   
}

void NeckServo::defaultMovementLoop() {

  /*
    Always reset the servo motor to its default position at 90 degrees.
    Maintain uniformity.
  */
  
  this->neckReset();
  delay(RESET_DELAY);
  this->scan();
  delay(SCAN_DELAY);
}

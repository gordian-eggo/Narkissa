/* Run "sudo chmod a+rw /dev/ttyACM0" if you get
the "can't open device: Permission denied error" */

#include <EnableInterrupt.h>      //https://github.com/GreyGnome/EnableInterrupt/wiki/Usage#user-content-disableInterrupt
#include <arduino-timer.h>
#include "WheelMotor.h"
#include "NeckServo.h"

// pins for IR sensors
#define BL_IR A4
#define BR_IR A3
#define FL_IR A2
#define FR_IR A1

/*
 * Wiring scheme:
 * 
  Y-----                      ----- B
        |--- M1_BL   M4_FL ---|
  w-----                      ----- Y

  Y-----                      ----- B
        |--- M2_BR   M3_FL ---|
  W-----                      ----- Y

*/
int servoPin = 10;

// white + yellow wires
WheelMotor frontRight(1, MOTOR12_2KHZ);
WheelMotor frontLeft(2, MOTOR12_2KHZ);
// blue + yellow wires
WheelMotor rearLeft(3, MOTOR34_1KHZ);
WheelMotor rearRight(4, MOTOR34_1KHZ);

NeckServo neckServo;

Timer<4, millis> sysTimer;
volatile byte RPSCounter = 0;
float disk_slots = 20;

int bl_ir, br_ir, fl_ir, fr_ir = 0;

void setup() {

//    sysTimer.every(1000, M1_RPM);
//    sysTimer.every(1000, M2_RPM);
//    sysTimer.every(1000, M3_RPM);
//    sysTimer.every(1000, M4_RPM);

    // Q: RPS ng alin? all four wheels ba?
    neckServo.attachNeck(servoPin);
    Serial.begin(115200);
  
}

//TODO: safety controller? It oughta be okay,
//      tutal the point is to detect mirror by itself. 
//      A safety controller during testing to keep 
//      property damage to a minimum should be okay.

void loop() {
  
//  bl_ir = analogRead(BL_IR);
//  br_ir = analogRead(BR_IR);
//  fl_ir = analogRead(FL_IR);
//  fr_ir = analogRead(FR_IR);

  /* 
   * hindi to dapat mano-mano mag-perform pag dating ng  
   * defense. TODO: switch case for mirror positions
   */

//   neckTurn();
//
   sysTimer.tick();
}

//// working 10/3, needs some adjustment tho
//void left() {
//  
//  front_left.setSpeed(50);
//  front_right.setSpeed(50);
//  back_left.setSpeed(70);
//  back_right.setSpeed(70);
//  
//  front_left.run(BACKWARD);
//  front_right.run(BACKWARD);
//  back_left.run(FORWARD);
//  back_right.run(FORWARD);
//
//}
//
//// working 10/3, needs some adjustment
//void right() {
//  
//  front_left.setSpeed(70);
//  front_right.setSpeed(90);
//  back_left.setSpeed(70);
//  back_right.setSpeed(90);
//  
//  front_left.run(FORWARD);
//  front_right.run(FORWARD);
//  back_left.run(FORWARD);
//  back_right.run(FORWARD);
//
//}

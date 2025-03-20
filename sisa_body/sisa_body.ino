/* Run "sudo chmod a+rw /dev/ttyACM0" if you get
  the "can't open device: Permission denied error" */

#include <arduino-timer.h>
#include <SoftwareSerial.h>         // to allow use of other pins for bluetooth rx/tx
//#include "WheelMotor.h"
#include "WheelPair.h"
#include "NeckServo.h"

// pins for IR sensors and HC-05
// gusto ko pa sana gamitin yung motor shield after this
#define BL_IR A5
#define BR_IR A4
#define FL_IR A3
#define FR_IR A2
#define HC_TXD A1
#define HC_RXD A0


/*
   Wiring scheme:

  B-----                      ----- R
        |--- M1_BL   M4_FL ---|
  R-----                      ----- B

  R-----                      ----- R
        |--- M2_BR   M3_FL ---|
  B-----                      ----- B

*/

int servoPin = 10;

// setup for front and back wheel pairs
WheelMotor frontRight(1, MOTOR12_2KHZ);
WheelMotor frontLeft(2, MOTOR12_2KHZ);
WheelMotor rearLeft(3, MOTOR34_1KHZ);
WheelMotor rearRight(4, MOTOR34_1KHZ);

WheelPair front(frontRight, frontLeft);
WheelPair rear(rearRight, rearLeft);

NeckServo neckServo;

// bluetooth requirements
SoftwareSerial bt(HC_RXD, HC_TXD);
int state = 0;

int bl_ir, br_ir, fl_ir, fr_ir = 0;

void setup() {
  // Q: RPS ng alin? all four wheels ba?
  
  neckServo.attachNeck(servoPin);
  
  Serial.begin(9600);
  Serial.println("Establishing bluetooth connection...");
  bt.begin(9600);

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
     hindi to dapat mano-mano mag-perform pag dating ng
     defense. TODO: switch case for mirror positions
  */
//  startBluetoothFeed(&bt);
  neckServo.defaultMovementLoop();

  

  //   sysTimer.tick();
}

void startBluetoothFeed(SoftwareSerial *bt) {

  //HC-05 address (AT+ADDR) = +ADDR:98D3:61:F687EE
  /*

    12-3-2025: debugging issues with bluetooth. got AT commands to work again
               
  
  */

  if (Serial.available()) {
    bt->write(Serial.read());
//    state = Serial.read();
  }

  if (bt->available()) {
    Serial.write(bt->read());
  }
  
}
 

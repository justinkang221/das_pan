#include <phys253.h>
#include <LiquidCrystal.h>

#include <pan.h>
#include <arm.h>
#include <drive.h>
#include <path.h>
#include <passenger.h>

// Pan, RCServo2 --> left, RCServo1 --> right
Pan pan;
// Arm(motor pin, pot pin, cycle speed), uses RCServo0
Arm arm(37, 0, 135);
// Tape (left motor, right motor, inner left qrd, inner right qrd, outer left qrd, outer right qrd, reverse left qrd, reverse right qrd)
Drive drive(2, 1, 1, 2, 0, 3, 5, 7);
// Path (starting left - T or F)
Path path(0);
// Passenger (general left IR, precise left IR, general right IR, precise right IR)
Passenger passenger(5, 7, 4 , 6);

void setup() {
#include <phys253setup.txt>
  Serial.begin(9600);
  path.find();
  arm.center();
  // pan.leftUp();
  pan.rightUp();
}

uint8_t next;
uint8_t t;
boolean findPath = false;
boolean veryFirst = true;

/*void loop() {
  drive.reverse();
  }*/

void loop() {
  drive.straight();
  while ( stopbutton() ) {
    drive.setPD(map(knob(6), 0, 1023, 0, 100), map(knob(7), 0, 1023, 0, 100));
    drive.stats();
  }

  /*if (passenger.detect() == 1) {
    drive.brake();

    pan.leftPick();

    arm.left();
    arm.cycle();
    arm.center();

    pan.leftUp();

    drive.straight();
    }
    else if (passenger.detect() == 2) {
    drive.brake();

    pan.rightPick();

    arm.right();
    arm.cycle();
    arm.center();

    pan.rightUp();

    drive.straight();
    }*/


  if (drive.intersection() || veryFirst) {
    veryFirst = false;

    drive.brake();
    while ( !startbutton() );

    findPath = true;
    t = path.turn();
    switch (t) {
      case 0: drive.reverse();
        break;
      case 1: drive.left();
        break;
      case 2: drive.straight();
        break;
      case 3: drive.right();
        break;
      case 4: drive.uturn();
        break;
    }
    path.stats();
    path.update();
    if (path.nearDrop()) drive.prepareDrop();
    if (path.nearEndpoint()) drive.prepareEndpoint();

    drive.straight();
  }
  else if (findPath) {
    path.find();
    findPath = false;
    path.stats();
  }

  drive.straight();
}


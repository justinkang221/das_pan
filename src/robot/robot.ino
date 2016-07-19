#include <phys253.h>
#include <LiquidCrystal.h>

#include <pan.h>
#include <arm.h>
#include <drive.h>
#include <path.h>
#include <passenger.h>

Pan pan;
Arm arm;
Drive drive;
Path path;
Passenger passenger;

void setup() {
#include <phys253setup.txt>
  Serial.begin(9600);

  path.find();

  arm.center();
  pan.leftUp();
  pan.rightUp();

  //path.startingRight();
}

uint8_t next;
uint8_t t;
boolean findPath = false;

void looop() {
  drive.left();

  while ( stopbutton() ) {
    drive.brake();
    drive.setPD(map(knob(6), 0, 1023, 0, 100), map(knob(7), 0, 1023, 0, 100));
    drive.stats();
  }
}

void loop() {
  drive.straight();

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


  if (drive.intersection()) {
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

    path.update();

    if (path.nearDrop()) drive.prepareDrop();
    if (path.nearEndpoint()) drive.prepareEndpoint();

    drive.straight();
  }
  else if (findPath) {
    path.find();
    findPath = false;
  }

  path.stats();
  drive.straight();
}


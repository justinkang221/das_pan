#include <phys253.h>
#include <LiquidCrystal.h>

#include <pan.h>
#include <arm.h>
#include <drive.h>
#include <path.h>
#include <passenger.h>

Pan pan;
Arm arm(0, 0, 10);
// Tape left motor, right motor, inner left qrd, inner right qrd, outer left qrd, outer right qrd
Drive drive(2, 1, 1, 2, 0, 3);
Path path(0);
// Passenger general left IR, precise left IR, general right IR, precise right IR
// Passenger passenger(1, 3, 0 , 2);

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  path.stats();
}

uint8_t next;
uint8_t turn;
boolean findPath = true;
boolean forward = true;
uint16_t c = 1000;

void loop() {
  drive.straight();

  /*if (drive.intersection() && c > 1000) {
    //drive.brake();
    drive.left();
    c = 0;
    }*/

  ++c;
  while ( stopbutton() ) {
    drive.setPD(map(knob(6), 0, 1023, 0, 100), map(knob(7), 0, 1023, 0, 100));
    drive.stats();
  }

  /*while ( startbutton() ) {
    passenger.stats();
    }*/

  /*if (passenger.detect() == 1) {
    drive.speed(50);
    while (passenger.precise() != 1) {
      while (passenger.precise() != 1) {
        if (forward) drive.straight();
        else drive.reverse();
      }
      drive.brake();
      forward = !forward;
    }

    pan.leftDown();
    arm.left();
    arm.cycle();
    arm.center();
    pan.leftUp();

    forward = true;
    drive.straight();
    }
    else if (passenger.detect() == 2) {
      drive.speed(50);
      while (passenger.precise() != 2) {
      while (passenger.precise() != 2) {
        if (forward) drive.straight();
        else drive.reverse();
      }
      drive.brake();
      forward = !forward;
    }

    pan.rightDown();
    arm.right();
    arm.cycle();
    arm.center();
    pan.rightUp();

    forward = true;
    drive.straight();
    }*/

  if (drive.intersection() && c > 1000) {
    drive.brake();
    while( !startbutton() );
    findPath = true;
    turn = path.turn();
    switch (turn) {
      case 0: drive.reverse();
        break;
      case 1: drive.left();
        break;
      case 2: drive.straight();
        break;
      case 3: drive.right();
        break;
    }
    drive.straight();
    c = 0;
  }

  else if (findPath) {
    path.find();
    findPath = false;
    path.stats();
  }
  drive.straight();
}


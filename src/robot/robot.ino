#include <phys253.h>
#include <LiquidCrystal.h>

#include <pan.h>
#include <arm.h>
#include <drive.h>
#include <path.h>
#include <passenger.h>

// Pan servo1 is right, servo2 is left
Pan pan;
// Arm (motor, pot, speed) uses Servo0
Arm arm(37, 0, 135);
// Tape (left motor, right motor, inner left qrd, inner right qrd, outer left qrd, outer right qrd)
Drive drive(2, 1, 1, 2, 0, 3);
// Path (starting left - T or F);
Path path(1);
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
uint8_t turn;
boolean findPath = false;
uint16_t c = 1000;
boolean veryFirst = true;

/*void loop() {
  drive.straight();
  passenger.stats();

  if (passenger.precise() == 2) {
    drive.brake();

    pan.rightDown();
    arm.right();
    arm.cycle();
    arm.center();
    pan.rightUp();
  }
}*/

void loop() {
  drive.straight();
  
  ++c;
  /*while ( stopbutton() ) {
    drive.setPD(map(knob(6), 0, 1023, 0, 100), map(knob(7), 0, 1023, 0, 100));
    drive.stats();
    }

    while ( startbutton() ) {
    passenger.stats();
    }*/

  /*if (passenger.detect() == 1) {
    drive.brake();
    
    pan.leftDown();
    arm.left();
    arm.cycle();
    arm.center();
    pan.leftUp();

    drive.straight();
  }*/
  /*if (passenger.detect() == 2) {
    drive.brake();

    pan.rightDown();
    arm.right();
    arm.cycle();
    arm.center();
    pan.rightUp();
    
    drive.straight();
  }*/

  if ((drive.intersection() && c > 1000) || veryFirst) {
    veryFirst = false;
    drive.brake();
    while ( !startbutton() );
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
    path.stats();
    path.update();
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


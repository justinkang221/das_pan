#include <phys253.h>
#include <LiquidCrystal.h>

#include <pan.h>
#include <arm.h>
#include <drive.h>
#include <path.h>
#include <passenger.h>

Arm arm;
Drive drive;
Pan pan;
Passenger passenger;
Path path;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600);

  pan.leftUp();
  pan.rightUp();
  arm.center();
}

uint8_t t;
int8_t n;
boolean ccw = false;
boolean tight = false;

int8_t seen;
uint8_t expected = 0;

uint16_t distance;
uint16_t leftD;
uint16_t rightD;

boolean leftPassengers = false;
boolean rightPassengers = false;

boolean corner = false;
boolean crash = false;

boolean lost = false;
boolean endpoint = false;

void loops() {
  if ( passenger.coarse() == 1 ) {
    if ( !leftPassengers ) pickUp(true);
    else if ( !rightPassengers ) {
      drive.uturn(true);
      pickUp(false);
    }
  }
  else if ( passenger.coarse() == 2) {
    if ( !rightPassengers ) pickUp(false);
    else if ( !leftPassengers ) {
      drive.uturn(false);
      pickUp(true);
    }
  }
}

void loop()
{
  drive.go();

  if ( drive.atIntersection() ) {
    seen = drive.describeIntersection();

    if ( drive.atIntersection() == 0 ) {
      /*drive.brake();
      LCD.home();
      path.stats();
      while ( !startbutton() );*/
      
      leftD = drive.leftDistance();
      rightD = drive.rightDistance();

      //drive.stats(true);
      //path.stats();
      //need a way to account for movement during turning
      if ( lost ) {
        if ( leftD > 110 && rightD > 110 && seen == 3 ) {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found 15");
          path.reorient(15);
          expected = path.getDirections();
          t = path.turn();
          path.find();
          path.update();
        }
        else if ( leftD > 110 && rightD > 110 && seen == 6 )  {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found 10");
          path.reorient(10);
          expected = path.getDirections();
          t = path.turn();
          path.find();
          path.update();
        }
        else if ( leftD < 80 && rightD > 100 && seen == 6 ) {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found CR");
          path.reorient(13);
          expected = path.getDirections();
          t = path.turn();
          path.find();
          path.update();
        }
        else if ( leftD > 100 && rightD < 80 && seen == 3 ) {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found CL");
          path.reorient(12);
          expected = path.getDirections();
          t = path.turn();
          path.find();
          path.update();
        }
      }

      if ( lost || ( seen != expected  && n != 7 ) ) {
        lost = true;
        LCD.home();
        LCD.print("FUCKED!!!!!");
        LCD.print(expected);
        LCD.print(seen);
        if (seen & B010) t = 2;
        else if (seen == 5) t = (drive.getDistance() % 2 == 0) ? 1 : 3;
        else if (seen & B001) t = 1;
        else if (seen & B100) t = 3;
      }
      else {
        /*drive.brake();
          path.stats();
          while ( !startbutton() );*/

        if ( path.nearDrop() ) {
          /*drive.brake();
            LCD.home();
            LCD.clear();
            LCD.print("near drop");
            while ( !startbutton() );*/
          drive.prepareDrop();
        }
        // path.update();
      }

      switch ( t ) {
        case 0: drive.reverse();
          break;
        case 1: drive.left(tight);
          break;
        case 2: drive.straight();
          break;
        case 3: drive.right(tight);
          break;
        case 4: drive.uturn(ccw);
          break;
      }
    }
  }
  else {
    corner = drive.intersection();
    crash = drive.collision();

    /*if (corner) {
      drive.brake();
      LCD.home();
      LCD.print("corner  ");
      while(!startbutton);
    }
    if (crash) {
      drive.brake();
      LCD.home();
      LCD.print("crashed  ");
      while(!startbutton);
    }*/

    if ( passenger.coarse() == 1 ) {
      drive.brake();
      while( !startbutton() ) passenger.stats(false);
      if ( !leftPassengers ) pickUp(true);
      else if ( !rightPassengers ) {
        drive.uturn(true);
        pickUp(false);
      }
    }
    else if ( passenger.coarse() == 2) {
      drive.brake();
      while( !startbutton() ) passenger.stats(false);
      if ( !rightPassengers ) pickUp(false);
      else if ( !leftPassengers ) {
        drive.uturn(false);
        pickUp(true);
      }
    }

    if ( corner || crash ) {
      if (lost && crash) drive.uturn(true);
      else {
        if ( n == -1 ) {
          if (leftPassengers) {
            drive.brake();
            pan.leftDrop();
            delay(500);

            leftPassengers = 0;

            pan.leftUp();
          }

          if (rightPassengers) {
            drive.uturn(true);
            pan.rightDrop();
            delay(500);

            rightPassengers = 0;

            pan.rightUp();
            drive.uturn(false);
          }

          path.passengers(leftPassengers + rightPassengers);
        }
        else if ( n == -2 ) {
          if ( rightPassengers )
          {
            drive.brake();
            pan.rightDrop();
            delay(500);

            rightPassengers = 0;

            pan.rightUp();
          }

          if ( leftPassengers )
          {
            drive.uturn(false);
            pan.leftDrop();
            delay(500);

            leftPassengers = 0;

            pan.leftUp();
            drive.uturn(true);
          }

          path.passengers(leftPassengers + rightPassengers);
        }

        ccw = ( n == 2 );
        tight = ( n == 3 );
        if ( ( n == 2 ) || ( n == 1 ) ) {
          endpoint = true;
        }

        n = path.find();
        expected = path.getDirections();
        drive.record(true);

        if ( crash ) {
          if ( endpoint ) {
            endpoint = false;
            drive.uturn(ccw);
          }
          /*else if ( drive.getDistance() < 10 && t != 2 ) {
            path.avoid();
            drive.unturn(t, tight, true);
            path.find();
            path.update();
            n = path.find();
            expected = path.getDirections();
          }*/
          else {
            path.avoid();
            drive.uturn(true);
            n = path.find();
            expected = path.getDirections();
          }
        }

        t = path.turn();

        path.update();

        drive.record(true);
      }
    }
  }
}

void pickUp(boolean left) {
  drive.speed(50);
  distance = drive.getDistance();
  if (left) {
    while ( drive.getDistance() < distance + 4 ) {
      drive.go();

      if ( passenger.precise() == 1) {
        drive.brake();

        arm.leftCenter();
        pan.leftPick();
        arm.cycle();

        if ( pan.leftFull() ) {
          leftPassengers = true;
          pan.leftUp();
          arm.center();
          break;
        }

        arm.leftFront();
        arm.cycle();

        if ( pan.leftFull() ) {
          leftPassengers = true;
          pan.leftUp();
          arm.center();
          break;
        }

        arm.leftBack();
        arm.cycle();

        if ( pan.leftFull() ) {
          leftPassengers = true;
        }
        pan.leftUp();
        arm.center();
        break;
      }
    }
    path.passengers(leftPassengers + rightPassengers);
    /*LCD.home();
      LCD.clear();
      LCD.print("l: ");
      LCD.print(leftPassengers);
      LCD.print(" r: ");
      LCD.print(rightPassengers);
      while ( !startbutton() );*/
    drive.speed(150);
    drive.record(true);
  }

  else {
    while ( drive.getDistance() < distance + 4 ) {
      drive.go();
      //passenger.stats();
      if ( passenger.precise() == 2) {
        drive.brake();

        arm.rightCenter();
        pan.rightPick();
        arm.cycle();

        if (pan.rightFull()) {
          rightPassengers = true;
          pan.rightUp();
          arm.center();
          break;
        }

        arm.rightFront();
        arm.cycle();

        if (pan.rightFull()) {
          rightPassengers = true;
          pan.rightUp();
          arm.center();
          break;
        }

        arm.rightBack();
        arm.cycle();

        if (pan.rightFull()) {
          rightPassengers = true;
        }
        pan.rightUp();
        arm.center();
        break;
      }
    }
    path.passengers(leftPassengers + rightPassengers);
    /*LCD.home();
      LCD.clear();
      LCD.print("l: ");
      LCD.print(leftPassengers);
      LCD.print(" r: ");
      LCD.print(rightPassengers);
      while ( !startbutton() );*/
    drive.speed(150);
    drive.record(true);
  }
}


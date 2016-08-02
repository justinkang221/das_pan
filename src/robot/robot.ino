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

  arm.center();
  pan.leftUp();
  pan.rightUp();
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

void loop()
{
  drive.go();

  if ( drive.atIntersection() ) {
    seen = drive.describeIntersection();

    if ( drive.atIntersection() == 0 ) {
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
          drive.brake();
          path.reorient(15);
          expected = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(expected);
          LCD.print(" th:");
          LCD.print(seen);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
        else if ( leftD > 110 && rightD > 110 && seen == 6)  {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found 10");
          drive.brake();
          path.reorient(10);
          expected = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(expected);
          LCD.print(" th:");
          LCD.print(seen);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
        else if ( leftD < 80 && rightD > 100 && seen == 6 ) {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found CR");
          drive.brake();
          path.reorient(13);
          expected = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(expected);
          LCD.print(" th:");
          LCD.print(seen);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
        else if ( leftD > 100 && rightD < 80 && seen == 3 ) {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found CL");
          drive.brake();
          path.reorient(12);
          expected = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(expected);
          LCD.print(" th:");
          LCD.print(seen);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
      }

      if ( lost || seen != expected ) {
        lost = true;
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
    crash = drive.collision();// && ( n != 0 ); // TODO: implement collisions with other robots and get rid of n != 0
    
    if ( passenger.coarse() == 1 && !leftPassengers) {
      drive.speed(50);
      distance = drive.getDistance();
      while ( drive.getDistance() < distance + 4 ) {
        drive.go();
        //passenger.stats();
        if ( passenger.precise() == 1) {
          drive.brake();

          arm.leftCenter();
          pan.leftPick();
          arm.cycle();

          if (pan.leftFull()) {
            leftPassengers = true;
            pan.leftUp();
            break;
          }

          arm.leftFront();
          arm.cycle();

          if (pan.leftFull()) {
            leftPassengers = true;
            pan.leftUp();
            break;
          }

          arm.leftBack();
          arm.cycle();

          if (pan.leftFull()) {
            leftPassengers = true;
            pan.leftUp();
          }
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

    if ( passenger.coarse() == 2 && !rightPassengers ) {
      drive.speed(50);
      distance = drive.getDistance();
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
            break;
          }

          arm.rightFront();
          arm.cycle();

          if (pan.rightFull()) {
            rightPassengers = true;
            pan.rightUp();
            break;
          }

          arm.rightBack();
          arm.cycle();

          if (pan.rightFull()) {
            rightPassengers = true;
            pan.rightUp();
          }
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
      //drive.go();
    }

    if ( corner || crash ) {
      if (lost && crash) drive.uturn(true);
      else {
        /*drive.brake();
          path.stats();
          while (!startbutton());*/
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
          else if ( drive.getDistance() < 15 ) {
            path.avoid();
            drive.unturn(t, tight, true);
            path.find();
            path.update();
            n = path.find();
            expected = path.getDirections();
          }
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


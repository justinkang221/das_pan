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
int8_t thing;
uint16_t currDistance;
uint16_t lastLeftDist;
uint16_t lastRightDist;

uint8_t leftPassengers = 0, rightPassengers = 0;

boolean corner = false;
boolean crash = false;
boolean ccw = false;
boolean tight = false;
boolean lost = false;
boolean avoids = false;
uint8_t directions = 0;
boolean endpoint = false;

void loops()
{
  drive.go();
  if (drive.intersection()) {
    drive.uturn(true);
    drive.brake();
    while (!startbutton());
  }
}

void loop()
{
  drive.go();

  if (drive.isSacked()) {
    thing = drive.describeIntersection();
    if (drive.isSacked() == 0) {
      lastLeftDist = drive.getLastTraveledL();
      lastRightDist = drive.getLastTraveledR();
      //drive.stats(true);
      //path.stats();
      //need a way to account for movement during turning
      if (lost) {
        if (lastLeftDist > 110 && lastRightDist > 110 && thing == 3) // we are at destination 15
        {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found 15");
          drive.brake();
          path.reorient(15);
          directions = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(directions);
          LCD.print(" th:");
          LCD.print(thing);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
        else if (lastLeftDist > 110 && lastRightDist > 110 && thing == 6) // we are at destination 10
        {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found 10");
          drive.brake();
          path.reorient(10);
          directions = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(directions);
          LCD.print(" th:");
          LCD.print(thing);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
        else if (lastLeftDist < 80 && lastRightDist > 100 && thing == 6) // we are at the circle going right
        {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found CR");
          drive.brake();
          path.reorient(13);
          directions = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(directions);
          LCD.print(" th:");
          LCD.print(thing);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
        else if (lastLeftDist > 100 && lastRightDist < 80 && thing == 3) // we are at the circle going left
        {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("I'm found CL");
          drive.brake();
          path.reorient(12);
          directions = path.getDirections();
          t = path.turn();
          LCD.home();
          LCD.print("d:");
          LCD.print(directions);
          LCD.print(" th:");
          LCD.print(thing);
          LCD.print(" t:");
          LCD.print(t);
          while ( !startbutton() );
        }
      }
      if (lost || thing != directions)
      {
        lost = true;
        if (thing & B010) t = 2;
        else if (thing == 5) t = (drive.getDistance() % 2 == 0) ? 1 : 3;
        else if (thing & B001) t = 1;
        else if (thing & B100) t = 3;
      }
      else
      {
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

      switch (t)
      {
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
  else
  {
    corner = drive.intersection();
    crash = drive.collision();// && ( n != 0 ); // TODO: implement collisions with other robots and get rid of n != 0
    // TODO: handle IR detection around corners and stuff
    if ( passenger.precise() == 1 && leftPassengers < 2 )
    {
      drive.speed(50);
      /*while ( passenger.precise() != 1 ) {
        drive.go();
        passenger.stats();
        }*/
      drive.brake();

      if (leftPassengers) {
        currDistance = drive.getDistance();
        drive.speed(50);
        while (drive.getDistance() < currDistance + 3) drive.go();
        drive.brake();
        arm.leftBack();
      }
      else arm.leftFront();

      pan.leftPick();

      arm.cycle();

      arm.center();

      if (pan.leftFull(leftPassengers)) ++leftPassengers;
      path.passengers(leftPassengers + rightPassengers);
      LCD.home();
      LCD.clear();
      LCD.print("l: ");
      LCD.print(leftPassengers);
      LCD.print(" r: ");
      LCD.print(rightPassengers);
      while ( !startbutton() );

      pan.leftUp();
      drive.speed(150);
      //drive.go();
    }

    if ( passenger.precise() == 2 && rightPassengers < 2 )
    {
      drive.speed(50);
      /*while ( passenger.precise() != 2 ) {
        drive.go();
        passenger.stats();
        }*/

      drive.brake();

      pan.rightPick();

      if (rightPassengers) arm.rightBack();
      else arm.rightFront();
      arm.cycle();

      path.passengers(leftPassengers + ++rightPassengers);

      arm.center();

      pan.rightUp();
      drive.speed(150);
      //drive.go();
    }

    if ( corner || crash )
    {
      if (lost) {
        if (crash) {
          drive.uturn(true);
        }
      }
      else {
        /*drive.brake();
          path.stats();
          while (!startbutton());*/
        if (n == -1)
        {
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
        else if (n == -2)
        {
          if (rightPassengers)
          {
            drive.brake();
            pan.rightDrop();
            delay(500);

            rightPassengers = 0;

            pan.rightUp();
          }

          if (leftPassengers)
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
        if (( n == 2) || ( n == 1)) {
          endpoint = true;
        }

        n = path.find();
        directions = path.getDirections();
        drive.record(true);

        t = path.turn();

        //if ( path.nearEndpoint() ) drive.prepareEndpoint();
        if ( crash ) {
          if (endpoint) {
            endpoint = false;
            drive.uturn(ccw);
          }
          else {
            path.avoid();
            avoids = true;
            drive.uturn(true);
            n = path.find();
            directions = path.getDirections();
          }
        }
        
        path.update();
        
        drive.record(true);
      }
    }
  }
}


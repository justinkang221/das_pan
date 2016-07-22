#include <phys253.h>
#include <LiquidCrystal.h>

#include <pan.h>
#include <arm.h>
#include <drive.h>
#include <path.h>
#include <passenger.h>

#define CCW (2)

Arm arm;
Drive drive;
Pan pan;
Passenger passenger;
Path path;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600);

  path.find();

  arm.center();
  pan.leftUp();
  pan.rightUp();
}

uint8_t t;
int8_t n;
uint8_t findPath = 0;

uint8_t leftPassengers = 0, rightPassengers = 0;

int c = 0;
uint8_t collision = 0;
boolean ccw = true;

void lop()
{
  drive.go();
  if (drive.intersection()) {
    drive.brake();
    while ( !startbutton() );
    drive.uturn(ccw);
    ccw = !ccw;
  }
}

void loop()
{
  drive.go();

  collision = drive.collision();
  // TODO: handle IR detection around corners and stuff
  if ( passenger.precise() == 1 && leftPassengers <= 2 )
  {
    drive.speed(50);
    /*while ( passenger.precise() != 1 ) {
      drive.go();
      passenger.stats();
      }*/

    drive.brake();

    pan.leftPick();

    arm.left();
    arm.cycle();

    path.passengers(++leftPassengers + rightPassengers);

    arm.center();

    pan.leftUp();
    drive.speed(150);
    //drive.go();
  }

  if ( passenger.precise() == 2 && rightPassengers <= 2 )
  {
    drive.speed(50);
    /*while ( passenger.precise() != 2 ) {
      drive.go();
      passenger.stats();
      }*/

    drive.brake();

    pan.rightPick();

    arm.right();
    arm.cycle();

    path.passengers(leftPassengers + ++rightPassengers);

    arm.center();

    pan.rightUp();
    drive.speed(150);
    //drive.go();
  }

  if ( drive.intersection() || collision ) {
    /*drive.brake();
    path.stats();
    while ( !startbutton() );*/
    // TODO: write path.weights()
    /*if ( collision )
    {
      drive.brake(); // get rid of this
      LCD.clear();
      LCD.home();
      LCD.print("n: ");
      LCD.print(n);
      LCD.print(" c: ");
      LCD.print(collision);
      while ( !startbutton() ); // get rid of this
      if ( n == -3 && collision < 4 );
      else if ( n == 1 && collision > 5 );
      else
      {
        path.update(); // pretend you've hit the intersection you were going towards
        path.avoid(); // ADJUST WEIGHTS TO GO AWAY FROM COLLISION
        path.find(); // find the path that leads you away from collision
      }
    }*/

    // TODO: test drop off
    if (n == -1)
    {
      if (leftPassengers) {
        drive.brake();
        pan.leftDrop();
        delay(1000);

        leftPassengers = 0;

        pan.leftUp();
      }

      if (rightPassengers) {
        drive.uturn(true);
        pan.rightDrop();
        delay(1000);

        rightPassengers = 0;

        pan.rightUp();
        drive.uturn(false);
      }

      path.passengers(leftPassengers + rightPassengers);
    }
    else if (n == -2)
    {
      if (rightPassengers) {
        drive.brake();
        pan.rightDrop();
        delay(1000);

        rightPassengers = 0;

        pan.rightUp();
      }

      if (leftPassengers) {
        drive.uturn(false);
        pan.leftDrop();
        delay(1000);

        leftPassengers = 0;

        pan.leftUp();
        drive.uturn(true);
      }

      path.passengers(leftPassengers + rightPassengers);
    }

    t = path.turn();

    path.update();

    if ( path.nearDrop() ) drive.prepareDrop();
    //if ( path.nearEndpoint() ) drive.prepareEndpoint();

    n = path.find();
    switch (t)
    {
      case 0: drive.uturn( n == CCW ? true : false );
        break;
      case 1: drive.left();
        break;
      case 2: drive.straight();
        break;
      case 3: drive.right();
        break;
      case 4: drive.uturn(true);
        break;
    }
  }

  //path.stats();
}


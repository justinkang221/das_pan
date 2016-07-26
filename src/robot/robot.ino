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

uint8_t leftPassengers = 0, rightPassengers = 0;

boolean corner = false;
boolean crash = false;
boolean ccw = false;
boolean tight = false;

void loop()
{
  drive.uturn(1);
}

void fuck()
{
  drive.go();

  corner = drive.intersection();
  crash = drive.collision() && ( n != 0 ); // TODO: implement collisions with other robots and get rid of n != 0
  // TODO: handle IR detection around corners and stuff
  if ( passenger.precise() == 1 && leftPassengers < 2 )
  {
    drive.speed(50);
    /*while ( passenger.precise() != 1 ) {
      drive.go();
      passenger.stats();
      }*/

    drive.brake();

    pan.leftPick();

    if (leftPassengers) arm.leftBack();
    else arm.leftFront();
    arm.cycle();

    path.passengers(++leftPassengers + rightPassengers);

    arm.center();

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
    drive.brake();
    path.stats();
    while( !startbutton() );
    // TODO: write path.avoid()
    /*if ( crash )
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

    if ( path.nearDrop() ) drive.prepareDrop();
    //if ( path.nearEndpoint() ) drive.prepareEndpoint();
    
    ccw = ( n == 2 );
    tight = ( n == 3 );

    drive.go();
    
    n = path.find();

    drive.go();
    
    t = path.turn();

    path.update();
    
    
    switch (t)
    {
      case 0: drive.uturn(ccw);
        break;
      case 1: drive.left(tight);
        break;
      case 2: drive.straight();
        break;
      case 3: drive.right(tight);
        break;
      case 4: drive.reverse();
          break;
    }
  }
}


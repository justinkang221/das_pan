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

  path.find();

  arm.center();
  pan.leftUp();
  pan.rightUp();
}

uint8_t t, n;
boolean findPath = false;

uint8_t leftPassengers = 0, rightPassengers = 0;

boolean r = true; // get rid of this

void loopy()
{
  if (r) arm.right();
  else arm.left();
  arm.cycle();
  delay(1000);
  r = !r;
}

void loop()
{
  drive.go();

  /*if ( passenger.detect() == 1 && leftPassenger <= 2 )
  {
    while ( passenger.precise() != 1 ) drive.go();

    drive.brake();

    pan.leftPick();

    arm.left();
    arm.cycle();

    path.passengers(++leftPassenger, rightPassenger);

    arm.center();

    pan.leftUp();
    //drive.go();
  }

  if ( passenger.detect() == 2 && rightPassenger <= 2 )
  {
    while ( passenger.precise() != 2 ) drive.go();

    drive.brake();

    pan.rightPick();

    arm.right();
    arm.cycle();
    path.passengers(leftPassenger, ++rightPassenger);
    arm.center();

    pan.rightUp();
    //drive.go();
  }*/

  if ( drive.intersection() )
  {
    drive.brake(); // get rid of this
    while ( !startbutton() ); // get rid of this

    // TODO: test drop off
    if (n == -1 && leftPassengers)
    {
      drive.brake();
      pan.leftDrop();

      leftPassengers = 0;
      path.passengers(leftPassengers, rightPassengers);

      pan.leftUp();
    }
    else if (n == -2 && rightPassengers)
    {
      drive.brake();
      pan.rightDrop();

      rightPassengers = 0;
      path.passengers(leftPassengers, rightPassengers);

      pan.rightUp();
    }

    findPath = true;
    t = path.turn();

    switch (t)
    {
      case 0: drive.reverse();
        break;
      case 1: drive.left();
        break;
      case 2: drive.forward();
        break;
      case 3: drive.right();
        break;
      case 4: drive.uturn();
        break;
    }

    path.update();

    if ( path.nearDrop() ) drive.prepareDrop();
    if ( path.nearEndpoint() ) drive.prepareEndpoint();

    //drive.go();
  }
  else if ( findPath )
  {
    n = path.find();
    findPath = false;
  }

  path.stats();
  //drive.go();
}


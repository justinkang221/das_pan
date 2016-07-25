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

uint8_t theta = 0;
uint8_t phi = 0;
uint8_t c = 0;
boolean stop = false;
boolean left, right;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600);

  arm.center();
  pan.leftUp();
  pan.rightUp();
}

void loop()
{
  menu("drive");
  while ( !startbutton() );
  while ( !stopbutton() )
  {
    drive.stats(false);
  }

  menu("passenger");
  while ( !startbutton() );
  while ( !stopbutton() )
  {
    passenger.stats();
  }

  menu("arm");
  while ( !startbutton() );
  while ( !stopbutton() )
  {
    theta = map( knob(6), 0, 1023, 0, 180 );

    RCServo0.write(theta);

    if ( c > 30 )
    {
      LCD.clear();
      LCD.home();
      
      LCD.print("arm position: ");
      
      LCD.setCursor(0, 1);
      
      LCD.print(theta);
      
      c = 0;
    }

    ++c;
  }

  menu("pans");
  while ( !startbutton() );
  while ( !stopbutton() )
  {
    theta = map( knob(6), 0, 1023, 0, 180 );
    phi = map( knob(7), 0, 1023, 0, 180 );

    RCServo1.write(theta);
    RCServo2.write(phi);

    if ( c > 30 )
    {
      LCD.clear();
      LCD.home();

      LCD.print("left pan: ");
      LCD.print(theta);

      LCD.setCursor(0, 1);

      LCD.print("right pan: ");
      LCD.print(phi);

      c = 0;
    }

    ++c;
  }

  menu("collisions");
  while ( !startbutton() );
  while ( !stopbutton() )
  {
    drive.stats(true);
  }

  menu("path");
  while ( !startbutton() );
  Path path;
  path.find();
  delay(500);
  while ( !stopbutton() )
  {
    path.stats();
    while ( !startbutton() ) {
      if ( stopbutton() ) {
        stop = true;
        break;
      }
    }
    if (stop) {
      stop = false;
      break;
    }
    path.update();
    path.find();
  }
}

void menu(String name)
{
  LCD.clear();
  LCD.home();
  LCD.print("test " + name);
  LCD.setCursor(0, 1);
  LCD.print("press <start>");
}


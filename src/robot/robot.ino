#include <phys253.h>
#include <LiquidCrystal.h>

#include <pan.h>
#include <arm.h>
#include <drive.h>
#include <path.h>
#include <passenger.h>
//#include a high five to Ryan when you read this :-)

Arm arm;
Drive drive;
Pan pan;
Passenger passenger;
Path path;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();
  LCD.home();
  LCD.print("    das_pan*    ");

  pan.leftUp();
  pan.rightUp();
  arm.center();
  while (!startbutton());
  drive.resetStuck();
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
boolean rebounded = false;

unsigned long ii = 0;
void lorp() {
  passenger.stats(false);
}
void loop()
{
  drive.go();
  drive.stuck();

  if ( drive.atIntersection() ) {
    seen = drive.describeIntersection();
    /*if (drive.offBoard() && !lost && path.getDirections() != 7) {
      drive.burstBack();
      }*/
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
          if ( leftPassengers || rightPassengers ) {
            reverseToDrop(true);
          }
          else {
            lost = false;
            LCD.setCursor(0, 1);
            LCD.print("    found 15    ");
            path.reorient(15);
            expected = path.getDirections();
            seen = expected;
            t = path.turn();
            path.find();
            path.update();
          }
        }
        else if ( leftD > 110 && rightD > 110 && seen == 6 )  {
          if ( leftPassengers || rightPassengers ) {
            reverseToDrop(false);
          }
          else {
            lost = false;
            LCD.setCursor(0, 1);
            LCD.print("    found 10    ");
            path.reorient(10);
            expected = path.getDirections();
            seen = expected;
            t = path.turn();
            path.find();
            path.update();
          }
        }
        else if ( leftD < 80 && rightD > 100 && seen == 6 ) {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("    found 13    ");
          /*drive.brake();
            while (!startbutton());*/
          path.reorient(13);
          expected = path.getDirections();
          seen = expected;
          t = path.turn();
          path.find();
          path.update();
        }
        else if ( leftD > 100 && rightD < 80 && seen == 3 ) {
          lost = false;
          LCD.setCursor(0, 1);
          LCD.print("    found 12    ");
          /*drive.brake();
            while (!startbutton());*/
          path.reorient(12);
          expected = path.getDirections();
          seen = expected;
          t = path.turn();
          path.find();
          path.update();
        }
      }

      if ( lost || ( seen != expected  && n != 7 ) ) {
        lost = true;
        LCD.setCursor(0, 1);
        //LCD.print("      lost      ");
        /*LCD.print(expected);
          LCD.print(seen);
          drive.brake();
          while ( !startbutton() );*/
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
    drive.resetStuck();
  }
  else {
    corner = drive.intersection();
    crash = drive.collision();
    /*if (drive.offBoard() && !lost && path.getDirections() != 7) {
      drive.burstBack();
      }*/
    if ( passenger.coarse() == 1 && !drive.isIracked()) {
      /*drive.brake();
        while( !startbutton() ) passenger.stats(false);*/
      if ( !leftPassengers ) {
        drive.setIrack(6);
        pickUp(true);
      }
      /*else if ( !rightPassengers ) {
        drive.uturn(true);

        drive.reverse();

        distance = drive.getDistance();
        while ( drive.getDistance() < distance + 12 ) {
          drive.go();
        }
        drive.reverse();
        pickUp(false);
        drive.uturn(true);
        }*/
      drive.resetStuck();
    }
    else if ( passenger.coarse() == 2 && !drive.isIracked()) {
      /*drive.brake();
        while( !startbutton() ) passenger.stats(false);*/
      if ( !rightPassengers ) {
        drive.setIrack(6);
        pickUp(false);
      }
      /*else if ( !leftPassengers ) {
        drive.uturn(false);

        drive.reverse();

        distance = drive.getDistance();
        while ( drive.getDistance() < distance + 12 ) {
          drive.go();
        }
        drive.reverse();

        pickUp(true);
        drive.uturn(false);
        }*/
      drive.resetStuck();
    }

    if ( corner || crash ) {
      if ( !lost ) {
        if ( n == -1 ) {
          if (leftPassengers) {
            drive.brake();
            pan.leftDrop();
            delay(1000);

            leftPassengers = 0;

            pan.leftUp();
          }

          if (rightPassengers) {
            drive.uturn(true);
            drive.brake();
            pan.rightDrop();
            delay(1000);

            rightPassengers = 0;

            pan.rightUp();
            delay(500);
            drive.resetDistance();
            while (drive.getDistance() < 8) {
              drive.go();
            }
            drive.uturn(false);
          }

          path.passengers(leftPassengers + rightPassengers);
        }
        else if ( n == -2 ) {
          if ( rightPassengers )
          {
            drive.brake();
            pan.rightDrop();
            delay(1000);

            rightPassengers = 0;

            pan.rightUp();
          }

          if ( leftPassengers )
          {
            drive.uturn(false);
            drive.brake();
            pan.leftDrop();
            delay(1000);

            leftPassengers = 0;

            pan.leftUp();
            delay(500);
            drive.resetDistance();
            while (drive.getDistance() < 8) {
              drive.go();
            }
            drive.uturn(true);
          }

          path.passengers(leftPassengers + rightPassengers);
          drive.resetStuck();
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
            /*path.stats();
              drive.brake();
              while ( !startbutton() );*/
            ii = millis();
            while (millis() - ii < 1000) {
              drive.go();
            }
            path.avoid();
            /*path.stats();
              while ( !startbutton() );*/
            drive.uturn(true);
            n = path.find();
            expected = path.getDirections();
            /*path.stats();
              while ( !startbutton() );*/
          }

        }
        t = path.turn();
        path.update();
        drive.record(true);
      }

      else if (crash) {
        drive.uturn(true);
      }
      drive.resetStuck();
    }
  }
}

void pickUp(boolean left) {
  drive.speed(50);
  distance = drive.getDistance();
  while ( drive.getDistance() < distance + 2 ) {
    drive.go();
    if (drive.intersection()) {
      corner = true;
      return;
    }
  }
  if (left) {
    while ( drive.getDistance() < distance + 4 ) {
      drive.go();
      if (drive.intersection()) {
        corner = true;
        return;
      }
      if ( passenger.precise(true) == 1) {
        drive.brake();

        arm.leftCenter();
        delay(500);
        pan.leftPick();
        arm.cycle();

        if ( !passenger.precise(true) ) {
          ii = millis();
          rebounded = false;
          while ( millis() - ii < 500 ) {
            if ( passenger.precise(true) ) rebounded = true;
          }
          if (!rebounded) {
            pan.leftUp();
            leftPassengers = true;
            arm.center();
            break;
          }
        }

        arm.leftFront();
        arm.cycle();

        if ( !passenger.precise(true) ) {
          ii = millis();
          rebounded = false;
          while ( millis() - ii < 500 ) {
            if ( passenger.precise(true) ) rebounded = true;
          }
          if (!rebounded) {
            pan.leftUp();
            leftPassengers = true;
            arm.center();
            break;
          }
        }

        arm.leftBack();
        arm.cycle();
        pan.leftUp();
        if ( !passenger.precise(true) ) leftPassengers = true;
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
      if (drive.intersection()) {
        corner = true;
        return;
      }
      //passenger.stats();
      if (passenger.precise(false) == 2) {
        drive.brake();

        arm.rightCenter();
        delay(500);
        pan.rightPick();
        arm.cycle();
        if ( !passenger.precise(false) ) {
          ii = millis();
          rebounded = false;
          while ( millis() - ii < 500 ) {
            if ( passenger.precise(false) ) rebounded = true;;
          }
          if (!rebounded) {
            pan.rightUp();
            rightPassengers = true;
            arm.center();
            break;
          }
        }

        arm.rightFront();
        arm.cycle();

        if ( !passenger.precise(false) ) {
          ii = millis();
          rebounded = false;
          while ( millis() - ii < 500 ) {
            if ( passenger.precise(false) ) rebounded = true;;
          }
          if (!rebounded) {
            pan.rightUp();
            rightPassengers = true;
            arm.center();
            break;
          }
        }

        arm.rightBack();
        arm.cycle();
        pan.rightUp();
        if ( !passenger.precise(true) ) leftPassengers = true;
        arm.center();
        break;
      }
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

void reverseToDrop(boolean comingFrom15) {
  drive.reverse();
  drive.resetDistance();
  while (drive.getDistance() > -30) {
    drive.go();
  }
  if (comingFrom15) {
    drive.reverse();
    if (rightPassengers) {
      drive.brake();
      pan.rightDrop();
      delay(1000);

      rightPassengers = 0;

      pan.rightUp();
      delay(500);
    }

    if (leftPassengers) {
      drive.resetDistance();
      while (drive.getDistance() < 4) {
        drive.go();
      }
      drive.uturn(false);
      drive.brake();
      pan.leftDrop();
      delay(1000);

      leftPassengers = 0;

      pan.leftUp();
      delay(500);
      drive.resetDistance();
      while (drive.getDistance() < 8) {
        drive.go();
      }
      drive.uturn(true);
    }

    path.passengers(leftPassengers + rightPassengers);
  }
  else {
    drive.reverse();
    if (leftPassengers) {
      drive.brake();
      pan.leftDrop();
      delay(1000);

      leftPassengers = 0;

      pan.leftUp();
      delay(500);
    }

    if (rightPassengers) {
      drive.resetDistance();
      while (drive.getDistance() < 4) {
        drive.go();
      }
      drive.uturn(true);
      drive.brake();
      pan.rightDrop();
      delay(1000);

      rightPassengers = 0;

      pan.rightUp();
      delay(500);
      drive.resetDistance();
      while (drive.getDistance() < 8) {
        drive.go();
      }
      drive.uturn(false);
    }

    path.passengers(leftPassengers + rightPassengers);
  }
  drive.resetStuck();
  while (!drive.intersection()) {
    drive.go();
  }
  drive.resetStuck();

  drive.setOldDistance(111, 111);
  /*drive.resetIntersection();

    while (drive.getDistance() < 10) {
    drive.go();
    }*/
}


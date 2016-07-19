/*
 * Passenger.cpp - Library for detecting passengers.
 */

#include <passenger.h>
#include <phys253.h>
#include <Arduino.h>

#define _leftIR1 (1)
#define _leftIR2 (3)
#define _rightIR1 (0)
#define _rightIR2 (2)

#define _threshold (250)
#define _precise (500)

Passenger::Passenger()
{
    _c = 0;
}

uint8_t Passenger::detect(void)
{
    _left = analogRead(_leftIR1);
    _right = analogRead(_rightIR1);
    if (_left > _threshold) {
        return 1;
    }
    else if (_right > _threshold) {
        return 2;
    }
    else return 0;
}

uint8_t Passenger::precise(void)
{
    _left = analogRead(_leftIR2);
    _right = analogRead(_rightIR2);
    if (_left > _precise) {
        return 1;
    }
    else if (_right > _precise) {
        return 2;
    }
    else return 0;
}

void Passenger::stats(void)
{
    if (_c > 30) {
        LCD.clear();
        LCD.home();
        LCD.print(analogRead(_leftIR1));
        LCD.print("  ");
        LCD.print(analogRead(_rightIR1));
        LCD.setCursor(0,1);
        LCD.print(analogRead(_leftIR2));
        LCD.print("  ");
        LCD.print(analogRead(_rightIR2));
        _c = 0;
    }
    ++_c;
}

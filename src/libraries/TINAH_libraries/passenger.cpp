/*
 * Passenger.cpp - Library for detecting passengers.
 */

#include <passenger.h>
#include <phys253.h>
#include <Arduino.h>

#define _leftIR1 (3)
#define _leftIR2 (1)
#define _rightIR1 (2)
#define _rightIR2 (0)

#define _l1 (0)
#define _l2 (1)
#define _r1 (2)
#define _r2 (3)

#define _threshold (200)
#define _precise (500)
#define _coarse (150)

Passenger::Passenger(void)
{
    _l = 0;
    _r = 0;
    
    _c = 0;
}

uint8_t Passenger::detect(void)
{
    if (analogRead(_leftIR1) > _threshold && analogRead(_leftIR1) > _threshold && analogRead(_leftIR1) > _threshold && analogRead(_leftIR1) > _threshold && analogRead(_leftIR1) > _threshold) return 1;
    else if(analogRead(_rightIR1) > _threshold && analogRead(_rightIR1) > _threshold && analogRead(_rightIR1) > _threshold && analogRead(_rightIR1) > _threshold && analogRead(_rightIR1) > _threshold) return 2;
    return 0;
}

uint8_t Passenger::precise(void)
{
    if (analogRead(_leftIR2) > _precise && analogRead(_leftIR2) > _precise && analogRead(_leftIR2) > _precise && analogRead(_leftIR2) > _precise && analogRead(_leftIR2) > _precise) return 1;
    else if(analogRead(_rightIR2) > _precise && analogRead(_rightIR2) > _precise && analogRead(_rightIR2) > _precise && analogRead(_rightIR2) > _precise && analogRead(_rightIR2) > _precise) return 2;
    return 0;
}

uint8_t Passenger::coarse(void)
{
    if (analogRead(_leftIR2) > _coarse && analogRead(_leftIR2) > _coarse && analogRead(_leftIR2) > _coarse && analogRead(_leftIR2) > _coarse && analogRead(_leftIR2) > _coarse) return 1;
    else if(analogRead(_rightIR2) > _coarse && analogRead(_rightIR2) > _coarse && analogRead(_rightIR2) > _coarse && analogRead(_rightIR2) > _coarse && analogRead(_rightIR2) > _coarse) return 2;
    return 0;
}

void Passenger::stats(void)
{
    if (_c > 30) {
        LCD.clear();
        LCD.home();
        
        LCD.print("L: ");
        LCD.print(analogRead(_leftIR1));
        LCD.print(" R: ");
        LCD.print(analogRead(_rightIR1));
        
        LCD.setCursor(0,1);
        
        LCD.print("l: ");
        LCD.print(analogRead(_leftIR2));
        LCD.print(" r: ");
        LCD.print(analogRead(_rightIR2));
        
        _c = 0;
    }
    
    ++_c;
}

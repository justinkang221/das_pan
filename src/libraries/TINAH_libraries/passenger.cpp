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

#define _l1 (0)
#define _l2 (1)
#define _r1 (2)
#define _r2 (3)

#define _threshold (250)
#define _precise (500)

Passenger::Passenger(void)
{
    _l = 0;
    _r = 0;
    
    _c = 0;
}

uint8_t Passenger::detect(void)
{
    _left = analogRead(_leftIR1);
    _right = analogRead(_rightIR1);
    
    return (_left > _threshold) ? 1 : ((_right > _threshold) ? 2 : 0);
}

uint8_t Passenger::precise(void)
{
    _left = analogRead(_leftIR2);
    _right = analogRead(_rightIR2);
    
    return (_left > _precise) ? 1 : ((_right > _precise) ? 2 : 0);
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

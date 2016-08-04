/*
 * Passenger.cpp - Library for detecting passengers.
 */

#include <passenger.h>
#include <phys253.h>
#include <Arduino.h>

#define LEFT_IR1 (1)
#define LEFT_IR2 (3)
#define RIGHT_IR1 (2)
#define RIGHT_IR2 (0)

#define THRESHOLD (200)
#define PRECISE_LEFT (600)
#define PRECISE_RIGHT (660)
#define COARSE (350)

Passenger::Passenger(void)
{
    _l = 0;
    _r = 0;
    
    _c = 0;
}

uint8_t Passenger::detect(void)
{
    if (analogRead(LEFT_IR1) > THRESHOLD && analogRead(LEFT_IR1) > THRESHOLD && analogRead(LEFT_IR1) > THRESHOLD && analogRead(LEFT_IR1) > THRESHOLD && analogRead(LEFT_IR1) > THRESHOLD) return 1;
    else if(analogRead(RIGHT_IR1) > THRESHOLD && analogRead(RIGHT_IR1) > THRESHOLD && analogRead(RIGHT_IR1) > THRESHOLD && analogRead(RIGHT_IR1) > THRESHOLD && analogRead(RIGHT_IR1) > THRESHOLD) return 2;
    return 0;
}

uint8_t Passenger::precise(boolean left)
{
    if ( left && ( analogRead(LEFT_IR2) > PRECISE_LEFT && analogRead(LEFT_IR2) > PRECISE_LEFT && analogRead(LEFT_IR2) > PRECISE_LEFT && analogRead(LEFT_IR2) > PRECISE_LEFT && analogRead(LEFT_IR2) > PRECISE_LEFT ) ) return 1;
    else if( ! left && ( analogRead(RIGHT_IR2) > PRECISE_RIGHT && analogRead(RIGHT_IR2) > PRECISE_RIGHT && analogRead(RIGHT_IR2) > PRECISE_RIGHT && analogRead(RIGHT_IR2) > PRECISE_RIGHT && analogRead(RIGHT_IR2) > PRECISE_RIGHT ) ) return 2;
    return 0;
}

uint8_t Passenger::coarse(void)
{
    if (analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE && analogRead(LEFT_IR2) > COARSE) return 1;
    else if(analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE && analogRead(RIGHT_IR2) > COARSE) return 2;
    return 0;
}

void Passenger::stats(boolean printEveryTime)
{
    if (_c > 30 || printEveryTime) {
        LCD.clear();
        LCD.home();
        
        LCD.print("L: ");
        LCD.print(analogRead(LEFT_IR1));
        LCD.print(" R: ");
        LCD.print(analogRead(RIGHT_IR1));
        
        LCD.setCursor(0,1);
        
        LCD.print("l: ");
        LCD.print(analogRead(LEFT_IR2));
        LCD.print(" r: ");
        LCD.print(analogRead(RIGHT_IR2));
        
        _c = 0;
    }
    
    ++_c;
}

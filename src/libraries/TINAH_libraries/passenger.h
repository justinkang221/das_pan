/*
 * Passenger.h - Library for detecting passengers.
 */

#ifndef passenger_h
#define passenger_h

#include <Arduino.h>

class Passenger
{
public:
    Passenger(uint8_t, uint8_t, uint8_t, uint8_t);
    uint8_t detect(void);
    uint8_t precise(void);
    void stats(void);
private:
    uint8_t _leftIR1;
    uint8_t _left;
    uint8_t _leftIR2;
    uint8_t _leftPrecise;
    
    uint8_t _rightIR1;
    uint8_t _right;
    uint8_t _rightIR2;
    uint8_t _rightPrecise;
    
    uint8_t _threshold;
    uint8_t _precise;
    
    uint8_t _c;
};

#endif

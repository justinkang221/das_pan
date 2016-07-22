/*
 * Passenger.h - Library for detecting passengers.
 */

#ifndef passenger_h
#define passenger_h

#include <Arduino.h>

class Passenger
{
public:
    Passenger(void);
    uint8_t detect(void);
    uint8_t precise(void);
    void stats(void);
private:
    uint8_t _l;
    uint8_t _r;
    
    uint8_t _c;
};

#endif
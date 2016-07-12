/*
 * Arm.h - Library for actuating arm.
 * Created by Rahat Dhande, July 8, 2016.
 */
#ifndef Arm_h
#define Arm_h

#include <Arduino.h>

class Arm
{
public:
    Arm(uint8_t, uint8_t, uint8_t);
    void left();
    void center();
    void right();
    void cycle();
private:
    uint8_t _motor;
    uint8_t _pot;
    uint8_t _speed;
};

#endif

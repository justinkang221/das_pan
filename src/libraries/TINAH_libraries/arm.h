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
    Arm(void);
    void left();
    void center();
    void right();
    void cycle();
};

#endif

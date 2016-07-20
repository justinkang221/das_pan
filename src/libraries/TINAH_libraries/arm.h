/*
 * Arm.h - Library for actuating arm.
 */

#ifndef Arm_h
#define Arm_h

#include <Arduino.h>

class Arm
{
public:
    Arm(void);
    void left(void);
    void center(void);
    void right(void);
    void cycle(void);
};

#endif

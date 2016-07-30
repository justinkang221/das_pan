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
    void leftFront(void);
    void leftBack(void);
	void leftCenter(void);
    void center(void);
    void rightFront(void);
    void rightBack(void);
	void rightCenter(void);
    void cycle(void);
};

#endif

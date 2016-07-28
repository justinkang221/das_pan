/*
 * Arm.cpp - Library for actuating arm.
 */

#include <arm.h>
#include <phys253.h>
#include <Arduino.h>

#define _motor (37)
#define _pot (11)
#define _speed (135)

Arm::Arm(void) {}

void Arm::leftFront(void)
{
    RCServo0.write(176);
    delay(1000);
}

void Arm::leftBack(void)
{
    RCServo0.write(155);
    delay(1000);
}

void Arm::center(void)
{
    RCServo0.write(70);
}

void Arm::rightFront(void)
{
    RCServo0.write(1);
    delay(1000);
}

void Arm::rightBack(void)
{
    RCServo0.write(10);
    delay(1000);
}

void Arm::cycle(void)
{
    analogWrite(_motor, _speed);
    
    while(digitalRead(_pot)==HIGH);
    delay(1);
    while(digitalRead(_pot)==HIGH);
    
    while(digitalRead(_pot)==LOW);
    delay(1);
    while(digitalRead(_pot)==LOW);
    
    delay(200);
    analogWrite(_motor, 0);
}

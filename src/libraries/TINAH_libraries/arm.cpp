/*
 * Arm.cpp - Library for actuating arm.
 */

#include <arm.h>
#include <phys253.h>
#include <Arduino.h>

#define MOTOR (37)
#define POT (A4)
#define SPEED (180)

Arm::Arm(void) {}

void Arm::leftFront(void)
{
    RCServo0.write(180);
}

void Arm::leftBack(void)
{
    RCServo0.write(170);
}

void Arm::leftCenter(void)
{
	RCServo0.write(177);
}

void Arm::center(void)
{
    RCServo0.write(80);
    delay(500);
}

void Arm::rightFront(void)
{
    RCServo0.write(1);
}

void Arm::rightBack(void)
{
    RCServo0.write(13);
}

void Arm::rightCenter(void)
{
	RCServo0.write(7);
}

void Arm::cycle(void)
{
    analogWrite(MOTOR, SPEED);
    
    while(digitalRead(POT)==HIGH);
    delay(1);
    while(digitalRead(POT)==HIGH);
    
    while(digitalRead(POT)==LOW);
    delay(1);
    while(digitalRead(POT)==LOW);
    
    delay(200);
    analogWrite(MOTOR, 0);
}

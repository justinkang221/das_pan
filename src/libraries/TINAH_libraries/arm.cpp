/*
 * Arm.cpp - Library for actuating arm.
 */

#include <arm.h>
#include <phys253.h>
#include <Arduino.h>

#define MOTOR (37)
#define POT (A4)
#define SPEED (150)

Arm::Arm(void) {}

void Arm::leftFront(void)
{
    RCServo0.write(169);
    delay(1000);
}

void Arm::leftBack(void)
{
    RCServo0.write(161);
    delay(1000);
}

void Arm::leftCenter(void)
{
	RCServo0.write(165);
	delay(1000);
}

void Arm::center(void)
{
    RCServo0.write(70);
    delay(500);
}

void Arm::rightFront(void)
{
    RCServo0.write(3);
    delay(1000);
}

void Arm::rightBack(void)
{
    RCServo0.write(11);
    delay(1000);
}

void Arm::rightCenter(void)
{
	RCServo0.write(7);
	delay(1000);
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

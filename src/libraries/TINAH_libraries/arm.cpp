/*
 * Arm.cpp - Library for actuating arm.
 * Created by Rahat Dhande, July 8, 2016.
 */

#include <arm.h>
#include <phys253.h>
#include <Arduino.h>

#define _motor (37)
#define _pot (0)
#define _speed (135)

Arm::Arm(){}

void Arm::left()
{
    RCServo0.write(179);
    delay(1000);
}

void Arm::right()
{
    RCServo0.write(1);
    delay(1000);
}

void Arm::center()
{
    RCServo0.write(90);
    delay(1000);
}

void Arm::cycle()
{
    analogWrite(_motor, _speed);
    while(digitalRead(_pot)==HIGH);
    while(digitalRead(_pot)==LOW);
    delay(200);
    analogWrite(_motor, 0);
}


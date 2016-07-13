/*
 * Arm.cpp - Library for actuating arm.
 * Created by Rahat Dhande, July 8, 2016.
 */

#include <arm.h>
#include <phys253.h>
#include <Arduino.h>

Arm::Arm(uint8_t motor, uint8_t pot, uint8_t speed)
{
    _motor = motor;
    _pot = pot;
    _speed = speed;
}

void Arm::left()
{
    RCServo0.write(1);
    delay(1000);
}

void Arm::right()
{
    RCServo0.write(179);
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
    while(digitalRead(_pot)==HIGH) { delay(10); }
    while(digitalRead(_pot)==LOW) { delay(10); }
    analogWrite(_motor, 0);
}


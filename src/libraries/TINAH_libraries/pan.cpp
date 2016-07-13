/*
 * Pan.cpp - library for actuating pans.
 * Created by Rahat Dhande, July 8, 2016.
 */
#include <pan.h>
#include <phys253.h>
#include <Arduino.h>

Pan::Pan() {}

void Pan::leftUp()
{
    RCServo2.write(170);
}

void Pan::leftDown()
{
    RCServo2.write(30);
}

void Pan::rightUp()
{
    RCServo1.write(170);
}

void Pan::rightDown()
{
    RCServo1.write(30);
}

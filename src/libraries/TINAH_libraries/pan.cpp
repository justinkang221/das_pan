/*
 * Pan.cpp - library for actuating pans.
 * Created by Rahat Dhande, July 8, 2016.
 */
#include <pan.h>
#include <phys253.h>
#include <Arduino.h>

Pan::Pan()
{
}

void Pan::leftUp()
{
    RCServo1.write(0);
}

void Pan::leftDown()
{
    RCServo1.write(180);
}

void Pan::rightUp()
{
    RCServo2.write(0);
}

void Pan::rightDown()
{
    RCServo2.write(180);
}

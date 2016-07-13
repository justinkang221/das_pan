/*
 * Pan.cpp - library for actuating pans.
 * Created by Rahat Dhande, July 8, 2016.
 */
#include <pan.h>
#include <phys253.h>
#include <Arduino.h>

Pan::Pan() {}

void Pan::leftUp(void)
{
    RCServo2.write(170);
}

void Pan::leftPick(void)
{
    RCServo2.write(30);
}

void Pan::leftDrop(void)
{
    RCServo2.write(0);
}

void Pan::rightUp(void)
{
    RCServo1.write(170);
}

void Pan::rightPick(void)
{
    RCServo1.write(30);
}

void Pan::rightDrop(void)
{
    RCServo1.write(0);
}

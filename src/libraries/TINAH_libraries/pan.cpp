/*
 * Pan.cpp - library for actuating pans.
 */

#include <pan.h>
#include <phys253.h>
#include <Arduino.h>

Pan::Pan(void) {}

void Pan::leftUp(void)
{
    RCServo1.write(7);
}

void Pan::leftPick(void)
{
    RCServo1.write(86);
}

void Pan::leftDrop(void)
{
    RCServo1.write(130);
}

void Pan::rightUp(void)
{
    RCServo2.write(180);
}

void Pan::rightPick(void)
{
    RCServo2.write(65);
}

void Pan::rightDrop(void)
{
    RCServo2.write(25);
}

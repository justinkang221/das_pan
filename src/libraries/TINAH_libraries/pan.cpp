/*
 * Pan.cpp - library for actuating pans.
 */

#include <pan.h>
#include <phys253.h>
#include <Arduino.h>

#define _pocket1 (10)
#define _pocket2 (11)
#define _pocket3 (8) 
#define _pocket4 (9) 

Pan::Pan(void) {}

void Pan::leftUp(void)
{
    RCServo1.write(1);
}

void Pan::leftPick(void)
{
    RCServo1.write(80);
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

boolean Pan::leftFull(boolean _backPocket)
{
	if (_backPocket) return !(digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1) || digitalRead(_pocket1));
	else return !(digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2) || digitalRead(_pocket2));
}

boolean Pan::rightFull(boolean _backPocket)
{
	if (_backPocket) return !(digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3) || digitalRead(_pocket3));
	else return !(digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4) || digitalRead(_pocket4));
}

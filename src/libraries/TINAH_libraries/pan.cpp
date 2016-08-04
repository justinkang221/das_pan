/*
 * Pan.cpp - library for actuating pans.
 */

#include <pan.h>
#include <phys253.h>
#include <Arduino.h>

#define POCKET1 (10)
#define POCKET2 (11)
#define POCKET3 (8) 
#define POCKET4 (9) 

Pan::Pan(void) {}

void Pan::leftUp(void)
{
    RCServo1.write(30);
}

void Pan::leftPick(void)
{
    RCServo1.write(115);
}

void Pan::leftDrop(void)
{
    RCServo1.write(155);
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
    RCServo2.write(30);
}

boolean Pan::leftFull(void)
{
	return !( digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) || digitalRead(POCKET1) )
    
    ||
    
    !( digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) || digitalRead(POCKET2) );
}

boolean Pan::rightFull(void)
{
	return !( digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) || digitalRead(POCKET3) )
    
    ||
    
    !(digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) || digitalRead(POCKET4) );
}

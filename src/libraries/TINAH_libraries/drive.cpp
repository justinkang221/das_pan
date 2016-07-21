/*
 * Drive.cpp - Library for driving robot.
 */

#include <drive.h>
#include <phys253.h>
#include <LiquidCrystal.h>
#include <Arduino.h>

#define _m1 (2)
#define _m2 (1)

#define _qrd1 (0)
#define _qrd2 (1)
#define _qrd3 (2)
#define _qrd4 (3)
#define _qrd5 (4)
#define _qrd6 (5)

#define _turnSpeed (150)
#define _tightness (-35)

#define _threshold (500)

#define _col1 (15)
#define _col2 (14)
#define _col3 (12)
#define _col4 (13)

Drive::Drive(void)
{
    _speed = 150;
    
    _kp = 12;
    _kd = 35;
    
    _p = 0;
    _d = 0;
    _correction = 0;
    
    _error = 0;
    _lastError = 0;
    _recentError = 0;
    
    _c = 0;
    _q = 0;
    _m = 0;
    
    _hack = 0;
    _sack = 0;
    
    _backing = false;
}

void Drive::setPD(uint8_t kp, uint8_t kd)
{
    _kp = kp;
    _kd = kd;
}

void Drive::go(void)
{
    // TODO: test this
    if (_backing) {
        _left = digitalRead(_qrd5);
        _right = digitalRead(_qrd6);
        
        if (_left) {
            if (_right) _error = 0;
            else _error = -1;
        }
        else {
            if (_right) _error = 1;
            else if (_lastError > 0) _error = 5;
            else _error = -5;
        }
        
        if (_error != _lastError) {
            _recentError = _lastError;
            _q = _m;
            _m = 1;
        }
        
        _p = _kp * _error;
        _d = _kd * (_error - _recentError) / (_q + _m);
        _correction = _p + _d;
        
        motor.speed(_m2, -1 * (100 + _correction));
        motor.speed(_m1, -1 * (100 - _correction));
        _lastError = _error;
    }
    else {
        _left = digitalRead(_qrd1);
        _right = digitalRead(_qrd2);
        
        if (_left) {
            if (_right) _error = 0;
            else _error = -1;
        }
        else {
            if (_right) _error = 1;
            else if (_lastError > 0) _error = 5;
            else _error = -5;
        }
        
        if (_error != _lastError) {
            _recentError = _lastError;
            _q = _m;
            _m = 1;
        }
        
        _p = _kp * _error;
        _d = _kd * (_error - _recentError) / (_q + _m);
        _correction = _p + _d;
        ++_m;
        
        motor.speed(_m1, _speed + _correction);
        motor.speed(_m2, _speed - _correction + 10);
        _lastError = _error;
    }
}

void Drive::left(void)
{
    motor.speed(_m1, _speed);
    motor.speed(_m2, _speed);
    delay(150);
    
    if (_backing) {
        motor.speed(_m2, _tightness*_turnSpeed/100);
        motor.speed(_m1, _turnSpeed);
        delay(400);
        _lastError = -5;
    }
    else {
        motor.speed(_m1, _tightness*_turnSpeed/100);
        motor.speed(_m2, _turnSpeed);
        delay(400);
        _lastError = 5;
    }
    _error = 0;
    _recentError = 0;
    
    while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
    _backing = false;
}

void Drive::straight(void)
{
    _sack = 500;
}

void Drive::right(void)
{
    motor.speed(_m1, _speed);
    motor.speed(_m2, _speed);
    delay(150);
    
    if (_backing) {
        motor.speed(_m2, _turnSpeed);
        motor.speed(_m1, _tightness*_turnSpeed/100);
        delay(400);
        _lastError = 5;

    }
    else {
        motor.speed(_m1, _turnSpeed);
        motor.speed(_m2, _tightness*_turnSpeed/100);
        delay(400);
        _lastError = -5;
    }
    _error = 0;
    _recentError = 0;
    
    while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
    _backing = false;
}

void Drive::reverse(void)
{
    _backing = !_backing;
}

void Drive::uturn(void)
{
    motor.speed(_m1, -200);
    motor.speed(_m2, -50);
    delay(1200);
    
    motor.speed(_m1, 0);
    motor.speed(_m2, 200);
    while (!digitalRead(_qrd1));
}

void Drive::prepareDrop(void)
{
    _hack = 70000 / _speed;
}

void Drive::prepareEndpoint(void)
{
    _hack = 15000 / _speed;
}

boolean Drive::intersection()
{
    // TODO: get rid of _inter
    if (_hack) --_hack;
    if(_hack == 1){
        return true;
    }
    else if ( !_sack && ( (digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3)) || (digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4)) ) ) {
        return true;
    }
    
    if (_sack) --_sack;
    
    return false;
    
    //return (analogRead(_qrd3) > _threshold || analogRead(_qrd4) > _threshold || (_hack && _hack-- == 1));
    //return ( (!_sack) && (analogRead(_qrd3) > _threshold || analogRead(_qrd4) > _threshold || (_hack && _hack-- == 1)) );
}

uint8_t Drive::collision(void)
{
    return ( !((digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1))) + 2 * !(digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2)) + 4 * !(digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3)) + 8 * !(digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4)));
}

void Drive::speed(int16_t speed)
{
    _speed = speed;
}

void Drive::brake(void)
{
    motor.stop_all();
    //delay(500);
}

void Drive::stats(void)
{
    if (_c > 30) {
        LCD.clear();
        LCD.home();
        
        LCD.print("L: ");
        LCD.print(digitalRead(_qrd3));
        LCD.print(" R: ");
        LCD.print(digitalRead(_qrd4));
        
        LCD.setCursor(0,1);
        
        if (_backing) {
            LCD.print("l: ");
            LCD.print(analogRead(_qrd5));
            LCD.print(" r: ");
            LCD.print(analogRead(_qrd6));
        }
        else {
            LCD.print("l: ");
            LCD.print(digitalRead(_qrd1));
            LCD.print(" r: ");
            LCD.print(digitalRead(_qrd2));
        }
        
        _c = 0;
    }
    
    ++_c;
}



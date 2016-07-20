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
#define _qrd3 (4)
#define _qrd4 (5)
#define _qrd5 (4)
#define _qrd6 (5)

#define _turnSpeed (150)
#define _tightness (-70)

#define _threshold (500)

#define _col1 (0)
#define _col2 (1)
#define _col3 (2)
#define _col4 (3)

Drive::Drive(void)
{
    _speed = 100;
    
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
    _inter = false;
}

void Drive::setPD(uint8_t kp, uint8_t kd)
{
    _kp = kp;
    _kd = kd;
}

void Drive::go(void)
{
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
    
    _sack && --_sack;
}

void Drive::left(void)
{
    motor.speed(_m1, _speed);
    motor.speed(_m2, _speed);
    delay(150);
    
    if (_backing) {
        motor.speed(_m2, _tightness*_turnSpeed/100);
        motor.speed(_m1, _turnSpeed);
        
    }
    else {
        motor.speed(_m1, _tightness*_turnSpeed/100);
        motor.speed(_m2, _turnSpeed);
        delay(400);
    }
    
    while (!(digitalRead(_qrd1) && !digitalRead(_qrd2)));
    _backing = false;
}

void Drive::forward(void)
{
    _sack = 20;
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

    }
    else {
        motor.speed(_m1, _turnSpeed);
        motor.speed(_m2, _tightness*_turnSpeed/100);
        delay(400);
    }
    
    while (!(digitalRead(_qrd1) && !digitalRead(_qrd2)));
    _backing = false;
}

void Drive::reverse(void)
{
    _backing = true;
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
    
    motor.speed(_m2, -1 * (_speed + _correction));
    motor.speed(_m1, -1 * (_speed - _correction));
    _lastError = _error;
}

void Drive::uturn(void)
{
    motor.speed(_m1, -200);
    motor.speed(_m2, -50);
    delay(1200);
    
    motor.speed(_m1, 0);
    motor.speed(_m2, 200);
    while (digitalRead(_qrd1));
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
    /*if (_hack) --_hack;
    if(_hack == 1){
        return true;
    }
    else if (_sack == 0 && (digitalRead(_qrd3) || digitalRead(_qrd4)) ) {
        if (_inter) {
            _inter = false;
            return true;
        }
        else {
            _inter = true;
            return false;
        }
    }
    _inter = false;
    return false;*/
    
    // TODO: see if we need _sack
    return (analogRead(_qrd3) > _threshold || analogRead(_qrd4) > _threshold || (_hack && _hack-- == 1));
    //return ( (!_sack) && (analogRead(_qrd3) > _threshold || analogRead(_qrd4) > _threshold || (_hack && _hack-- == 1)) );
}

uint8_t Drive::collision(void)
{
    return ( (digitalRead(_col1)) + 2 * (digitalRead(_col2)) + 3 * (digitalRead(_col3)) + 4 * (digitalRead(_col4)) );
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
        LCD.print(analogRead(_qrd3));
        LCD.print(" R: ");
        LCD.print(analogRead(_qrd4));
        
        LCD.setCursor(0,1);
        
        if (_backing) {
            LCD.print("l: ");
            LCD.print(digitalRead(_qrd1));
            LCD.print(" r: ");
            LCD.print(digitalRead(_qrd2));
        }
        else {
            LCD.print("l: ");
            LCD.print(digitalRead(_qrd5));
            LCD.print(" r: ");
            LCD.print(digitalRead(_qrd6));
        }
        
        _c = 0;
    }
    
    ++_c;
}

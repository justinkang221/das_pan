/*
 * Drive.cpp - Library for tape-following
 * Created by Rahat Dhande, July 10, 2016.
 */
 

#include <drive.h>
#include <phys253.h>
#include <LiquidCrystal.h>
#include <Arduino.h>

#define _m1 (2)
#define _m2 (1)

#define _qrd1 (0)
#define _qrd2 (1)
#define _qrd3 (3)
#define _qrd4 (2)
#define _qrd5 (4)
#define _qrd6 (5)

#define _turnSpeed (100)
#define _tightness (-55)

Drive::Drive()
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
}

void Drive::setPD(uint8_t kp, uint8_t kd)
{
    _kp = kp;
    _kd = kd;
}

void Drive::straight()
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
    
    if (_c > 30) {
        LCD.clear();
        LCD.home();
        LCD.print("L: ");
        LCD.print(digitalRead(_qrd3));
        LCD.print(" R: ");
        LCD.print(digitalRead(_qrd4));
        LCD.setCursor(0,1);
        LCD.print("l: ");
        LCD.print(_left);
        LCD.print(" r: ");
        LCD.print(_right);
        _c = 0;
    }
    
    ++_c;
    ++_m;
    
    motor.speed(_m1, _speed + _correction);
    motor.speed(_m2, _speed - _correction + 10);
    _lastError = _error;
}

void Drive::left()
{
    motor.speed(_m1, _speed);
    motor.speed(_m2, _speed);
    delay(100);
    motor.speed(_m1, _tightness*_turnSpeed/100);
    motor.speed(_m2, _turnSpeed);
    delay(300);
    while (digitalRead(_qrd1));
}

void Drive::right()
{
    motor.speed(_m1, _speed);
    motor.speed(_m2, _speed);
    delay(100);
    motor.speed(_m1, _turnSpeed);
    motor.speed(_m2, _tightness*_turnSpeed/100);
    delay(300);
    while (digitalRead(_qrd2));
}

void Drive::reverse()
{
    // TODO: test if it works
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
    
    if (_c > 30) {
        LCD.clear();
        LCD.home();
        LCD.print(-1 * (_speed + _correction));
        LCD.print(-1 * (_speed - _correction));
        LCD.setCursor(0,1);
        LCD.print("l: ");
        LCD.print(_left);
        LCD.print("  r: ");
        LCD.print(_right);
        _c = 0;
    }
    
    ++_c;
    ++_m;
    
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
    while (digitalRead(_qrd1)) {
    }
}

void Drive::prepareDrop(void)
{
    _hack = 300000 / _speed;
}

void Drive::prepareEndpoint(void)
{
    _hack = 200000 / _speed;
}

boolean Drive::intersection()
{
    /*if (_c > 30) {
        LCD.clear();
        LCD.home();
        LCD.print("hack:");
        LCD.print(_hack);
        _c = 0;
    }
    ++_c;*/
    
    if (_hack) --_hack;
    return ((digitalRead(_qrd3) && digitalRead(_qrd3)) || (digitalRead(_qrd4) && digitalRead(_qrd4)) || _hack == 1);
}

void Drive::speed(int16_t speed)
{
    _speed = speed;
}

void Drive::brake()
{
    motor.stop_all();
    //delay(500);
}

void Drive::stats()
{
    if (_c > 30) {
        LCD.setCursor(0,1);
        LCD.print("kp: ");
        LCD.print(_kp);
        LCD.print("  kd: ");
        LCD.print(_kd);
        _c = 0;
    }
    ++_c;
}

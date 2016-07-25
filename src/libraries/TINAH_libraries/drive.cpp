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

#define _wheelL (0) // TODO: change this
#define _wheelR (1)

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
    _cack = 0;
    
    _backing = false;
    
    _black = digitalRead(_wheelL);
    _brack = digitalRead(_wheelR);
}

void Drive::setPD(uint8_t kp, uint8_t kd)
{
    _kp = kp;
    _kd = kd;
}

void Drive::go(void)
{
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

void Drive::left(boolean tight)
{
    // update encoder
    this->wheel(_wheelR);
    
    if (tight) {
        motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        while( !this->wheel(_wheelR) );
        // delay(150);
        
        if (_backing) {
            motor.speed(_m2, 1.5 * _tightness*_turnSpeed/100);
            motor.speed(_m1, _turnSpeed);
            while( !this->wheel(_wheelR) );
            //delay(150);
            _lastError = -5;
        }
        else {
            motor.speed(_m1, 1.5 * _tightness*_turnSpeed/100);
            motor.speed(_m2, _turnSpeed);
            while( !this->wheel(_wheelR) );
            // delay(150);
            _lastError = 5;
        }
        _error = 0;
        _recentError = 0;
        
        while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
        _backing = false;
    }
    else {
        motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        while( !this->wheel(1) );
        // delay(150);
        
        if (_backing) {
            motor.speed(_m2, _tightness*_turnSpeed/100);
            motor.speed(_m1, _turnSpeed);
            while( !this->wheel(1) );
            // delay(150);
            _lastError = -5;
        }
        else {
            motor.speed(_m1, _tightness*_turnSpeed/100);
            motor.speed(_m2, _turnSpeed);
            while( !this->wheel(1) );
            // delay(150);
            _lastError = 5;
        }
        _error = 0;
        _recentError = 0;
        
        while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
        _backing = false;
    }
}

void Drive::straight(void)
{
    // update encoder
    this->wheel(_wheelL);
    
    _sack = 500;
}

void Drive::right(boolean tight)
{
    // update encoder
    this->wheel(_wheelL);
    
    if (tight) {
        motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        while( !this->wheel(_wheelL) );
        // delay(150);
        
        if (_backing) {
            motor.speed(_m2, _turnSpeed);
            motor.speed(_m1, 1.5 * _tightness*_turnSpeed/100);
            while( !this->wheel(_wheelL) );
            // delay(150);
            _lastError = 5;
            
        }
        else {
            motor.speed(_m1, _turnSpeed);
            motor.speed(_m2, 1.5 * _tightness*_turnSpeed/100);
            while( !this->wheel(_wheelL) );
            // delay(150);
            _lastError = -5;
        }
        _error = 0;
        _recentError = 0;
        
        while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
        _backing = false;
    }
    else {
        motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        while( !this->wheel(0) );
        // delay(150);
        
        if (_backing) {
            motor.speed(_m2, _turnSpeed);
            motor.speed(_m1, _tightness*_turnSpeed/100);
            while( !this->wheel(0) );
            // delay(150);
            _lastError = 5;
            
        }
        else {
            motor.speed(_m1, _turnSpeed);
            motor.speed(_m2, _tightness*_turnSpeed/100);
            while( !this->wheel(0) );
            // delay(150);
            _lastError = -5;
        }
        _error = 0;
        _recentError = 0;
        
        while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
        _backing = false;
    }
}

void Drive::reverse(void)
{
    _backing = !_backing;
}

void Drive::uturn(boolean ccw)
{
    if (ccw) {
        _backing = true;
        
        motor.speed(_m1, -200);
        motor.speed(_m2, 50);
        
        while ( !this->collision() );
        
        _backing = false;
        motor.speed(_m1, -50);
        motor.speed(_m2, 200);
        while (!(digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1)));
        
        _lastError = 5;
        
    }
    else {
        _backing = true;
        
        motor.speed(_m1, 50);
        motor.speed(_m2, -200);
        
        while ( !this->collision() );
        
        _backing = false;
        motor.speed(_m1, 200);
        motor.speed(_m2, -50);
        while (!(digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2)));
        
        _lastError = -5;
    }
    
    _error = 0;
    _recentError = 0;
}

void Drive::prepareDrop(void)
{
    // update encoder
    this->wheel(_wheelL);
    
    _hack = 44;
}

void Drive::prepareEndpoint(void)
{
    _hack = 15000 / _speed;
}

boolean Drive::intersection()
{
    _hack && this->wheel(_wheelL) && --_hack;
    if(_hack == 1)
    {
        return true;
    }
    else if (
             !_sack
             
             &&
             
             (
              ( digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) )
              
              ||
              
              ( digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) )
              )
             )
    {
        return true;
    }
    
    _sack && this->wheel(_wheelL) && --_sack;
    
    return false;
}

boolean Drive::wheel(uint8_t _wheel)
{
    if ( _wheel == _wheelL ) {
        if (
            digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black
            )
        {
            _black = !_black;
            return true;
        }
        else return false;
    }
    else {
        if (
            digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack
            )
        {
            _brack = !_brack;
            return true;
        }
        else return false;
    }
}

boolean Drive::collision(void)
{
    return
    (
     ( _backing )
     
     &&
     
     (
      !( digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) || digitalRead(_col3) )
      
      ||
      
      !( digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) || digitalRead(_col4) )
      )
     )
     
     ||
     
     (
      !( _backing )
      
      &&
      
      (
       !( (digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1) || digitalRead(_col1)) )
       
       ||
       
       !( (digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2) || digitalRead(_col2)) )
       )
      );
}

void Drive::speed(int16_t speed)
{
    _speed = speed;
}

void Drive::brake(void)
{
    motor.stop_all();
}

void Drive::stats(boolean collision)
{
    if (_c > 30) {
        LCD.clear();
        LCD.home();
        
        if (collision) {
            LCD.print("fl:");
            LCD.print(digitalRead(_col1));
            LCD.print(" bl:");
            LCD.print(digitalRead(_col2));
            
            LCD.setCursor(0, 1);
            
            LCD.print("fr:");
            LCD.print(digitalRead(_col3));
            LCD.print(" br:");
            LCD.print(digitalRead(_col4));
        }
        else {
            LCD.print("fl:");
            LCD.print(digitalRead(_qrd1));
            LCD.print(" bl:");
            LCD.print(digitalRead(_qrd5));
            LCD.print(" il:");
            LCD.print(digitalRead(_qrd3));
            
            LCD.setCursor(0,1);
            
            LCD.print("fr:");
            LCD.print(digitalRead(_qrd2));
            LCD.print(" br:");
            LCD.print(digitalRead(_qrd6));
            LCD.print(" ir:");
            LCD.print(digitalRead(_qrd4));
        }
        _c = 0;
    }
    
    ++_c;
}

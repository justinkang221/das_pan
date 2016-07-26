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
#define _tightness (-55)

#define _threshold (500)

#define _col1 (15)
#define _col2 (14)
#define _col3 (12)
#define _col4 (13)

#define _wheelL (9)
#define _wheelR (10)

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
    
    _distanceR = 0;
    _distanceL = 0;
	
	_interL = false;
	_interR = false;
	_interS = true;
	_interSPast = false;
	_lost = false;
	_store = 0;
    
    _i = 0;
}

void Drive::setPD(uint8_t kp, uint8_t kd)
{
    _kp = kp;
    _kd = kd;
}

void Drive::addDistance(void){
    if(this->wheel(_wheelR)){
        _distanceR++;
    }
    if(this->wheel(_wheelL)){
        _distanceL++;
    }
}

void Drive::removeDistance(void){
    if(this->wheel(_wheelR)){
        _distanceR--;
    }
    if(this->wheel(_wheelL)){
        _distanceL--;
    }
}

uint16_t Drive::getDistance(void){
    return min(_distanceR,_distanceL);
}

void Drive::go(void)
{
    if (_backing) {
        this->removeDistance();
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
        this->addDistance();
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
        /*motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelR) );
        	}
        // delay(150);
        */
        if (_backing) {
            motor.speed(_m2, 1.5 * _tightness*_turnSpeed/100);
            motor.speed(_m1, _turnSpeed);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelR) );
        	}
            //delay(150);
            _lastError = -5;
        }
        else {
            motor.speed(_m1, 1.5 * _tightness*_turnSpeed/100);
            motor.speed(_m2, _turnSpeed);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelR) );
        	}
            // delay(150);
            _lastError = 5;
        }
        _error = 0;
        _recentError = 0;
        
        while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
        _backing = false;
    }
    else {
        /*motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelR) );
        	}
        // delay(150);
        */
        if (_backing) {
            motor.speed(_m2, _tightness*_turnSpeed/100);
            motor.speed(_m1, _turnSpeed);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelR) );
        	}
            // delay(150);
            _lastError = -5;
        }
        else {
            motor.speed(_m1, _tightness*_turnSpeed/100);
            motor.speed(_m2, _turnSpeed);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelR) );
        	}
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
}

void Drive::right(boolean tight)
{
    // update encoder
    this->wheel(_wheelL);
    
    if (tight) {
        /*motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelL) );
        	}
        // delay(150);
        */
        if (_backing) {
            motor.speed(_m2, _turnSpeed);
            motor.speed(_m1, 1.5 * _tightness*_turnSpeed/100);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelL) );
        	}
            // delay(150);
            _lastError = 5;
            
        }
        else {
            motor.speed(_m1, _turnSpeed);
            motor.speed(_m2, 1.5 * _tightness*_turnSpeed/100);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelL) );
        	}
            // delay(150);
            _lastError = -5;
        }
        _error = 0;
        _recentError = 0;
        
        while (!(digitalRead(_qrd1) && digitalRead(_qrd2)));
        _backing = false;
    }
    else {
		/*
        motor.speed(_m1, _speed);
        motor.speed(_m2, _speed);
        for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelL) );
        	}
        // delay(150);
        */
        if (_backing) {
            motor.speed(_m2, _turnSpeed);
            motor.speed(_m1, _tightness*_turnSpeed/100);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelL) );
        	}
            // delay(150);
            _lastError = 5;
            
        }
        else {
            motor.speed(_m1, _turnSpeed);
            motor.speed(_m2, _tightness*_turnSpeed/100);
            for (_i = 0; _i<6; _i++){
        	while( !this->wheel(_wheelL) );
        	}
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

boolean Drive::isBacking(void)
{
    return _backing;
}

void Drive::uturn(boolean ccw)
{
    if (ccw) {
        _backing = true;
		
        this->wheel(_wheelL);
		while(true){
		motor.speed(_m1, -150);
        motor.speed(_m2, 0);
        for (_i = 0; _i<4; _i++){
        	while( !this->wheel(_wheelL) );
			if(this->collisionSpecific(_col3)){
				break;
			}
        }
		if(this->collisionSpecific(_col3)){
				break;
			}
		motor.speed(_m1, -150);
        motor.speed(_m2, -150);
        this->wheel(_wheelL);
        for (_i = 0; _i<4; _i++){
        	while( !this->wheel(_wheelL) );
			if(this->collisionSpecific(_col3)){
				break;
			}
        }
		if(this->collisionSpecific(_col3)){
				break;
		}
		}
        _backing = false;
        motor.speed(_m1, 0);
        motor.speed(_m2, 150);
		while (!(digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2)));
		motor.speed(_m1, 0);
		motor.speed(_m2, 0);
		while( !startbutton() )
        //while (!(digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1)));
        
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
    
    _hack = 88;
}

void Drive::prepareEndpoint(void)
{
    _hack = 20;
}

uint8_t Drive::isSacked(void){
	if (_sack == 0){
		return 0;
	}
	else if(_sack > 1){
		return 2;
	}
	else{
		return 1;
	}
}

int8_t Drive::describeIntersection(void){
	if(_sack){
		if(this->wheel(_wheelL)){
		--_sack;
		}
		if(!_interL && ( digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3))) 
		{
			_interL=true;
		}
		if(!_interR && ( digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) )){
			_interR=true;
		}
		if(_interS && !_interSPast &&
		( !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1) && !digitalRead(_qrd1)) &&
		( !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2) && !digitalRead(_qrd2)) 
		){
			_interS = false;
			_interSPast= true;
		}
		if(!_interS && _interSPast &&
		(( digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1) && digitalRead(_qrd1)) ||
		( digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2) && digitalRead(_qrd2))) 
		){
			_interS = true;
		}
	if(_sack == 0){
		_store = _interL + 2*_interS + 4*_interR;
		_interR = false;
		_interL = false;
		_interS = true;
		_interSPast = false;
		return _store;
	}
	}
	else return -1;
}

/*
boolean Drive::intersection(boolean right){
	if(right){
		if (( digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3)) 
		{
		   return true;
	    }
	    else return false; 
	}
	else {
		if( digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) ){
			return true;
		}
		else return false;
	}
}
*/

boolean Drive::intersection(void)
{
    _hack && this->wheel(_wheelL) && --_hack;
    if(_hack == 1)
    {
        _distanceL=0;
        _distanceR=0;
		_sack = 4;
		this->wheel(_wheelL);
        return true;
    }
    else if (_backing){
        if(this->getDistance() == 0){
            _distanceL=0;
            _distanceR=0;
			_sack = 4;
			this->wheel(_wheelL);
            return true;
        }
    }
    else if (!_sack){
		if(( digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) && digitalRead(_qrd3) )){
			_interL = true;
		}
		if(( digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) && digitalRead(_qrd4) )){
			_interR = true;
		}
		if(_interR || _interL){
			_distanceL=0;
			_distanceR=0;
			_sack = 4;
			this->wheel(_wheelL);
			return true;
		}
	}
    
    return false;
}

boolean Drive::wheel(uint8_t _wheel)
{
    if ( _wheel == _wheelR ) {
        if (
            digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack
            && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack && digitalRead(_wheel) != _brack
            )
        {
            _brack = !_brack;
            return true;
        }
        else return false;
            
    }
    else {
		if(
		digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black
        && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black && digitalRead(_wheel) != _black
        )
        {
            _black = !_black;
            return true;
        }
        else return false;
    }
}

boolean Drive::collisionSpecific(uint8_t _colNum){
	return !( digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) || digitalRead(_colNum) );
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
    if (_c > 0) {
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
            LCD.print(digitalRead(_qrd4));
            
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

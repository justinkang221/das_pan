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
    
    _backerror = 0;
    _backlastError = 0;
    _backrecentError = 0;
    
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
    _j = 0;
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

void Drive::record(boolean front){
    if(front){
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
        }
        
        if (_error != _lastError) {
            _recentError = _lastError;
            _q = _m;
            _m = 1;
        }
        _lastError = _error;
        ++_m;
    }
    else {
        _left = digitalRead(_qrd5);
        _right = digitalRead(_qrd6);
        
        if (_left) {
            if (_right) _backerror = 0;
            else _backerror = -1;
        }
        else {
            if (_right) _backerror = 1;
            else if (_backlastError > 0) _backerror = 5;
            else _backerror = -5;
        }
        
        if (_backerror != _backlastError) {
            _backrecentError = _backlastError;
            
        }
        if (_backerror != _backlastError) {
            _backrecentError = _backlastError;
            _q = _m;
            _m = 1;
        }
        _backlastError = _backerror;
        ++_m;
    }
}

    void Drive::go(void)
{
    if (_backing) {
        this->record(true);
        this->removeDistance();
        _left = digitalRead(_qrd5);
        _right = digitalRead(_qrd6);
        
        if (_left) {
            if (_right) _backerror = 0;
            else _backerror = -1;
        }
        else {
            if (_right) _backerror = 1;
            else if (_backlastError > 0) _backerror = 5;
            else _backerror = -5;
        }
        
        if (_backerror != _backlastError) {
            _backrecentError = _backlastError;
            _q = _m;
            _m = 1;
        }
        
        _p = _kp * _backerror;
        _d = _kd * (_backerror - _backrecentError) / (_q + _m);
        _correction = _p + _d;
        
        motor.speed(_m2, -0.5 * (_speed + _correction));
        motor.speed(_m1, -0.5 * (_speed - _correction));
        _backlastError = _backerror;
    }
    else {
        this->record(false);
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
            for (_i = 0; _i<2; _i++){
        	while( !this->wheel(_wheelR) );
        	}
            //delay(150);
            _lastError = -5;
        }
        else {
            motor.speed(_m1, 1.5 * _tightness*_turnSpeed/100);
            motor.speed(_m2, _turnSpeed);
            for (_i = 0; _i<2; _i++){
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
            for (_i = 0; _i<2; _i++){
        	while( !this->wheel(_wheelR) );
        	}
            // delay(150);
            _lastError = -5;
        }
        else {
            motor.speed(_m1, _tightness*_turnSpeed/100);
            motor.speed(_m2, _turnSpeed);
            for (_i = 0; _i<2; _i++){
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
            for (_i = 0; _i<2; _i++){
        	while( !this->wheel(_wheelL) );
        	}
            // delay(150);
            _lastError = 5;
            
        }
        else {
            motor.speed(_m1, _turnSpeed);
            motor.speed(_m2, 1.5 * _tightness*_turnSpeed/100);
            for (_i = 0; _i<2; _i++){
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
            for (_i = 0; _i<2; _i++){
        	while( !this->wheel(_wheelL) );
        	}
            // delay(150);
            _lastError = 5;
            
        }
        else {
            motor.speed(_m1, _turnSpeed);
            motor.speed(_m2, _tightness*_turnSpeed/100);
            for (_i = 0; _i<2; _i++){
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
    _distanceL += 20;
    _distanceR += 20;
    _backing = !_backing;
}

boolean Drive::isBacking(void)
{
    return _backing;
}

void Drive::uturn(boolean ccw = true)
{
    if (ccw) {
        
    // enh
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, -150);
    motor.speed(_m2, -50);
    for ( _i = 0, _j = 0; (_i - _j) < 12;) {
        if(this->wheel(_wheelL) ) ++_i;
        if(this->wheel(_wheelR) ) ++_j;
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // whoop
    _m = 1;
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, -150);
    motor.speed(_m2, 125);
    for ( _i = 0; _i < 17;) {
        this->record(true);
        if(this->wheel(_wheelL) ) ++_i;
        if(this->collisionSpecific(_col1) || this->collisionSpecific(_col2)){
            motor.speed(_m1, -150);
            motor.speed(_m2, -150);
            delay(250);
            motor.speed(_m1, -150);
            motor.speed(_m2, 125);
			this->wheel(_wheelL);
			_i-=2;
        }
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // enh
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, 50);
    motor.speed(_m2, 150);
    for ( _i = 0, _j = 0; (_j - _i) < 11;) {
        this->record(true);
        if(this->wheel(_wheelL) ) ++_i;
        if(this->wheel(_wheelR) ) ++_j;
        if(this->collisionSpecific(_col1) || this->collisionSpecific(_col2)){
            motor.speed(_m1, -150);
            motor.speed(_m2, -150);
            delay(250);
            motor.speed(_m1, 50);
            motor.speed(_m2, 150);
			this->wheel(_wheelL);
			this->wheel(_wheelR);
        }
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // back
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, -150);
    motor.speed(_m2, -160);
    for ( _i = 0; _i < 24;) {
        this->record(true);
        if(this->wheel(_wheelL) ) ++_i;
        if(this->collisionSpecific(_col3) || this->collisionSpecific(_col4)) break;
    }
	
	else {
		// enh
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, -50);
    motor.speed(_m2, -150);
    for ( _i = 0, _j = 0; (_i - _j) < 12;) {
        if(this->wheel(_wheelL) ) ++_j;
        if(this->wheel(_wheelR) ) ++_i;
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // whoop
    _m = 1;
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, 125);
    motor.speed(_m2, -150);
    for ( _i = 0; _i < 17;) {
        this->record(true);
        if(this->wheel(_wheelR) ) ++_i;
        if(this->collisionSpecific(_col1) || this->collisionSpecific(_col2)){
            motor.speed(_m1, -150);
            motor.speed(_m2, -150);
            delay(250);
            motor.speed(_m1, 125);
            motor.speed(_m2, -150);
			this->wheel(_wheelR);
			_i-=2;
        }
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // enh
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, 150);
    motor.speed(_m2, 50);
    for ( _i = 0, _j = 0; (_j - _i) < 11;) {
        this->record(true);
        if(this->wheel(_wheelL) ) ++_j;
        if(this->wheel(_wheelR) ) ++_i;
        if(this->collisionSpecific(_col1) || this->collisionSpecific(_col2)){
            motor.speed(_m1, -150);
            motor.speed(_m2, -150);
            delay(250);
            motor.speed(_m1, 150);
            motor.speed(_m2, 50);
			this->wheel(_wheelL);
			this->wheel(_wheelR);
        }
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // back
    this->wheel(_wheelL);
    this->wheel(_wheelR);
    motor.speed(_m1, -150);
    motor.speed(_m2, -160);
    for ( _i = 0; _i < 24;) {
        this->record(true);
        if(this->wheel(_wheelL) ) ++_i;
        if(this->collisionSpecific(_col3) || this->collisionSpecific(_col4)) break;
    }
	}
    
}

void Drive::prepareDrop(void)
{
    // update encoder
    this->wheel(_wheelL);
    
    _hack = 80;
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
        this->speed(150);
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
    /*if (_hack && this->wheel(_wheelL)) {
		--_hack;
		LCD.home();
		LCD.print(_hack);
	}*/
    if(_hack && _hack == _distanceL)
    {
        _distanceL=0;
        _distanceR=0;
		_sack = 4;
		_hack = 0;
        this->speed(75);
		this->brake();
		while(!startbutton);
        return true;
    }
    else if (_backing){
        if(this->getDistance() == 0){
            _distanceL=0;
            _distanceR=0;
            this->speed(150);
			this->wheel(_wheelL);
            _error = -_error;
            _lastError = -_lastError;
            _recentError = -_recentError;
            _backing = false;
        }
        return false;
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
            this->speed(75);
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
            LCD.print(digitalRead(_col3));
            
            LCD.setCursor(0, 1);
            
            LCD.print("fr:");
            LCD.print(digitalRead(_col2));
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

/*
 * Drive.cpp - Library for driving robot.
 */

#include <drive.h>
#include <phys253.h>
#include <LiquidCrystal.h>
#include <Arduino.h>

#define M1 (2)
#define M2 (1)

#define QRD1 (0)
#define QRD2 (1)
#define QRD3 (2)
#define QRD4 (3)
#define QRD5 (4)
#define QRD6 (5)

#define TURN_SPEED (150)
#define TIGHTNESS (-55)

#define THRESHOLD (500)

#define COL1 (12)
#define COL2 (13)
#define COL3 (14)
#define COL4 (15)

#define WHEEL (6)
#define WHEER (7)

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
    
    _black = digitalRead(WHEEL);
    _brack = digitalRead(WHEER);
    
    _distanceR = 0;
    _distanceL = 0;
	_oldR = 0;
    _oldL = 0;
	
	_interL = false;
	_interR = false;
	_interS = true;
	_interSPast = false;
	_lost = false;
    _interrupt = false;
	_store = 0;
    
    _i = 0;
    _j = 0;
	_k = 0;
}

void Drive::setPD(uint8_t kp, uint8_t kd)
{
    _kp = kp;
    _kd = kd;
}

void Drive::addDistance(void){
    if(this->wheel(WHEER)){
		_distanceR++;
    }
    if(this->wheel(WHEEL)){
		_distanceL++;
		if(_sack) --_sack;
    }
}

void Drive::removeDistance(void){
    if(this->wheel(WHEER)){
        _distanceR--;
    }
    if(this->wheel(WHEEL)){
        _distanceL--;
    }
}

uint16_t Drive::getDistance(void){
    return min(_distanceR,_distanceL);
}

uint16_t Drive::leftDistance(void){
    return _oldL;
}

uint16_t Drive::rightDistance(void){
    return _oldR;
}

void Drive::record(boolean front){
    if(front){
        _left = digitalRead(QRD1);
        _right = digitalRead(QRD2);
        
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
        _left = digitalRead(QRD5);
        _right = digitalRead(QRD6);
        
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
        _left = digitalRead(QRD5);
        _right = digitalRead(QRD6);
        
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
        
        motor.speed(M2, -0.5 * (_speed + _correction));
        motor.speed(M1, -0.5 * (_speed - _correction));
        _backlastError = _backerror;
    }
    else {
        this->record(false);
        this->addDistance();
        _left = digitalRead(QRD1);
        _right = digitalRead(QRD2);
        
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
        
        motor.speed(M1, _speed + _correction);
        motor.speed(M2, _speed - _correction + 10);
        _lastError = _error;
    }
}

void Drive::left(boolean tight)
{
    if (tight) {
        
        /*if (_backing) {
            motor.speed(M2, 1.5 * TIGHTNESS * TURN_SPEED / 100);
            motor.speed(M1, TURN_SPEED);
            for (_i = 0; _i < 2; ++_i){
                while( !this->wheel(WHEER) );
        	}
         _lastError = -5;
        }
        else {*/
        motor.speed(M1, 1.5 * TIGHTNESS * TURN_SPEED / 100);
        motor.speed(M2, TURN_SPEED);
        for (_i = 0; _i < 2; ++_i) {
            while( !this->wheel(WHEER) );
        }
        _lastError = 5;
        _error = 0;
        _recentError = 0;
        //}
        
        while ( !(digitalRead(QRD1) && digitalRead(QRD2)) ) {
            if ( this->wheel(WHEER) ) ++_i;
            if ( this->collision() ) {
                _interrupt = true;
                break;
            }
        }
        
        if (_interrupt) {
            this->unturn(true, true, false);
            _interrupt = false;
        }
    }
    else {
        
        /*if (_backing) {
            motor.speed(M2, TIGHTNESS*TURN_SPEED/100);
            motor.speed(M1, TURN_SPEED);
            for (_i = 0; _i < 2; ++_i){
                while( !this->wheel(WHEER) );
        	}
         _lastError = -5;
        }
        else {*/
        motor.speed(M1, TIGHTNESS*TURN_SPEED/100);
        motor.speed(M2, TURN_SPEED);
        for (_i = 0; _i < 2; ++_i){
            while( !this->wheel(WHEER) );
        }
        _lastError = 5;
        _error = 0;
        _recentError = 0;
        //}
        
        while (!(digitalRead(QRD1) && digitalRead(QRD2))) {
            if ( this->wheel(WHEER) ) ++_i;
            if ( this->collision() ) {
                _interrupt = true;
                break;
            }
        }
        
        if (_interrupt) {
            this->unturn(true, false, false);
            _interrupt = false;
        }
        
        _backing = false;
    }
	_distanceL = 8;
	_distanceR = 8;
}

void Drive::straight(void)
{
    // update encoder
}

void Drive::right(boolean tight)
{    
    if (tight) {
        
        /*if (_backing) {
            motor.speed(M2, TURN_SPEED);
            motor.speed(M1, 1.5 * TIGHTNESS*TURN_SPEED/100);
            for (_i = 0; _i<2; _i++){
        	while( !this->wheel(WHEEL) );
        	}
            _lastError = 5;
            
        }
        else {*/
        motor.speed(M1, TURN_SPEED);
        motor.speed(M2, 1.5 * TIGHTNESS*TURN_SPEED/100);
        for (_i = 0; _i < 2; ++_i){
            while( !this->wheel(WHEEL) );
        }
        _lastError = -5;
        _error = 0;
        _recentError = 0;
        //}
        
        while (!(digitalRead(QRD1) && digitalRead(QRD2))) {
            if (this->wheel(WHEEL)) ++_i;
            if ( this->collision() ) {
                _interrupt = true;
                break;
            }
        }
        
        if (_interrupt) {
            this->unturn(false, true, false);
            _interrupt = false;
        }
        
        _backing = false;
    }
    else {
		
        /*if (_backing) {
            motor.speed(M2, TURN_SPEED);
            motor.speed(M1, TIGHTNESS*TURN_SPEED/100);
            for (_i = 0; _i<2; _i++){
        	while( !this->wheel(WHEEL) );
        	}
            _lastError = 5;
            
        }
        else {*/
        motor.speed(M1, TURN_SPEED);
        motor.speed(M2, TIGHTNESS*TURN_SPEED/100);
        for (_i = 0; _i<2; _i++){
            while( !this->wheel(WHEEL) );
        }
        _lastError = -5;
        _error = 0;
        _recentError = 0;
        //}
        
        while (!(digitalRead(QRD1) && digitalRead(QRD2))) {
            if (this->wheel(WHEEL)) ++_i;
            if ( this->collision() ) {
                _interrupt = true;
                break;
            }
        }
        
        if (_interrupt) {
            this->unturn(false, false, false);
            _interrupt = false;
        }
        
        _backing = false;
    }
	
	_distanceL = 8;
	_distanceR = 8;
}

void Drive::unturn(boolean left, boolean tight, boolean reverse)
{
    if (reverse) {
        _backing = true;
        while (this->getDistance() > 0) {
            this->go();
        }
        _backing = false;
    }
    if (left) {
        LCD.home();
        LCD.print("interrupt ");
        LCD.print(_i);
        this->brake();
        while (!startbutton() );
        motor.speed(M1, -1 * (TIGHTNESS*TURN_SPEED/100) * (1 + tight * 0.5));
        motor.speed(M2, -1 * TURN_SPEED);
        for (; _i > 0; --_i) {
            while ( !this->wheel(WHEER) );
            if (digitalRead(QRD1) && digitalRead(QRD2)) {
                break;
            }
        }
        motor.speed(M1, -75);
        motor.speed(M2, -75);
        for (_i = 0; _i < 4; ++_i) {
            while ( !this->wheel(WHEER));
        }
        this->uturn(true);
    }
    else {
        LCD.home();
        LCD.print("interrupt ");
        LCD.print(_i);
        this->brake();
        while (!startbutton() );
        motor.speed(M1, -1 * TURN_SPEED);
        motor.speed(M2, (-1 * (TIGHTNESS*TURN_SPEED/100)) * (1 + tight * 0.5));
        for (; _i > 0; --_i) {
            while ( !this->wheel(WHEEL) );
            if (digitalRead(QRD1) && digitalRead(QRD2)) {
                break;
            }
        }
        motor.speed(M1, -75);
        motor.speed(M2, -75);
        for (_i = 0; _i < 4; ++_i) {
            while( !this->wheel(WHEEL) );
        }
        this->uturn(true);
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
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, -150);
    motor.speed(M2, -50);
	//_k=0;
    for ( _i = 0, _j = 0; (_i - _j) < 12;) {
		
        this->record(true);
		if(this->wheel(WHEEL) ){
			++_i;
			//_k=0;
		} 
        if(this->wheel(WHEER) ){
			//_k=0;
			++_j;
		}
		//else _k++;
		/*if(_k>5000){
			motor.speed(M1, -150);
            motor.speed(M2, -150);
			delay(250);
            motor.speed(M1, -150);
            motor.speed(M2, -50);
			_k=0;
		}*/
    }
    
    /*this->brake();
    while( !startbutton() );
    */
    // whoop
    _m = 1;
	_error = -_error;
	_lastError = -_lastError;
	_recentError = -_recentError;
	//_k=0;
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, -150);
    motor.speed(M2, 125);
    for ( _i = 0; _i < 17;) {
        this->record(true);
		if(this->wheel(WHEEL) ){
			++_i;
			//_k=0;
		}
		//else _k++;
		/*if(_k>5000){
			motor.speed(M1, -150);
            motor.speed(M2, -150);
			delay(250);
            motor.speed(M1, -150);
            motor.speed(M2, 125);
			_k=0;
		}*/
        if(this->collisionSpecific(COL1) || this->collisionSpecific(COL2)){
            motor.speed(M1, -150);
            motor.speed(M2, -150);
            delay(250);
            motor.speed(M1, -150);
            motor.speed(M2, 125);
			this->wheel(WHEEL);
			_i-=2;
        }
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // enh
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, 50);
    motor.speed(M2, 150);
	//_k=0;
    for ( _i = 0, _j = 0; (_j - _i) < 11;) {
        this->record(true);
		if(this->wheel(WHEEL) ){
			++_i;
			//_k=0;
		} 
        if(this->wheel(WHEER) ){
			//_k=0;
			++_j;
		}
		/*else _k++;
		if(_k>5000){
			motor.speed(M1, -150);
            motor.speed(M2, -150);
			delay(250);
            motor.speed(M1, 150);
            motor.speed(M2, 50);
			_k=0;
		}*/
        if(this->collisionSpecific(COL1) || this->collisionSpecific(COL2)){
            motor.speed(M1, -150);
            motor.speed(M2, -50);
            delay(250);
            motor.speed(M1, 50);
            motor.speed(M2, 150);
			_j++;
			this->wheel(WHEEL);
			this->wheel(WHEER);
        }
		/*if(digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2)){
			break;
		}*/
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // back
	
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, -150);
    motor.speed(M2, -160);
    for ( _i = 0; _i < 12;) {
        this->record(true);
        if(this->wheel(WHEEL) ) ++_i;
        if(this->collisionSpecific(COL3) || this->collisionSpecific(COL4)) break;
    }
    while(!(digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2))){
       this->go();
    }
    _distanceL = 0;
    _distanceR = 0;
		
	}
	else {
	// enh
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, -50);
    motor.speed(M2, -150);
	//_k=0;
    for ( _i = 0, _j = 0; (_i - _j) < 12;) {
		this->record(true);
		if(this->wheel(WHEEL) ){
			++_j;
			//_k=0;
		} 
        if(this->wheel(WHEER) ){
			//_k=0;
			++_i;
		}
		//else _k++;
		/*if(_k>5000){
			motor.speed(M1, -150);
            motor.speed(M2, -150);
			delay(250);
            motor.speed(M1, -150);
            motor.speed(M2, -50);
			_k=0;
		}*/
    }
    
    /*this->brake();
    while( !startbutton() );
    */
    // whoop
    _m = 1;
	_error = -_error;
	_lastError = -_lastError;
	_recentError = -_recentError;
	//_k=0;
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, -150);
    motor.speed(M2, 125);
    for ( _i = 0; _i < 17;) {
        this->record(true);
		if(this->wheel(WHEER) ){
			++_i;
			//_k=0;
		}
		//else _k++;
		/*if(_k>5000){
			motor.speed(M1, -150);
            motor.speed(M2, -150);
			delay(250);
            motor.speed(M1, -150);
            motor.speed(M2, 125);
			_k=0;
		}*/
        if(this->collisionSpecific(COL1) || this->collisionSpecific(COL2)){
            motor.speed(M1, -150);
            motor.speed(M2, -150);
            delay(250);
            motor.speed(M1, 125);
            motor.speed(M2, -150);
			this->wheel(WHEEL);
			_i-=2;
        }
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // enh
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, 150);
    motor.speed(M2, 50);
	//_k=0;
    for ( _i = 0, _j = 0; (_j - _i) < 11;) {
        this->record(true);
		if(this->wheel(WHEEL) ){
			++_j;
			//_k=0;
		} 
        if(this->wheel(WHEER) ){
			//_k=0;
			++_i;
		}
		/*else _k++;
		if(_k>5000){
			motor.speed(M1, -150);
            motor.speed(M2, -150);
			delay(250);
            motor.speed(M1, 150);
            motor.speed(M2, 50);
			_k=0;
		}*/
        if(this->collisionSpecific(COL1) || this->collisionSpecific(COL2)){
            motor.speed(M1, -50);
            motor.speed(M2, -150);
            delay(250);
            motor.speed(M1, 150);
            motor.speed(M2, 50);
			_j++;
			this->wheel(WHEEL);
			this->wheel(WHEER);
        }
		/*if(digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2)){
			break;
		}*/
    }
    /*
    this->brake();
    while( !startbutton() );
    */
    // back
	
    this->wheel(WHEEL);
    this->wheel(WHEER);
    motor.speed(M1, -150);
    motor.speed(M2, -160);
    for ( _i = 0; _i < 12;) {
        this->record(true);
        if(this->wheel(WHEEL) ) ++_i;
        if(this->collisionSpecific(COL3) || this->collisionSpecific(COL4)) break;
    }
    while(!(digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2))){
       this->go();
    }
    _distanceL = 0;
    _distanceR = 0;
		
	}
}

void Drive::prepareDrop(void)
{
     _hack = 80;
}

void Drive::prepareEndpoint(void)
{
    _hack = 20;
}

uint8_t Drive::atIntersection(void){
	if (_sack == 0){
		return 0;
	}
	else if(_sack > 1){
		return 2;
	}
	else{
		_sack = 0;
		return 1;
	}
}

/* Returns a unique value that describes the intersection
 * left - 1
 * straight - 2
 * right - 4
 */
int8_t Drive::describeIntersection(void){
	if(_sack){
		if(!_interL && ( digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3))) 
		{
			_interL=true;
		}
		if(!_interR && ( digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) )){
			_interR=true;
		}
		if(_interS && !_interSPast &&
		( !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1) && !digitalRead(QRD1)) &&
		( !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2) && !digitalRead(QRD2)) 
		){
			_interS = false;
			_interSPast= true;
		}
		if(!_interS && _interSPast &&
		(( digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1) && digitalRead(QRD1)) ||
		( digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2) && digitalRead(QRD2))) 
		){
			_interS = true;
		}
	}
	else{
		_store = _interL + 2*_interS + 4*_interR;
		_interR = false;
		_interL = false;
		_interS = true;
		_interSPast = false;
        this->speed(150);
		return _store;
	}
}

/*
boolean Drive::intersection(boolean right){
	if(right){
		if (( digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3)) 
		{
		   return true;
	    }
	    else return false; 
	}
	else {
		if( digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) ){
			return true;
		}
		else return false;
	}
}
*/

boolean Drive::intersection(void)
{
    /*if (_hack && this->wheel(WHEEL)) {
		--_hack;
		LCD.home();
		LCD.print(_hack);
	}*/
    if(_hack && _hack == _distanceL) {
        _distanceL=0;
        _distanceR=0;
		_sack = 6;
		_hack = 0;
        this->speed(75);
		this->brake();
		while(!startbutton);
        return true;
    }
    else if (_backing) {
        if(this->getDistance() == 0){
            _distanceL=0;
            _distanceR=0;
            this->speed(150);
            _error = -_error;
            _lastError = -_lastError;
            _recentError = -_recentError;
            _backing = false;
        }
        return false;
    }
    else if (!_sack) {
		if (( digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) && digitalRead(QRD3) )) {
			_interL = true;
		}
		if (( digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) && digitalRead(QRD4) )) {
			_interR = true;
		}
		if (_interR || _interL) {
			_oldL = _distanceL;
			_oldR = _distanceR;
			_distanceL=0;
			_distanceR=0;
            _sack = 6;
            this->speed(75);
			return true;
		}
	}
    
    return false;
}

boolean Drive::wheel(uint8_t _wheel)
{
    if ( _wheel == WHEER ) {
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

boolean Drive::collisionSpecific(uint8_t colNum){
	return !( digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) || digitalRead(colNum) );
}

boolean Drive::collision(void)
{
    return 
    (
     ( _backing )
     
     &&
     
     (
      !( digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) || digitalRead(COL3) )
      
      ||
      
      !( digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) || digitalRead(COL4) )
      )
     )
     
     ||
     
     (
      !( _backing )
      
      &&
      
      (
       !( (digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1) || digitalRead(COL1)) )
       
       ||
       
       !( (digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2) || digitalRead(COL2)) )
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
	/*Serial.print("Distance traveled on right:");
	Serial.println(_oldR);
    Serial.print("Distance traveled on left:");
	Serial.println(_oldL);
	Serial.print("Distance traveled during intersection right:");
	Serial.println(_distanceR);
    Serial.print("Distance traveled during intersection left:");
	Serial.println(_distanceL);
	return;*/
    if (_c > 30) {
        LCD.clear();
        LCD.home();
        
        if (collision) {
            LCD.print("fl:");
            LCD.print(digitalRead(COL1));
            LCD.print(" bl:");
            LCD.print(digitalRead(COL3));
            
            LCD.setCursor(0, 1);
            
            LCD.print("fr:");
            LCD.print(digitalRead(COL2));
            LCD.print(" br:");
            LCD.print(digitalRead(COL4));
        }
        else {
            LCD.print("fl:");
            LCD.print(digitalRead(QRD1));
            LCD.print(" bl:");
            LCD.print(digitalRead(QRD5));
            LCD.print(" il:");
            LCD.print(digitalRead(QRD3));
            
            LCD.setCursor(0,1);
            
            LCD.print("fr:");
            LCD.print(digitalRead(QRD2));
            LCD.print(" br:");
            LCD.print(digitalRead(QRD6));
            LCD.print(" ir:");
            LCD.print(digitalRead(QRD4));
        }
        _c = 0;
    }
    
    ++_c;
}

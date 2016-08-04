/*
 * Drive.h - Library for driving robot.
 */

#ifndef drive_h
#define drive_h

#include <Arduino.h>

class Drive
{
public:
    Drive(void);
    void setPD(uint8_t, uint8_t);
    
    void go(void);
    void brake(void);
    
    void left(boolean);
    void straight(void);
    void right(boolean);
    void reverse(void);
    void uturn(boolean);
    void unturn(boolean, boolean, boolean);
    
    boolean intersection(void);
    boolean collision(void);
	boolean collisionSpecific(uint8_t);
    boolean isBacking(void);
    
    void prepareDrop(void);
    void prepareEndpoint(void);
    void speed(int16_t);
    
    void addDistance(void);
    void removeDistance(void);
    int16_t getDistance(void);
	int16_t leftDistance(void);
	int16_t rightDistance(void);
    void resetDistance(void);
    void setOldDistance(int16_t, int16_t);
    
    void record(boolean);
    boolean offBoard(void);
    void burstBack(void);
    void setIrack(uint16_t);
    uint16_t isIracked(void);
    void stuck(void);
    void resetStuck(void);
	
	uint8_t atIntersection(void);
	int8_t describeIntersection(void);
    void resetIntersection(void);
    
    boolean wheel(uint8_t);
    void stats(boolean collision);
private:
    int16_t _speed;
    
    int16_t _distanceR;
    int16_t _distanceL;
    int16_t _oldR;
    int16_t _oldL;
    
    uint8_t _kp;
    uint8_t _kd;
    
    int16_t _p;
    int16_t _d;
    int16_t _correction;
    
    boolean _left;
    boolean _right;
    
    int8_t _error;
    int8_t _lastError;
    int8_t _recentError;
    
    int8_t _backerror;
    int8_t _backlastError;
    int8_t _backrecentError;
    
    uint8_t _c;
    uint8_t _q;
    uint8_t _m;
    
    uint8_t _i;
    uint8_t _j;
	uint16_t _k;
    
    uint16_t _hack;
    uint16_t _sack;
    uint16_t _irack;
	
	boolean _interL;
	boolean _interR;
	boolean _interS;
	boolean _interSPast;
	boolean _lost;
    int8_t _store;
    
    uint8_t _backing;
    boolean _black;
    boolean _brack;
    boolean _interrupt;
    
    unsigned long _currTime;
    
    unsigned long _stuckTime;
    uint8_t _wereFuckinJammed;
};

#endif

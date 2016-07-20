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
    
    void left(void);
    void forward(void);
    void right(void);
    void reverse(void);
    void uturn(void);
    
    boolean intersection(void);
    uint8_t collision(void);
    
    void prepareDrop(void);
    void prepareEndpoint(void);
    void speed(int16_t);
    
    void stats(void);
private:
    int16_t _speed;
    
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
    
    uint8_t _c;
    uint8_t _q;
    uint8_t _m;
    uint8_t _i;
    
    uint16_t _hack;
    uint16_t _sack;
    boolean _backing;
    boolean _inter;
};

#endif

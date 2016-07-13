/*
 * Tape.h - Library for tape-following
 * Created by Rahat Dhande, July 10, 2016.
 */

#ifndef tape_h
#define tape_h

#include <Arduino.h>

class Drive
{
public:
    Drive(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    void setPD(uint8_t, uint8_t);
    void straight();
    void left();
    void right();
    void reverse();
    void brake();
    boolean intersection();
    void speed(int16_t);
    
    void stats();
private:
    int16_t _speed;
    int16_t _turnSpeed;
    int8_t _tightness;
    
    uint8_t _m1;
    uint8_t _m2;
    
    uint8_t _qrd1;
    uint8_t _qrd2;
    uint8_t _qrd3;
    uint8_t _qrd4;
    
    uint8_t _kp;
    uint8_t _kd;
    
    int16_t _p;
    int16_t _d;
    int16_t _correction;
    
    uint16_t _left;
    uint16_t _right;
    uint16_t _threshold;
    uint16_t _intersection;
    
    int8_t _error;
    int8_t _lastError;
    int8_t _recentError;
    
    uint8_t _c;
    uint8_t _q;
    uint8_t _m;
};

#endif

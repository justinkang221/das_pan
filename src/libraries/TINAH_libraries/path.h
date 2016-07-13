/*
 * Path.h - Library for finding an optimal path for the robot.
 */

#ifndef path_h
#define path_h

#include <Arduino.h>
#include <phys253.h>
#include <LiquidCrystal.h>

class Path
{
public:
    Path(uint8_t);
    void find(void);
    uint8_t turn(void);
    void passengers(uint8_t, uint8_t);
    void stats(void);
private:
    uint8_t _intersections[21][4] = {
        { -1, 17, -1, -1}, // 0
        { -1, 6, -1, -1}, // 1
        { -1, 7, -1, -1}, // 2
        { -1, 8, -1, -1}, // 3
        { -1, 19, -1, -1}, // 4
        {6, -1, -1, -1}, // 5
        { -1, 11, 5, 1}, // 6
        {13, -1, 12, 2}, // 7
        {9, 14, -1, 3}, // 8
        { -1, -1, 8, -1}, // 9
        {18, -1, 16, 11}, // 10
        {12, 10, 17, 6}, // 11
        { -1, 13, 11, 7}, // 12
        {14, 12, -1, 7}, // 13
        {19, 15, 13, 8}, // 14
        { 20, -1, 18, 14}, // 15
        {10, -1, -1, 17}, // 16
        {11, 16, -1, 0}, // 17
        {15, -1, 10, -1}, // 18
        {-1, 20, 14, 4}, // 19
        { -1, -1, 15, 19} // 20
    };
    
    uint8_t _r0[6] = {11, 17, 0, 17, 16, 10};
    uint8_t _r1[7] = {11, 6, 1, 6, 5, 6, 11};
    uint8_t _r2[5] = {12, 7, 2, 7, 13};
    uint8_t _r3[7] = {14, 8, 3, 8, 9, 8, 14};
    uint8_t _r4[6] = {14, 19, 4, 19, 20, 15};
    uint8_t _r5[3] = {10, 18, 15};
    uint8_t _r6[6] = {10, 11, 12, 13, 14, 15};
    
    uint8_t *_regions[7] = {_r0, _r1, _r2, _r3, _r4, _r5, _r6};
    uint8_t _regLengths[6] = {6, 7, 5, 7, 6, 3};
    
    uint8_t _bias[6] = {30, 30, 0, 30, 30, 50};
    
    uint8_t _lastSearched[6] = {0};
    
    uint8_t _current;
    uint8_t _next;
    uint8_t _last;
    uint8_t _region;
    uint8_t _regIndex;
    int8_t _regDirec;
    int8_t _nextReg;
    
    uint8_t _distance;
    
    uint8_t _best;
    uint8_t _big;
    int16_t _weight;
    
    uint8_t _ii;
    uint8_t _jj;
    uint8_t _kk;
    
    uint8_t _leftPassengers;
    uint8_t _rightPassengers;
    
    uint8_t _c;
};

#endif

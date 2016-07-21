/*
 * Path.h - Library for finding path.
 */

#include <path.h>
#include <phys253.h>
#include <Arduino.h>

#define _startingRight (1)

static const uint8_t _intersections[21][4] = {
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

static const uint8_t _r0[6] = {11, 17, 0, 17, 16, 10};
static const uint8_t _r1[7] = {11, 6, 1, 6, 5, 6, 11};
static const uint8_t _r2[5] = {12, 7, 2, 7, 13};
static const uint8_t _r3[7] = {14, 8, 3, 8, 9, 8, 14};
static const uint8_t _r4[6] = {14, 19, 4, 19, 20, 15};
static const uint8_t _r5[3] = {10, 18, 15};
static const uint8_t _r6[6] = {10, 11, 12, 13, 14, 15};

static const uint8_t *_regions[7] = {_r0, _r1, _r2, _r3, _r4, _r5, _r6};
static const uint8_t _regLengths[6] = {6, 7, 5, 7, 6, 3};

Path::Path(void)
{
    // TODO: set initial condition based on switch check
    //if (digitalRead(_startingRight)) {
    if (_startingRight) {
        _current = 19;
        _next = 20;
        _last = 4;
        _region = 4;
    }
    else {
        _current = 17;
        _next = 16;
        _last = 0;
        _region = 0;
    }
    
    _regIndex = 3;
    _regDirec = 1;
    _nextReg = -1;
    
    _leftPassengers = 0;
    _rightPassengers = 0;
    
    _c = 0;
}

int8_t Path::find(void)
{
    _next = -1;
    do {
        // if no region is being searched, continue moving towards next region
        if (_region == -1) {
            // check if next region has been reached
            if (_current == _regions[_nextReg][0]) {
                _region = _nextReg;
                _regIndex = 0;
                _regDirec = 1;
                _nextReg = -1;
            }
            else if (_current == _regions[_nextReg][_regLengths[_nextReg] - 1]) {
                _region = _nextReg;
                _regIndex = _regLengths[_nextReg] - 1;
                _regDirec = -1;
                _nextReg = -1;
            }
            // otherwise, keep travelling towards next region
            else {
                _regIndex += _regDirec;
                _next = _regions[6][_regIndex];
            }
        }
        // otherwise, continue searching current region
        if (_region != -1) {
            _regIndex += _regDirec;
            if (_regIndex == _regLengths[_region] || _regIndex == -1) {
                // if region was just finished, leave it and determine which region to search next
                _lastSearched[_region] = _distance;
                
                _best = 0;
                _big = _bias[_best] + _distance - _lastSearched[_best];
                for (_ii = 0; _ii < 6; ++_ii) {
                    _weight = _bias[_ii] + _distance - _lastSearched[_ii];
                    if (_weight < _big) continue;
                    else {
                        _big = _weight;
                        _best = _ii;
                    }
                }
                _nextReg = _best;
                _regDirec = (_nextReg > _region || _current == 10 ? 1 : -1);
                _region = -1;
                _regIndex = _current - 10;
            }
            else {
                _next = _regions[_region][_regIndex];
            }
        }
    } while (_next == -1);
    
    ++_distance;
    if (_current == 18) {
        if (_regDirec == 1) {
            return -1;
        }
        else {
            return -2;
        }
    }
    else if (_current < 5 || _current == 9 || _current == 5) {
        return -3;
    }
    else if (_current == 8 || _current == 6 ) {
        return 1;
    }
    else return 0;
}

uint8_t Path::turn(void)
{
    // find index of destination node
    _ii = 0;
    while (_intersections[_current][_ii] != _next) ++_ii;
    
    // find index of last node
    _jj = 0;
    while (_intersections[_current][_jj] != _last) ++_jj;
    
    // check if special turning case: 4,5 == 180 at nodes 4,0
    switch (_current) {
        case 4:
        case 0: return 4;
        case 7: return (_regDirec == 1 ? 3 : 1);
    }
    
    // return direction 0 == backwards, 1 == left, 2 == straight, 3 == right
    for (_kk = _jj; (_kk % 4) != _ii; ++_kk);
    
    return (_kk - _jj);
}

void Path::update(void)
{
    _last = _current;
    _current = _next;
}

boolean Path::nearDrop(void)
{
    return (_next == 18);
}

boolean Path::nearEndpoint(void)
{
    return (_next == 0 || _next == 1 || _next == 2 || _next==3 || _next == 4 || _next == 5 || _next == 9);
}

void Path::passengers(uint8_t leftPassengers, uint8_t rightPassengers)
{
    _leftPassengers = leftPassengers;
    _rightPassengers = rightPassengers;
    // TODO: adjust weights in favour of drop zone
}

void Path::avoid(void)
{
    // TODO: adjust weights in favour of the last node
}

void Path::stats(void)
{
    LCD.clear();
    LCD.home();
    
    LCD.print("r:");
    LCD.print(_region);
    LCD.print(" nr:");
    LCD.print(_nextReg);
    
    LCD.setCursor(0,1);
    
    LCD.print("l:");
    LCD.print(_last);
    LCD.print(" c:");
    LCD.print(_current);
    LCD.print(" n:");
    LCD.print(_next);
    LCD.print("   ");
    
    Serial.print("\nregion: ");
    Serial.print(_region);
    Serial.print("\nlast: ");
    Serial.print(_last);
    Serial.print("\ncurrent: ");
    Serial.print(_current);
    Serial.print("\nnext region: ");
    Serial.print(_nextReg);
    Serial.print("\nnext: ");
    Serial.print(_next);
    Serial.print("\n\n\n");
}

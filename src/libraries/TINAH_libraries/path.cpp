/*
 * Path.h - Library for finding an optimal path for the robot.
 */

#include <path.h>
#include <Arduino.h>

Path::Path(uint8_t startingLeft)
{
    if (startingLeft) {
        _current = 0;
        _next = 17;
        _last = 0;
        _region = 0;
    }
    else {
        _current = 4;
        _next = 19;
        _last = 4;
        _region = 4;
    }
    
    _regIndex = 2;
    _regDirec = -1;
    _nextReg = -1;
    
    _leftPassengers = 0;
    _rightPassengers = 0;
    
    _c = 0;
}

void Path::find(void)
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
}

uint8_t Path::turn(void)
{
    // find index of destination node
    _ii = 0;
    while (_intersections[_current][_ii] != _next) ++_ii;
    
    // find index of last node
    _jj = 0;
    while (_intersections[_current][_jj] != _last) ++_jj;
    
    for (_kk = _jj; (_kk % 4) != _ii; ++_kk);
    
    _last = _current;
    _current = _next;
    
    // return direction 0 == backwards, 1 == left, 2 == straight, 3 == right
    return (_kk - _jj);
}

void Path::passengers(uint8_t leftPassengers, uint8_t rightPassengers)
{
    _leftPassengers = leftPassengers;
    _rightPassengers = rightPassengers;
}

void Path::stats(void)
{
    LCD.clear();
    LCD.home();
    LCD.print("r: ");
    LCD.print(_region);
    LCD.print(" nr: ");
    LCD.print(_nextReg);
    LCD.setCursor(0,1);
    LCD.print("l:");
    LCD.print(_last);
    LCD.print(" c:");
    LCD.print(_current);
    LCD.print(" n:");
    LCD.print(_next);
    Serial.print(_current);
}

/*
 * Path.h - Library for finding path.
 */

#ifndef path_h
#define path_h

#include <Arduino.h>
#include <phys253.h>
#include <LiquidCrystal.h>

class Path
{
public:
    Path(void);
    void weights(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    
    int8_t find(void);
    uint8_t turn(void);
    void update(void);
    
    boolean nearDrop(void);
    boolean nearEndpoint(void);
    
    void passengers(uint8_t);
    void avoid(void);
	
	int8_t getDirections(void);
    
    void stats(void);
private:
    uint8_t _bias[6] = {30, 30, 30, 35, 30, 25}; // weights are for starting on left side
    uint8_t _lastSearched[6] = {0};
    
    uint8_t _current;
    int8_t _next;
    uint8_t _last;
    int8_t _region;
    int8_t _regIndex;
    int8_t _regDirec;
    int8_t _nextReg;
    
    uint8_t _distance;
    
    uint8_t _best;
    uint8_t _big;
    int16_t _weight;
    
    uint8_t _ii;
    uint8_t _jj;
    uint8_t _kk;
    
    uint8_t _c;
};

#endif

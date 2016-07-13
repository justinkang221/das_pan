/*
 * Pan.h - Library for actuating pans.
 * Created by Rahat Dhande, July 8, 2016.
 */
#ifndef Pan_h
#define Pan_h

class Pan
{
public:
    Pan();
    void leftUp();
    void leftPick(void);
    void leftDrop(void);
    void rightUp(void);
    void rightPick(void);
    void rightDrop(void);
};

#endif

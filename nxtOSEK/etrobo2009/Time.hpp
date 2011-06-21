#ifndef _TIME_H_
#define _TIME_H_

#include "Clock.h"

class Time
{
private:
    Clock &clock;
    S32 time;
public:
    Time(Clock &c):
        clock(c)
        {time = clock.now();};
    
    ~Time(){};

    void reset(){time = clock.now();}

    S32 elapsedTime(){return clock.now() - time;}
};

#endif /* _TIME_H_ */

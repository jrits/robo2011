#ifndef _DISTANCE_H_
#define _DISTANCE_H_

#include "SensorHistory.hpp"
#include "parameters.hpp"

class Distance
{
private:
    SensorHistory &history;
    F32 resetCount;///<リセット時測定した際のモータカウントの値。モータカウント値は左と右のモータカウント値の平均。
public:
    Distance(SensorHistory &sh):
        history(sh)
        {
            resetCount = 0;
        };
    
    ~Distance(){};

    F32 computeDistance();
    void reset();
};

#endif /* _DISTANCE_H_ */

//
// TripodLineTrace.h
//

#ifndef TripodLineTrace_H_
#define TripodLineTrace_H_

#include "LineTrace.h"

/**
 * ライントレーススキル
 */
class TripodLineTrace : public LineTrace
{
public:
    TripodLineTrace(float black, float white, float threshold);
    ~TripodLineTrace(){}
    void execute();
};
#endif

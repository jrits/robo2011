//
// TripodAngleTrace.h
//

#ifndef TripodAngleTrace_H_
#define TripodAngleTrace_H_

#include "AngleTrace.h"

/**
 * ライントレーススキル
 */
class TripodAngleTrace : public AngleTrace
{
public:
    TripodAngleTrace();
    ~TripodAngleTrace(){}
    void execute();
};
#endif

//
// TripodAngleTrace.h
//

#ifndef TripodAngleTrace_H_
#define TripodAngleTrace_H_

#include "AngleTrace.h"

/**
 * ���C���g���[�X�X�L��
 */
class TripodAngleTrace : public AngleTrace
{
public:
    TripodAngleTrace();
    ~TripodAngleTrace(){}
    void execute();
};
#endif

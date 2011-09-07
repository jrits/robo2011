//
// StraightDetector.h
//

#ifndef StraightDetector_H_
#define StraightDetector_H_

#include "Detector.h"

/**
 * 直線検知
 */
class StraightDetector : public Detector
{
public:
    StraightDetector(){}
    ~StraightDetector(){}
    bool detect();
};

#endif

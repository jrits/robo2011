//
// MarkerDetector.h
//

#ifndef MARKERDETECTOR_H_
#define MARKERDETECTOR_H_

#include "LightSensor.h"
#include "constants.h"
#include "Detector.h"
using namespace ecrobot;

extern LightSensor mLightSensor;
const int MARKER_NSAMPLE = 250;

class MarkerDetector : public Detector
{
private:
    unsigned int mNsample; //!< この期間中、黒が現れなければマーカと判断する。
    unsigned int mGrayCount; //!< 灰色のサンプルをカウントする。黒を見つけたら0にリセットされる。
public:
    MarkerDetector(int nsample = MARKER_NSAMPLE)
        : mNsample(nsample),
          mGrayCount(0) {}
    ~MarkerDetector(){}
public:
    bool detect();
};

#endif

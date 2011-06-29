///////////////////////////////////////////////////////////
//  FailDetector.h
//  Implementation of the Class FailDetector
//  Created on:      11-25-2010 19:42:51
//  Original author: ksuzuki
///////////////////////////////////////////////////////////

#if !defined FailDetector_H
#define FailDetector_H

#include "Detector.h"
#include "History.h"

/**
 * 転倒検知
 */
class FailDetector : public Detector
{
private:
    int mTimeCounter; //!< タイムカウンタ
    int mPrevLeftCount; //!< 前回の左モータエンコーダ値
    int mPrevRightCount; //!< 前回の右モータエンコーダ値
    // int mSampleCount; // FAILSAFE_SAMPLECOUNT
    // int mThreshold; //!< FAILSAFE_THRESHOLD
public:
	FailDetector();
	~FailDetector(){}
	bool detect();
};
#endif // !defined FailDetector_H

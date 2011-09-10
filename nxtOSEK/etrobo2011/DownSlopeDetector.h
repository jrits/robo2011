///////////////////////////////////////////////////////////
//  DownSlopeDetector.h
//  Implementation of the Class DownSlopeDetector
//  Created on:      10-9-2010 19:42:51
//  Original author: skawasak
///////////////////////////////////////////////////////////

#if !defined DOWNSLOPEDETECTOR_H
#define DOWNSLOPEDETECTOR_H

#include "Detector.h"
#include "History.h"

/**
 * 下り坂検知
 */
class DownSlopeDetector : public Detector
{
public:
    DownSlopeDetector(){}
    ~DownSlopeDetector(){}
    bool detect();
};
#endif // !defined DOWNSLOPEDETECTOR_H

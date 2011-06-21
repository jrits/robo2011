///////////////////////////////////////////////////////////
//  DownSlopeDetector.cpp
//  Implementation of the Class DownSlopeDetector
//  Created on:      10-9-2010 19:42:52
//  Original author: skawasak
///////////////////////////////////////////////////////////

#include "DownSlopeDetector.h"
#include "factory.h"

/**
 * ‰º‚èâŒŸ’m‚·‚é
 *
 * @retval true ŒŸ’m‚µ‚½
 * @retval false ŒŸ’m‚µ‚È‚©‚Á‚½
 */
bool DownSlopeDetector::detect()
{
	float accR = mRightMotorSpeedHistory.calcDifference();
	float accL = mLeftMotorSpeedHistory.calcDifference();
	return (accR > DOWNSLOPE_THRESHOLD) && (accL > DOWNSLOPE_THRESHOLD);
}

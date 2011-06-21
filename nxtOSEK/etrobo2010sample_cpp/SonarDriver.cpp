//
// SonarDriver.cpp
//

#include "SonarDriver.h"
using namespace ecrobot;

//=============================================================================
// Update driver status
void SonarDriver::update(void)
{
	mDriverReq = Driver::NO_REQ;
	if (mObstacle)
	{
		mDriverReq = Driver::DRIVE;
	}
}

//=============================================================================
// Check obstacles within the specified range by using a Sonar sensor
void SonarDriver::checkObstacles(SonarSensor& sonar, S32 range)
{
	mObstacle = false;
	if (sonar.getDistance() <= range)
	{
		mObstacle = true;
	}
}

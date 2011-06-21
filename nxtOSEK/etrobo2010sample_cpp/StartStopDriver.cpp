//
// StartStopDriver.cpp
//

#include "StartStopDriver.h"
using namespace ecrobot;

//=============================================================================
// Update driver status
void StartStopDriver::update(void)
{
	// driver state machine
	switch(mDriverReq)
	{
		case Driver::STOP:
			if (mInputFalseToTrue)
			{
				mDriverReq = Driver::START;
				mInputFalseToTrue = false;
			}
			break;
		case Driver::NO_REQ:
			if (mInputFalseToTrue)
			{
				mDriverReq = Driver::STOP;
				mInputFalseToTrue = false;
			}
			break;
		case Driver::START:
		default:
			mDriverReq = Driver::NO_REQ;
			break;
	}
}

//=============================================================================
// Check boolean typed input (e.g Touch Sensor) to trigger start/stop of a robot
void StartStopDriver::checkInput(bool input)
{
	mInputFalseToTrue = false;
	if (input && !mInputState)
	{
		mInputFalseToTrue = true;
	}
	mInputState = input;
}


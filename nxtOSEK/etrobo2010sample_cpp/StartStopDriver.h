//
// StartStopDriver.h
//

#ifndef STARTSTOPDRIVER_H_
#define STARTSTOPDRIVER_H_

#include "Driver.h"

extern "C"
{
	#include "ecrobot_interface.h"
};

/**
 * Robot start/stop control driver class. When an input is changed from false to true, it triggers start/stop of a robot.
 */
class StartStopDriver: public Driver
{
public:
	/**
	 * Constructor.
	 */
	StartStopDriver(): mInputFalseToTrue(false), mInputState(false) { mDriverReq = Driver::STOP; }

	/**
	 * Update driver status
	 */
	virtual void update(void);

	/**
	 * Check boolean typed input (e.g Touch Sensor) to trigger start/stop of a robot
	 * @param input Boolean typed input
	 */
	void checkInput(bool input);

private:
	bool mInputFalseToTrue;
	bool mInputState;
};

#endif /* STARTSTOPDRIVER_H_ */

//
// SonarDriver.h
//

#ifndef SONARDRIVER_H_
#define SONARDRIVER_H_

#include "SonarSensor.h"
#include "Vector.h"
using namespace ecrobot;

#include "Driver.h"

extern "C"
{
	#include "ecrobot_interface.h"
};

/**
 * Sonar driver class which detects obstacle in front of the robot and stop the robot to avoid it.
 */
class SonarDriver: public Driver
{
public:
	/**
	 * Default check range in cm to avoid obstacles.
	 */
	static const S32 DEFAULT_RANGE = 30;

	/**
	 * Constructor.
	 */
	SonarDriver(void): mObstacle(false) {}

	/**
	 * Update driver status
	 */
	virtual void update(void);

	/**
	 * Check obstacles by using a Sonar Sensor
	 * Note that this function must be called with longer than 40msec period due to physical characteristics of sonar sensor.
	 * @param sonar Sonar sensor object
	 * @param range Sonar sensor check range in cm to avoid obstacles
	 */
	void checkObstacles(SonarSensor& sonar, S32 range = DEFAULT_RANGE);

private:
	bool mObstacle;
};

#endif /*SONARDRIVER_H_*/

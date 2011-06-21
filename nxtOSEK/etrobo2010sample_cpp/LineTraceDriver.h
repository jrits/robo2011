//
// LineTraceDriver.h
//

#ifndef LINETRACEDRIVER_H_
#define LINETRACEDRIVER_H_

#include "Vector.h"
#include "LightSensor.h"
using namespace ecrobot;

#include "Driver.h"

/**
 * Line tracing driver class
 */
class LineTraceDriver: public Driver
{
public:
	/**
	 * Default light sensor value for white color
	 */
	static const S16 DEFAULT_BLACK = 700;

	/**
	 * Default light sensor value for black color
	 */
	static const S16 DEFAULT_WHITE = 500;

	/**
	 * Constructor.
	 */
	LineTraceDriver(LightSensor& light, S16 black=DEFAULT_BLACK, S16 white=DEFAULT_WHITE):
		mrLight(light), mBlack(black), mWhite(white) {	mDriverReq = Driver::DRIVE; }

	/**
	 * Update driver status
	 */
	virtual void update(void);

	/**
	 * Set light sensor value for black and white color.
	 * @param black Light sensor value for black
	 * @param white Light sensor value for white
	 */
	void LineTraceDriver::setColorRef(S16 black, S16 white)
	{
		mBlack = black;
		mWhite = white;
	}

private:
	LightSensor& mrLight;
	S16 mBlack;
	S16 mWhite;
};

#endif

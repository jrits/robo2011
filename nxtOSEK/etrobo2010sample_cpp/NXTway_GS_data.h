//
// NXTway_GS_data.h
//

#ifndef NXTWAY_GS_DATA_H_
#define NXTWAY_GS_DATA_H_

#include "Vector.h"
using namespace ecrobot;

extern "C"
{
	#include "ecrobot_interface.h"
};

/**
 * NXTway-GS internal data structure
 */
struct NXTway_GS_data
{
public:
	/**
	 * Motor encoder count vector (mX:Left motor, mY:Right motor)
	 */
	VectorT<S32> motorCount;

	/**
	 * Command vector (mX:Forward, mY:Turn)
	 */
	VectorT<S16> command;

	/**
	 * Motor PWM output vector (mX:Left motor, mY:Right motor)
	 */
    VectorT<S8> motorPwm;

    /**
     * Gyro sensor data
     */
	S16 gyroData;

	/**
	 * Gyro sensor offset data
	 */
	S16 gyroOffset;

	/**
	 * Battery voltage in mV
	 */
	S16 battMv;
};

#endif /* NXTWAY_GS_DATA_H_ */

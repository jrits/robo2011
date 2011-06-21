//
// NXTway_GS.h
//

#ifndef NXTWAY_GS_H_
#define NXTWAY_GS_H_

#include "Vector.h"
#include "Nxt.h"
#include "GyroSensor.h"
#include "Motor.h"
using namespace ecrobot;

extern "C"
{
	#include "balancer.h"
};

class NXTway_GS
{
private:
	const Nxt& mrNxt;
	const GyroSensor& mrGyro;
	Motor& mrMotorL;
	Motor& mrMotorR;

	U16 mGyroOffset;
	
public:
	NXTway_GS(const Nxt& nxt, const GyroSensor& gyro, Motor& motorL, Motor& motorR);
	
	void reset(S16 offset=GyroSensor::DEFAULT_OFFSET);

	void setGyroOffset(S16 offset);

	void drive(VectorT<S8> direction);

	S8 pwmL, pwmR;
};

#endif


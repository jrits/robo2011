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

#include "Driver.h"


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
	
	~NXTway_GS(){}

	// 倒立振子制御の初期化
	void reset(S16 offset=GyroSensor::DEFAULT_OFFSET);

	// ジャイロセンサオフセット値のセット
	void setGyroOffset(S16 offset);

	// ドライバー命令による2輪倒立振子走行体の制御
	void drive(Driver* driver);
};

#endif


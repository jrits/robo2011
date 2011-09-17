//
// TripodAngleTrace.cpp
//
#include "TripodAngleTrace.h"
#include "factory.h"
extern TripodActivator mTripodActivator;
extern bool gDoForwardPid;

/**
 * コンストラクタ
 */
TripodAngleTrace::TripodAngleTrace()
{
	//mTargetAngle = 0;
}

/**
 * スキルを発揮してロボットを動かす。
 *
 * calcCommand を用いて走行ベクトルを決定し、制御機器(TripodActivator)を操作する。
 */
void TripodAngleTrace::execute()
{
    VectorT<float> command = calcCommand();
    mTripodActivator.run(command);//制御機器にセット
}

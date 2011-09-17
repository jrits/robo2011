//
// TripodCoordinateTrace.cpp
//
#include "TripodCoordinateTrace.h"
#include "factory.h"
extern TripodActivator mTripodActivator;
extern bool gDoForwardPid;

/**
 * コンストラクタ
 */
TripodCoordinateTrace::TripodCoordinateTrace()
{
    //mTargetCoordinate = MakePoint(0, 0);
    //setForward(100);
    //mAllowableError = 100.0; // mm
}

/**
 * スキルを発揮してロボットを動かす。
 *
 * calcCommand を用いて走行ベクトルを決定し、制御機器(TripodActivator)を操作する。
 */
void TripodCoordinateTrace::execute()
{
    VectorT<float> command = calcCommand();
    mTripodActivator.run(command);//制御機器にセット
}

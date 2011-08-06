//
// TripodLineTrace.cpp
//
#include "TripodLineTrace.h"
#include "factory.h"
extern TripodActivator mTripodActivator;

/**
 * コンストラクタ
 *
 * @param[in] black 黒
 * @param[in] white 白
 * @param[in] threshold 白黒閾値
 */
TripodLineTrace::TripodLineTrace(float black, float white, float threshold) :
    LineTrace(black, white, threshold)
{
}

/**
 * スキルを発揮してロボットを動かす。
 *
 * calcCommand を用いて走行ベクトルを決定し、制御機器(TripodActivator)を操作する。
 */
void TripodLineTrace::execute()
{
    VectorT<float> command = calcCommand();
    mTripodActivator.run(command);//制御機器にセット
    //mTripodActivator.runWithPid(command); //フォワードPID越しに運転
}

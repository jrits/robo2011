//
// TripodLineTrace.cpp
//
#include "TripodLineTrace.h"
#include "factory.h"
extern TripodActivator mTripodActivator;
extern bool gDoForwardPid;

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
<<<<<<< HEAD
  mBlack = black;
  mWhite = white;
  mLineThreshold = threshold;
  mInitForward = INIT_FORWARD;
  mInitDuration = INIT_SAMPLECOUNT;
  mUseOnoff = false;
  setForward(FORWARD);
  reset();
=======
>>>>>>> 65e92d601805257b1fa53b85325675cf5aeb0c84
}

/**
 * スキルを発揮してロボットを動かす。
 *
 * calcCommand を用いて走行ベクトルを決定し、制御機器(TripodActivator)を操作する。
 */
void TripodLineTrace::execute()
{
    VectorT<float> command = calcCommand();
    if (gDoForwardPid) {
        mTripodActivator.runWithPid(command); //フォワードPID越しに運転
    } else {
        mTripodActivator.run(command);//制御機器にセット
    }
}

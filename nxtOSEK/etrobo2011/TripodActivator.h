//
// TripodActivator.h
//
#ifndef TripodActivator_H_
#define TripodActivator_H_

#include "constants.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Vector.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C"
{
#include "balancer.h"
};

/**
 * 制御機器(ハンドル、アクセル、ブレーキ)
 *
 * Skill によって操作される。
 */
class TripodActivator
{
private:
  Motor &mLeftMotor; //!< 左モータ
  Motor &mRightMotor; //!< 右モータ
  GyroSensor &mGyroSensor; //!< ジャイロセンサ
  Nxt &mNxt; //!< NXTオブジェクト
  int mGyroOffset; //!< ジャイロセンサオフセット値
public:
  TripodActivator(Motor &leftMotor, 
                  Motor &rightMotor, 
                  GyroSensor &gyroSensor, 
                  Nxt &nxt);
  ~TripodActivator(){}
  void reset(int gyroOffset);
  void run(VectorT<F32> command);
  void stop();
private:
  // Vector<S8> balanceControl(VectorT<F32> command); // balancer_control を直に呼ぶことに
};
#endif

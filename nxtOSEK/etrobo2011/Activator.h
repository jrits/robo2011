//
// Activator.h
//
#ifndef Activator_H_
#define Activator_H_

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
 * 倒立振子制御機器(ハンドル、アクセル、ブレーキ)
 *
 * Skill によって操作される。
 */
class Activator
{
protected:
	Motor &mLeftMotor; //!< 左モータ
	Motor &mRightMotor; //!< 右モータ
	GyroSensor &mGyroSensor; //!< ジャイロセンサ
	Nxt &mNxt; //!< NXTオブジェクト
    int mGyroOffset; //!< ジャイロセンサオフセット値
public:
	Activator(Motor &leftMotor, 
              Motor &rightMotor, 
              GyroSensor &gyroSensor, 
              Nxt &nxt);
	~Activator(){}
    // パラメターの初期化
    void reset(int gyroOffset);
    // 走行。ハンドル、アクセスの操作。
	void run(VectorT<F32> command);
    // フォワードPID、ターンPID(@todo)を利用した走行
	void runWithPid(VectorT<F32> speed);
    // 停止
	void stop();
protected:
    float mTargetSpeed;
    float mCurrentForward;
    float forwardPid(float targetSpeed);
};
#endif

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
 * 制御機器(ハンドル、アクセル、ブレーキ)
 *
 * Skill によって操作される。
 */
class Activator
{
private:
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
    void reset(int gyroOffset);
    void run(VectorT<F32> command);
    void stop();
    void slow();
private:
    // Vector<S8> balanceControl(VectorT<F32> command); // balancer_control を直に呼ぶことに
};
#endif

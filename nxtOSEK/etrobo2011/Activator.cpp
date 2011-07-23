//
// Activator.cpp
//
#include "Activator.h"

/**
 * コンストラクタ
 *
 * @param[in] leftMotor 左モータ
 * @param[in] rightMotor 右モータ
 * @param[in] gyroSensor ジャイロセンサ
 * @param[in] nxt NXTオブジェクト
 */
Activator::Activator(Motor &leftMotor, 
                     Motor &rightMotor, 
                     GyroSensor &gyroSensor, 
                     Nxt &nxt) :
    mLeftMotor(leftMotor), 
    mRightMotor(rightMotor), 
    mGyroSensor(gyroSensor), 
    mNxt(nxt)
{
    mGyroOffset = USER_GYRO_OFFSET; //オフセット値を初期化
}

/**
 * パラメータの初期化
 *
 * @param[in] gyroOffset ジャイロオフセット
 */
void Activator::reset(int gyroOffset)
{
    mGyroOffset = gyroOffset;
}
/**
 * ハンドル、アクセルの操作。
 *
 * バランス制御は制御機器(Activator)が自動的に行ってくれる。
 *
 * @param[in] command 走行ベクトル
 */
void Activator::run(VectorT<F32> command)
{
    S8 pwmL, pwmR;
    
    balance_control(
        (F32)command.mX, // 前後進命令
        (F32)command.mY, // 旋回命令
        (F32)mGyroSensor.get(),
        (F32)mGyroOffset,
        (F32)mLeftMotor.getCount(),
        (F32)mRightMotor.getCount(),
        (F32)mNxt.getBattMv(),
        &pwmL,
        &pwmR);

    PWMR = pwmR;
    PWML = pwmL;
    
    if (! DESK_DEBUG) {
        mLeftMotor.setPWM(pwmL);
        mRightMotor.setPWM(pwmR);
    }
}

/**
 * ブレーキの操作。
 *
 * バランスを制御は行わない。モータを止めるだけ。
 *
 * @return -
 */
void Activator::stop()
{
    mLeftMotor.setPWM(0);
    mRightMotor.setPWM(0);
    mLeftMotor.setBrake(true);
    mRightMotor.setBrake(true);
}

/**
 * ブレーキ走行(実験中)。
 *
 * 
 *
 * @return -
 */
void Activator::slow()
{
    mLeftMotor.setPWM((S8)(PWML));
    mRightMotor.setPWM((S8)(PWMR));
}


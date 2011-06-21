//
// NXTway_GS.cpp
//

#include "NXTway_GS.h"

NXTway_GS::NXTway_GS(const Nxt& nxt, const GyroSensor& gyro, Motor& motorL, Motor& motorR)
:
    mrNxt(nxt),
    mrGyro(gyro),
    mrMotorL(motorL),
    mrMotorR(motorR)
{
    this->reset();
}

//=============================================================================
// 制御初期化
void NXTway_GS::reset(S16 offset)
{
    // モータリセット
    mrMotorL.reset();
    mrMotorR.reset();

    // 倒立振子制御の初期化
    balance_init();

    this->setGyroOffset(offset);
}

//=============================================================================
// ジャイロセンサオフセット値の設定
void NXTway_GS::setGyroOffset(S16 offset)
{
    mGyroOffset = offset;
}

//=============================================================================
// ドライバー命令による2輪倒立振子走行体の制御
void NXTway_GS::drive(Driver* driver)
{
    S8 pwmL, pwmR;
	
    VectorT<S8> command = driver->getCommand();

    balance_control(
        (F32)command.mX, // 前後進命令
        (F32)command.mY, // 旋回命令
        (F32)mrGyro.get(),
        (F32)mGyroOffset,
        (F32)mrMotorL.getCount(),
        (F32)mrMotorR.getCount(),
        (F32)mrNxt.getBattMv(),
        &pwmL,
        &pwmR);
		
    mrMotorL.setPWM(pwmL);
    mrMotorR.setPWM(pwmR);
}

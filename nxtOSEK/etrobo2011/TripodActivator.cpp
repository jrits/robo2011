//
// TripodActivator.cpp
//
#include "TripodActivator.h"
#include "constants.h"
#include "factory.h"
extern bool gDoForwardPid;

/**
 * コンストラクタ
 *
 * @param[in] leftMotor 左モータ
 * @param[in] rightMotor 右モータ
 * @param[in] gyroSensor ジャイロセンサ
 * @param[in] nxt NXTオブジェクト
 */
TripodActivator::TripodActivator(Motor &leftMotor, 
                                 Motor &rightMotor, 
                                 GyroSensor &gyroSensor, 
                                 Nxt &nxt) :
    Activator(leftMotor, rightMotor, gyroSensor, nxt)
{
}

/**
 * 走行。ハンドル、アクセルの操作。
 *
 * @param[in] command 走行ベクトル(forward, turn)
 */
void TripodActivator::run(VectorT<F32> command)
{
    float pwm_L, pwm_R;

    // フォワードPID
    if (gDoForwardPid) {
        command.mX = forwardPid(command.mX);
    }

    // @todo: balance_control と同じ入力値なら同じぐらいの出力値になるようにしたい
    //pwm_L = command.mX + (command.mY > 0 ? command.mY : 0) * 0.5;
    //pwm_R = command.mX + (-command.mY > 0 ? -command.mY : 0) * 0.5;
    pwm_L = command.mX + command.mY * 0.5;
    pwm_R = command.mX - command.mY * 0.5;

    if (! DESK_DEBUG) {
        mLeftMotor.setPWM((S8)(MIN(MAX(pwm_L, -128), 127)));
        mRightMotor.setPWM((S8)(MIN(MAX(pwm_R, -128), 127)));
    }

    // tail_control
    tail_control(TAIL_ANGLE_TRIPOD_DRIVE);

#if 0 // DEBUG
    static int count = 0;
    if (count++ > 5) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "TripodActivator");
        lcd.putf("dn", (int)command.mX);
        lcd.putf("dn", (int)command.mY);
        lcd.putf("dn", (int)pwm_L);
        lcd.putf("dn", (int)pwm_R);
        lcd.disp();
    }
#endif
}

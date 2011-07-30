//
// Activator.cpp
//
#include "Activator.h"
#include "Pid.h"
#include "factory.h"

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

    float forward = forwardPid(command.mX);

	balance_control(
        (F32)command.mY, // 前後進命令
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
 * @return -
 */
void Activator::slow()
{
	mLeftMotor.setPWM((S8)(PWML));
	mRightMotor.setPWM((S8)(PWMR));
}

// 調節方法: 地面に設置させずにバランサーを空転。その時の forward 値が100になるように調節。
// forwardPid 内の LCD 表示に計算式が書いてあるので、そのメッセージ通りに調節。
Pid mForwardPid(7.7, 0.0, 0.0);

/**
 * フォワードPID
 *
 * 走行スピードをPID制御する。
 *
 * @param speed 期待するスピード。平坦な道では forward == speed。
 * @return フォワード値
 */
float Activator::forwardPid(float speed)
{
    // 変化量
    float leftSpeed  = mLeftMotorHistory.get() - mLeftMotorHistory.get(-10);
    float rightSpeed = mRightMotorHistory.get() - mRightMotorHistory.get(-10);
    float currentSpeed = (leftSpeed + rightSpeed)/2.0;

    // P
    float P = speed - currentSpeed;

    // PID制御
    float forward = mForwardPid.control(P);

#if 1 // DEBUG
    {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "ForwardPid");
        lcd.putf("dn", (int)forward);
        lcd.putf("sn", "Set KP as");
        lcd.putf("d", (int)((100/P)*100));
        lcd.putf("sn", "/100");
        lcd.disp();
    }
#endif

    return forward;
}




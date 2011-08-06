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
    mTargetSpeed = 0.0;
    mCurrentForward = 0.0;
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
 * 走行。ハンドル、アクセルの操作。
 *
 * @param[in] command 走行ベクトル(forward, turn)
 */
void Activator::run(VectorT<F32> command)
{
	S8 pwm_L, pwm_R;

    // C++ バージョンだとなぜか mActivator.run() で動かないのでとりあえず。
     balance_control(
         (float)command.mX,							 /* 前後進命令(+:前進, -:後進) */
         (float)command.mY,							 /* 旋回命令(+:右旋回, -:左旋回) */
         (float)ecrobot_get_gyro_sensor(NXT_PORT_S1), /* ジャイロセンサ値 */
         (float)USER_GYRO_OFFSET,                     /* ジャイロセンサオフセット値 */
         (float)nxt_motor_get_count(NXT_PORT_C),		 /* 左モータ回転角度[deg] */
         (float)nxt_motor_get_count(NXT_PORT_B),		 /* 右モータ回転角度[deg] */
         (float)ecrobot_get_battery_voltage(),		 /* バッテリ電圧[mV] */
         &pwm_L,										 /* 左モータPWM出力値 */
         &pwm_R);									 /* 右モータPWM出力値 */

     if (! DESK_DEBUG) {
         nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); /* 左モータPWM出力セット(-100～100) */
         nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); /* 右モータPWM出力セット(-100～100) */
     }

	// balance_control(
    //     (F32)command.mY, // 前後進命令
    //     (F32)command.mY, // 旋回命令
    //     (F32)mGyroSensor.get(),
    //     (F32)mGyroOffset,
    //     (F32)mLeftMotor.getCount(),
    //     (F32)mRightMotor.getCount(),
    //     (F32)mNxt.getBattMv(),
    //     &pwm_L,
    //     &pwm_R);
	
    // if (! DESK_DEBUG) {
    //     mLeftMotor.setPWM(pwm_L);
    //     mRightMotor.setPWM(pwm_R);
    // }

#if 0// DEBUG
    {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "Activator::run");
        lcd.putf("dn", (int)command.mX);
        lcd.putf("dn", (int)command.mY);
        lcd.putf("dn", (int)pwm_L);
        lcd.putf("dn", (int)pwm_R);
        lcd.disp();
    }
#endif

}

/**
 * フォワードPID、ターンPID(@todo)を利用した走行
 *
 * @param[in] speed 目標走行スピード(encode/sec)
 */
void Activator::runWithPid(VectorT<F32> speed)
{
    VectorT<F32> command;
    command.mX = forwardPid(speed.mX);
    command.mY = speed.mY; // turnPid(speed.mY); // @todo
    run(command);
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

// 調節方法: 実際に走らせて調節。
Pid mForwardPid(0.0001, 0.0, 0.0);

/**
 * フォワードPID
 *
 * 走行スピードをPID制御する。
 *
 * @param speed 期待するスピード(encode/sec)。大体720(２回転)でマックススピード？
 * @return フォワード値
 * @todo スピードの単位をcm/secにする
 */
float Activator::forwardPid(float targetSpeed)
{
    // 初期化(初期化関数を作るのが面倒だったのでここで)
    if (targetSpeed != mTargetSpeed) {
        mTargetSpeed = targetSpeed;
        // この基準値からPIDで微調整する。係数は forward 100 で 720(２回転)/sec ぐらいと予想して決めうち。
        mCurrentForward = (1/7.2) * mTargetSpeed;
    }
    
    // 変化量(encode/sec)。直前だとスピード0の可能性もあるため、ある程度時間間隔(5count)をもたせている。
    float leftSpeed    = (mLeftMotorHistory.get()  - mLeftMotorHistory.get(-5))  / (0.004 * 5);
    float rightSpeed   = (mRightMotorHistory.get() - mRightMotorHistory.get(-5)) / (0.004 * 5);
    float currentSpeed = (leftSpeed + rightSpeed)/2.0;

    // PID制御
    float P = targetSpeed - currentSpeed;
    mCurrentForward += mForwardPid.control(P);
    mCurrentForward = MAX(MIN(100, mCurrentForward), -100);

#if 1 // DEBUG
    {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "ForwardPid");
        lcd.putf("dn", (int)targetSpeed);
        lcd.putf("dn", (int)currentSpeed);
        lcd.putf("dn", (int)mCurrentForward);
        lcd.putf("dn", (int)((1/7.2) * mTargetSpeed));
        lcd.disp();
    }
#endif

    return mCurrentForward;
}




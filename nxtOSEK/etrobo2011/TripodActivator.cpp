//
// TripodActivator.cpp
//
#include "TripodActivator.h"
#include "constants.h"
#include "factory.h"

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
void TripodActivator::reset(int gyroOffset)
{
  mGyroOffset = gyroOffset;
}
/**
 * ハンドル、アクセルの操作。
 *
 * バランス制御は制御機器(TripodActivator)が自動的に行ってくれる。
 *
 * @param[in] command 走行ベクトル
 */
void TripodActivator::run(VectorT<F32> command)
{
  float pwmL, pwmR;

//   balance_control(
//     (F32)command.mX, // 前後進命令
//     (F32)command.mY, // 旋回命令
//     (F32)mGyroSensor.get(),
//     (F32)mGyroOffset,
//     (F32)mLeftMotor.getCount(),
//     (F32)mRightMotor.getCount(),
//     (F32)mNxt.getBattMv(),
//     &pwmL,
//     &pwmR);

  // @todo: balance_control と同じ入力値なら同じぐらいの出力値になるようにしたい
  pwmL = command.mX + (command.mY > 0 ? command.mY : 0) * 0.3;
  pwmR = command.mX + (-command.mY > 0 ? -command.mY : 0) * 0.3;

  if (! DESK_DEBUG) {
    mLeftMotor.setPWM((S8)(MIN(MAX(pwmL, -128), 127)));
    mRightMotor.setPWM((S8)(MIN(MAX(pwmR, -128), 127)));
  }

#if 1 // DEBUG
  static int count = 0;
  if (count++ > 5) {
    Lcd lcd;
    lcd.clear();
    lcd.putf("sn", "TripodActivator");
    lcd.putf("dn", (int)command.mX);
    lcd.putf("dn", (int)command.mY);
    lcd.putf("dn", (int)pwmL);
    lcd.putf("dn", (int)pwmR);
    lcd.disp();
  }
#endif
}

/**
 * ブレーキの操作。
 *
 * バランスを制御は行わない。モータを止めるだけ。
 *
 * @return -
 */
void TripodActivator::stop()
{
  mLeftMotor.setPWM(0);
  mRightMotor.setPWM(0);
  mLeftMotor.setBrake(true);
  mRightMotor.setBrake(true);
}


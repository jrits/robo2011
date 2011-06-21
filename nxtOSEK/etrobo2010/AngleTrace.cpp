//
// AngleTrace.cpp
//
#include "AngleTrace.h"
#include "factory.h"

/**
 * コンストラクタ
 */
AngleTrace::AngleTrace()
{
    mTargetAngle = 0;
}
	
/**
 * 目標角度を設定
 *
 * @param[in] targetAngle 目標角度
 */
void AngleTrace::setTargetAngle(float targetAngle)
{
	mTargetAngle = targetAngle;
}

/**
 * 方向転換の許容誤差を設定
 *
 * @param[in] allowableError 許容誤差
 */
void AngleTrace::setAllowableError(float allowableError)
{
	mAllowableError = allowableError;
}

/**
 * 目標角度と現在の角度から、角度トレースを行うのに適切な走行ベクトルを計算する
 *
 * @return 走行ベクトル
 */
VectorT<float> AngleTrace::calcCommand()
{
	gLineTrace = false;
	float direction = 0;
	direction = mGps.getDirection(); //現在の角度

	float P = -(mTargetAngle - direction);
	
	//目標角度と現在角度の差分Pを -180 < P <= 180 の範囲内に再設定
    P = Gps::marge180(P);
	
	//Yは-値で右、+値で左を目指す←モータの接続ポート確認
	float Y = mAnglePid.control(P); //Pid制御後モータ出力補正する
	//Y += OFFSET_Y; //勝ロボのOFFSET_Yは8だったはず
	
	//Pid制御値が大きすぎる場合、ここで修正する
	//→現状だとP(現在角度と目標角度の差分)が2.5度以上か-2.5度以下でこのif分に入る 2010/09/27
	if(Y > 100) Y = 100;
	if(Y < -100) Y = -100;
	
    VectorT<float> command;
    command.mX = mForward;
    command.mY = Y;
	return command;
}

/**
 * 方向転換が完了したかどうかを判定する
 *
 * @retval true 完了
 * @retval false 未完了
 */
bool AngleTrace::isArrived()
{
    // 目標角度と現在角度の差
	float diffDirection = mTargetAngle - mGps.getDirection();
    // [-180, 180] の範囲内に再設定
    diffDirection = Gps::marge180(diffDirection); 

    // 到着判定。[-Error, Error] の範囲内にいた場合は到着とみなす。
    if (fabs(diffDirection) < mAllowableError) { 
        return true;
    }
    else {
        return false;
    }
}

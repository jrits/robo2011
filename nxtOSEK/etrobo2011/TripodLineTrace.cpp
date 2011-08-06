//
// TripodLineTrace.cpp
//
#include "TripodLineTrace.h"
#include "factory.h"

/**
 * コンストラクタ
 *
 * @param[in] black 黒
 * @param[in] white 白
 * @param[in] threshold 白黒閾値
 */
TripodLineTrace::TripodLineTrace(float black, float white, float threshold)
{
	mBlack = black;
	mWhite = white;
	mLineThreshold = threshold;
    mInitForward = INIT_FORWARD;
    mInitDuration = INIT_SAMPLECOUNT;
    mUseOnoff = false;
    setForward(FORWARD);
    reset();
}

/**
 * 状態のリセット
 *
 * 初速から始める
 */
void TripodLineTrace::reset()
{
    mTimeCounter = 0;
}

/**
 * 初速の設定
 *
 * @param[in] initForward 初速
 */
void TripodLineTrace::setInitForward(int initForward)
{
    mInitForward = initForward;
}

/**
 * 初速継続カウンタ期間の設定
 *
 * @param[in] initDuration 初速継続カウンタ期間
 */
void TripodLineTrace::setInitDuration(int initDuration)
{
    mInitDuration = initDuration;
}

/**
 * スキルを発揮してロボットを動かす。
 *
 * calcCommand を用いて走行ベクトルを決定し、制御機器(TripodActivator)を操作する。
 */
void TripodLineTrace::execute()
{
    VectorT<float> command = calcCommand();
    //mTripodActivator.run(command);//制御機器にセット
    mTripodActivator.runWithPid(command); //フォワードPID越しに運転
}

/**
 * 現在の光値から、ライントレースをするのに適切な走行ベクトルを計算する。
 *
 * @return 走行ベクトル
 */
VectorT<float> TripodLineTrace::calcCommand()
{
    // 起動時急ダッシュするため、最初のみスピードをゆるめる
    float X;
    if (mTimeCounter < mInitDuration) {
        mTimeCounter++;
        X = mInitForward;
    }
    else { // 通常スピード
        X = mForward;
    }

#if 0 // DEBUG
    {
        static int count = 0;
        if (count++ % 25 == 0)
        {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "TripodLineTrace");
            lcd.putf("dn", mState);
            lcd.putf("dn", mTimeCounter);
            lcd.putf("dn", (int)X);
            lcd.disp();
        }
    }
#endif
    float Y;
    if (mUseOnoff) { 
        Y = calcCommandTurnByOnOff();
    }
    else {
        Y = calcCommandTurn();
    }
    return VectorT<F32>(X,Y);
}

/**
 * (PID制御)現在の光値から、ライントレースをするのに適切なターン値を計算する。
 *
 * @return ターン値
 */
float TripodLineTrace::calcCommandTurn()
{
	//正規化した光センサ値をPに格納する
	float P = this->lightValueNormalization();
	//Pid制御
    float Y = mLightPid.control(P);
	
	//ラインの右側をトレースするか左側をトレースするかで旋回方向が決まる
	if(TRACE_EDGE == LEFT ) Y *= -1;
	if(TRACE_EDGE == RIGHT) Y *=  1;
    
    return Y;
}

/**
 * 正規化した光センサの値を取得する
 *
 * @return 正規化した光センサの値
 */
float TripodLineTrace::lightValueNormalization()
{
    float L = 0;
	L = mLightSensor.get();
	
    float P = (L - mLineThreshold); // 偏差
    if(L < mLineThreshold){ // 白
        P = P / (mLineThreshold - mWhite); // [-1.0, 1.0] の値に正規化
    }
    else{ // 黒
        P = P / (mBlack - mLineThreshold); // [-1.0, 1.0] の値に正規化
    }
	
	if(P > 1) P = 1;
	if(P < -1) P = -1;
	
    return P;
}

/**
 * ON/OFF制御(true) or PID制御(false)フラグの設定
 *
 * @param[in] useOnoff ON/OFF制御(true) or PID制御(false)フラグ
 */
void TripodLineTrace::setUseOnoff(bool useOnoff)
{
    mUseOnoff = useOnoff;
}

/**
 * (ONOFF制御)現在の光値から、ライントレースをするのに適切なターン値を計算する。
 *
 * @return ターン値
 */
float TripodLineTrace::calcCommandTurnByOnOff()
{
    float P = (mLightSensor.get() - mLineThreshold); // 偏差

	//ONOFF制御
    float Y;
    if (P < 0) { // 白
        Y = -LIGHT_ONOFF_K;
    }
    else { // 黒
        Y = LIGHT_ONOFF_K;
    }
	
	//ラインの右側をトレースするか左側をトレースするかで旋回方向が決まる
	if(TRACE_EDGE == LEFT ) Y *= -1;
	if(TRACE_EDGE == RIGHT) Y *=  1;
    
    return Y;
}

///////////////////////////////////////////////////////////
//  ETsumoDriver.cpp
//  Implementation of the Class ETsumoDriver
//  Created on:      16-7-2011 08:43:43
//  Original author: Takahashi
///////////////////////////////////////////////////////////

#include "ETsumoDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"

#define K_PHIDOT_FOR_MOVE 25.0F
#define K_PHIDOT_FOR_SEARCH 15.0F
#define RIKISHI_FORWARD 30.0F
#define OSHIDASHI_FORWARD 30.0F

#define SUMO_DEBUG 1

//ET相撲サーチ開始座標
float GPS_ETSUMO_SEARCH_X = 4500.0; // ＠todo要再設定
float GPS_ETSUMO_SEARCH_Y = -3400.0 + 800.0; // ＠todo要再設定
float GPS_ETSUMO_SEARCH_DIRECTION = 360.0; // ＠todo要再設定

float GPS_ETSUMO_SEARCH_X_SWEEP_1 = 5000.0; // ＠todo要再設定
float GPS_ETSUMO_SEARCH_Y_SWEEP_1 = -2500.0; // ＠todo要再設定
float GPS_ETSUMO_SEARCH_X_SWEEP_2 = 4300.0; // ＠todo要再設定
float GPS_ETSUMO_SEARCH_Y_SWEEP_2 = -3200.0; // ＠todo要再設定

extern bool gDoSonar; //!< ソナーセンサ発動フラグ
extern bool gSonarIsDetected; //!< 衝立検知の結果
extern int  gSonarTagetDistance;//!< ソナーセンサの距離結果
extern float gSonarTagetAngle;


/**
 * コンストラクタ
 */
ETsumoDriver::ETsumoDriver()    //初期値の設定
{
    mTimeCounter = 0;
    mInitState = false;             
    mState = ETsumoDriver::INIT;  //ステート
    mSearchPoint = 1;
}

/**
 * デストラクタ
 */
ETsumoDriver::~ETsumoDriver(){}

/**
 * 走行する。ET相撲を攻略する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
 

bool ETsumoDriver::drive()
{
#if 0 //臨時スイープ 調整中（まともに走りません＞＜）
    K_THETADOT = 6.5F;
    K_PHIDOT = 25.0F;
    if (!mInitState) {
        if(mSearchPoint == 1){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_1; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_1;}
        if(mSearchPoint == 2){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_2; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_2;mGps.adjustDirection(mGps.getDirection() + 30);}
        if(mSearchPoint == 3){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_1; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_1 -150;mGps.adjustDirection(mGps.getDirection() + 30);}
        if(mSearchPoint == 4){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_2 + 150; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_2;}
        if(mSearchPoint == 5){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_1; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_1 -300;}
        if(mSearchPoint == 6){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_2 + 300; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_2;}
        if(mSearchPoint == 7){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_1; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_1 -450;}
        if(mSearchPoint == 8){mPoint.X = GPS_ETSUMO_SEARCH_X_SWEEP_2 + 450; mPoint.Y = GPS_ETSUMO_SEARCH_Y_SWEEP_2;}
        if(mSearchPoint > 8)mSearchPoint = 1;
        mTripodCoordinateTrace.setTargetCoordinate(mPoint);//
        mInitState = true;
        mIsArrived = false;
        mTripodCoordinateTrace.setAllowableError(30);
        mTripodCoordinateTrace.setForward(50);
        mTimeCounter = 0;
    }
    if (mTripodCoordinateTrace.isArrived())
    {
        mInitState = false;
        mTimeCounter = 0;
        mSearchPoint++;
        return 0;
    }
    mTripodCoordinateTrace.execute();
    return 0;
#endif
    
    
    
#if 0 //超信地旋回調査用
    gDoSonar = true;
    K_THETADOT = 6.5F;
    K_PHIDOT = 25.0F;
    if(mTimeCounter < 100000){
        VectorT<float> command(50, 0);//第一要素：進行速度、第二要素：旋回速度
        mActivator.run(command);//制御機器にセット
    }
    else{
        VectorT<float> command2(0, 100);//第一要素：進行速度、第二要素：旋回速度
        mActivator.run(command2);//制御機器にセット
    }
    mTimeCounter++;
    return 0;
#endif
    
#if 0 //超信地旋回調査用
    if (mState == ETsumoDriver::INIT) { // 初期化状態
        gDoSonar = true;
        if(gSonarIsDetected){
            mInitState = true;
            mState = ETsumoDriver::DOHYO_IN;
            updateTargetCoordinates();
            //mTargetX = mTargetTotalX;
            //mTargetY = mTargetTotalY;
            return 0;
        }
        K_THETADOT = 6.5F;
        K_PHIDOT = 20.0F;
        mTimeCounter++;
        
        if (!mInitState) {
            if(mSearchPoint == 1){mPoint.X = GPS_ETSUMO_SEARCH_X; mPoint.Y = GPS_ETSUMO_SEARCH_Y;}
            if(mSearchPoint == 2){mPoint.X = GPS_ETSUMO_SEARCH_X + 200; mPoint.Y = GPS_ETSUMO_SEARCH_Y -200;}
            if(mSearchPoint == 3){mPoint.X = GPS_ETSUMO_SEARCH_X + 400; mPoint.Y = GPS_ETSUMO_SEARCH_Y -400;}
            if(mSearchPoint == 4){mPoint.X = GPS_ETSUMO_SEARCH_X + 600; mPoint.Y = GPS_ETSUMO_SEARCH_Y -600;}
            mCoordinateTrace.setTargetCoordinate(mPoint);//
            mInitState = true;
            mIsArrived = false;
            mCoordinateTrace.setAllowableError(30);
            mCoordinateTrace.setForward(50);
            mTimeCounter = 0;
        }
        if (mCoordinateTrace.isArrived()) {mIsArrived = true; mTimeCounter = 0; mMyAngle = mGps.getDirection();}
        if(mIsArrived){
            if((mGps.getDirection() <= mMyAngle - 720) || (mGps.getDirection() >= mMyAngle +720)){
                mInitState = false;
                mTimeCounter = 0;
                mSearchPoint++;
                return 0;
            }
            else{
                if((mSearchPoint % 2) == 0 ){
                    VectorT<float> command1(0, 100);//第一要素：進行速度、第二要素：旋回速度
                    mActivator.run(command1);//制御機器にセット
                }
                else{
                    VectorT<float> command2(0, -100);//第一要素：進行速度、第二要素：旋回速度
                    mActivator.run(command2);//制御機器にセット
                }
            }
            return 0;
        }
        mCoordinateTrace.execute();
        return 0;
    }
#endif

    //試走会用ライントレース
    /*
    if (mState == ETsumoDriver::INIT) { // 初期化状態
        if (mInitState) {
            gDoSonar = false;
            K_THETADOT = 7.5F;
            K_PHIDOT = 25.0F;
            mLineTrace.setForward(100);
            mInitState = false;
        }
        mLineTrace.execute();
    }
    */
    
    if (mState == ETsumoDriver::INIT) { // 初期化状態
        gDoSonar = false;
        mTimeCounter = 0;
        mOrigK_THETADOT =  K_THETADOT; // 後で戻すために保存
        mOrigK_PHIDOT = K_PHIDOT; // 後で戻すために保存
        K_THETADOT = 6.5F;
        K_PHIDOT = 20.0F;
        mScanState = UNKNOWN;
        mLightSensor.setLamp(0);//試しにライトセンサOFF
        
        //状態遷移
        mInitState = true;
        mState = ETsumoDriver::PREPARE_SPOTSEARCH;
    }

    if (mState == ETsumoDriver::PREPARE_SPOTSEARCH) {
        if (mInitState) {
            gDoSonar = false;
            K_THETADOT = 6.5F;
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X, GPS_ETSUMO_SEARCH_Y));// ＠todo要再設定
            mCoordinateTrace.setForward(50.0);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
        }
        // 移動完了
        if (mCoordinateTrace.isArrived()) {
            mInitState = true;
            //mState = ETsumoDriver::SWINGSEARCH;
            mState = ETsumoDriver::SPOTSEARCH;
        }
        mCoordinateTrace.execute();
    }
    
    
    if (mState == ETsumoDriver::SPOTSEARCH) {
        if (mInitState) {
            gDoSonar = false; // 
            K_PHIDOT = K_PHIDOT_FOR_MOVE;
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(360.0);
            mAngleTrace.setAllowableError(2.0); // 2度
            mInitState = false;
            mIsArrived = false;
            mSonarDetectCount = 0;
            mTargetTotalX = 0;
            mTargetTotalY = 0;
        }
        // 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            gDoSonar = true;
            mIsArrived = true;
        }
        // 方向転換完了してからスポットサーチ開始
        if(mIsArrived && (mTimeCounter % 20 == 0) && (mTimeCounter >= 100)){
            if(gSonarIsDetected){
                mSonarDetectCount++;
                updateTargetCoordinates();
                if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
            }
            else if(mTimeCounter % 100 == 0){
                float setangle = mGps.getDirection() - 100;//この数値は十分大きいため、調整不要のはず
                mAngleTrace.setTargetAngle(setangle);
            }
        }
        
        //スポットサーチ範囲内で探知成功！
        if(mSonarDetectCount >= 3){
            mInitState = true;
            mTargetX = mTargetTotalX / mSonarDetectCount;
            mTargetY = mTargetTotalY / mSonarDetectCount;
            mState = ETsumoDriver::DOHYO_IN;
        }
        //スポットサーチ範囲内にペットボトルが無いことを確認、スイングサーチへ移行
        else if((Gps::marge180(mGps.getDirection()) <= -90.0) && mIsArrived){
            mInitState = true;
            mState = ETsumoDriver::SPOTSEARCH_to_SWINGSEARCH;
        }
        mAngleTrace.execute();
    }
    
    if (mState == ETsumoDriver::SPOTSEARCH_to_SWINGSEARCH) {
        if (mInitState) {
            gDoSonar = false;
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X + 200.0, GPS_ETSUMO_SEARCH_Y - 200.0));// ＠todo要再設定
            mCoordinateTrace.setForward(RIKISHI_FORWARD);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
        }
        // 移動完了
        if (mCoordinateTrace.isArrived()) {
            mInitState = true;
            mState = ETsumoDriver::SWINGSEARCH;
        }
        mCoordinateTrace.execute();
    }
        
    if (mState == ETsumoDriver::SWINGSEARCH) {
        if (mInitState) {
            gDoSonar = false;
            K_PHIDOT = K_PHIDOT_FOR_MOVE;
            mTimeCounter = 0;
            mSonarDetectCount = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(480);
            //mAngleTrace.setTargetAngle(415);
            mAngleTrace.setAllowableError(2.0); // 2度
            mInitState = false;
            mIsArrived = false;
            mTargetTotalX = 0;
            mTargetTotalY = 0;
        }
        // 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
            K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            gDoSonar = true; // ソナー起動
        }
        // 方向転換完了してからスイングサーチ開始
        if(mIsArrived && (mTimeCounter % 20 == 0)){
            if(gSonarIsDetected){
                mSonarDetectCount++;
                updateTargetCoordinates();
                if(SUMO_DEBUG) {mSpeaker.playTone(500, 1, 20);}
            }
            else if((mTimeCounter % 100 == 0) && (mTimeCounter > 100)){
                float setangle = mGps.getDirection() - 100;
                mAngleTrace.setTargetAngle(setangle);
            }
        }
        
        //スイングサーチ範囲内で探知成功！
        if(mSonarDetectCount >= 3){
            mInitState = true;
            mTargetX = mTargetTotalX / mSonarDetectCount;
            mTargetY = mTargetTotalY / mSonarDetectCount;
            mState = ETsumoDriver::DOHYO_IN;
        }
        mAngleTrace.execute();
    }
    
    if (mState == ETsumoDriver::DOHYO_IN) {
        if (mInitState) {
            mTimeCounter = 0;
            K_PHIDOT = 60.0F;
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//ターゲットのアングルを-180〜180で返す
            if((mTargetAngle > -45) && (mTargetAngle < 135)){
                mCoordinateTrace.setTargetCoordinate(MakePoint(mTargetX - 300, mTargetY));
            }
            else{
                mCoordinateTrace.setTargetCoordinate(MakePoint(mTargetX, mTargetY + 300));
            }
            mCoordinateTrace.setForward(RIKISHI_FORWARD);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
            gDoSonar = false; 
        }
        // 移動完了
        if (! mIsArrived && mCoordinateTrace.isArrived()) {
            mInitState = true;
            mState = ETsumoDriver::HAKKE_READY;
        }
        mCoordinateTrace.execute();
    }
    
    
    if (mState == ETsumoDriver::HAKKE_READY) {
        if (mInitState) {
            gDoSonar = false; 
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            K_PHIDOT = K_PHIDOT_FOR_MOVE;
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//ターゲットのアングルを-180〜180で返す
            if((mTargetAngle > -45) && (mTargetAngle < 135)){
                mAngleTrace.setTargetAngle(45);
            }
            else{
                mAngleTrace.setTargetAngle(-45);
            }            
            mAngleTrace.setAllowableError(2.0); // 2度
            mInitState = false;
            mIsArrived = false;
        }
        // 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
            K_PHIDOT = K_PHIDOT_FOR_SEARCH;
        }
        // 方向転換完了してから落ち着くまで待機
        if(mIsArrived && (mTimeCounter > 100)){
            mInitState = true;
            mState = ETsumoDriver::SCAN;
        }
        
        mAngleTrace.execute();
    }
    if (mState == ETsumoDriver::SCAN) {
        if (mInitState) {
            gDoSonar = true; // ソナー起動
            K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            mTimeCounter = 0;
            mSonarDetectCount = 0;
            mFailScanCounter = 0;
            mTargetTotalX = 0;
            mTargetTotalY = 0;
            mAngleTrace.setForward(0);
            mInitState = false;
            mIsArrived = false;
            mScanState = SWINGRIGHT;
        }
        //最初は右回りにスキャン、ターゲットロスト後に左回りにスキャン
        if(mTimeCounter % 20 == 0){
            if(gSonarIsDetected){
                mFailScanCounter = 0; //検知したら検知失敗回数をリセット
                mSonarDetectCount++;
                updateTargetCoordinates();
                if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
            }
            else{
                mFailScanCounter++;
                if(mFailScanCounter > 255){
                    mFailScanCounter = 255;//念のため
                }
            }
            //右回りにスキャン
            if((mTimeCounter % 100 == 0) && (mScanState == SWINGRIGHT)){
                float setangle = mGps.getDirection() - 100;
                mAngleTrace.setTargetAngle(setangle);
            }
            //左回りにスキャン
            else if((mTimeCounter % 100 == 0) && (mScanState == SWINGLEFT)){
                float setangle = mGps.getDirection() + 100;
                mAngleTrace.setTargetAngle(setangle);
            }
        }
        //ターゲットロスト後に状態遷移
        if((mFailScanCounter >= 10) && (mSonarDetectCount >= 2)){
            //右回りなら左回りへ状態遷移
            if(mScanState == SWINGRIGHT){
                mFailScanCounter = 0;
                mPrevSonarDetectCount = mSonarDetectCount; //右回りの時の検知回数を保存
                mSonarDetectCount = 0;                     //右回りの時の検知回数をクリア
                mTimeCounter = 0;
                mScanState = SWINGLEFT;
            }
            //
            else if(mScanState == SWINGLEFT){
                gDoSonar = false;
                mTimeCounter = 0;
                mTargetX = mTargetTotalX / (mSonarDetectCount + mPrevSonarDetectCount);
                mTargetY = mTargetTotalY / (mSonarDetectCount + mPrevSonarDetectCount);
                mInitState = true;
                mState = ETsumoDriver::NOKOTTA_GO;
            }
        }
        mAngleTrace.execute();
    }

    if (mState == ETsumoDriver::NOKOTTA_GO) {
        if (mInitState) {
            gDoSonar = false; //
            K_PHIDOT = 30.0F;
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//ターゲットのアングルを-180〜180で返す
            mAngleTrace.setTargetAngle(mTargetAngle + 0);//左右視力の違い?補正 ＠todo機体依存か要調査
            mAngleTrace.setAllowableError(1.0); // 1度
            mInitState = false;
            mIsArrived = false;
            mOshidashiFlag = false;
        }
        if (! mIsArrived) {
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//精度向上のため、動的にターゲットアングルを更新する
            mAngleTrace.setTargetAngle(mTargetAngle + 0);//左右視力の違い?補正 ＠todo機体依存か要調査
        }
            
        // 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        //ゆっくり押し出し、時々張り手
        if(!mOshidashiFlag && mIsArrived && (mTimeCounter > 100)){
            mAngleTrace.setForward(OSHIDASHI_FORWARD);//＠todoベストな値を要検証
        }
        //押し出し判定
        if((mGps.getXCoordinate() > (GPS_ETSUMO_SEARCH_X + 800)) || (mGps.getYCoordinate() < (GPS_ETSUMO_SEARCH_Y - 800))){//＠todoベストな値を要検証
            mAngleTrace.setForward(-30);//判定が出たらゆっくり後退
            mTimeCounter = 0;
            mOshidashiFlag = true;
            mLightSensor.setLamp(1);//ライトセンサON
        }
        //そっと4秒ほど後退後状態遷移
        if(mOshidashiFlag && (mTimeCounter > 500)){    
            //mState = ETsumoDriver::KACHI_NANORI;
            //mInitState = true;
        }
        mAngleTrace.execute();
    }
    /*メモリが足りない？ためコメントアウト
    if (mState == ETsumoDriver::KACHI_NANORI) {
        if (mInitState) {
            gDoSonar = false;
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(0, 0));
            mCoordinateTrace.setForward(30);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
        }
        // 移動完了
        if (mCoordinateTrace.isArrived()) {
            mInitState = true;
            mState = ETsumoDriver::SPOTSEARCH_to_SWINGSEARCH;
        }
        mCoordinateTrace.execute();
    }
    */
    mTimeCounter++;
    return 0;
}

//発見したペットボトルのおおよその位置を計算し、mTargetTotalX、mTargetTotalYに加える関数
void ETsumoDriver::updateTargetCoordinates()
{
    //
    float rad  = gSonarTagetAngle * 3.1416 / 180.0;
    float x = mGps.getXCoordinate() + gSonarTagetDistance * cosf(rad);
    float y = mGps.getYCoordinate() + gSonarTagetDistance * sinf(rad);
    
    //UPDATE
    mTargetTotalX += x;
    mTargetTotalY += y;

    //以下の計算だと、なぜかロボがこける
    //mTargetX = mTargetX * (c -1.0) / c + x / c;
    //mTargetY = mTargetY * (c -1.0) / c + y / c;
    
    return;
}

//CoordinateTraceより借用、アングルのみを計算させる
//ターゲットのアングルを-180〜180で返す
float ETsumoDriver::calcTargetAngle(float targetX, float targetY)
{
        
    // 現在座標取得
    float currentX = mGps.getXCoordinate();
    float currentY = mGps.getYCoordinate();

    // 現在座標と目標座標との差
    float diffX = targetX - currentX;
    float diffY = targetY - currentY;

    // 目標への角度を算出
    float targetDirection;
    if (diffX == 0.0 && diffY == 0.0) {
        targetDirection = mGps.getDirection();
    }
    else {
        double radian = Gps::atan2(diffY, diffX);
        if (isinf(radian)) targetDirection = mGps.getDirection();
        else targetDirection = Gps::radianToDegree(radian);
    }
    Gps::marge180(targetDirection);
    
    return targetDirection;
}

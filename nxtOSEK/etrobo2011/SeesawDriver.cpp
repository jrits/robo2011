///////////////////////////////////////////////////////////
//  SeesawDriver.cpp
//  Implementation of the Class SeesawDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: masui
///////////////////////////////////////////////////////////

#include "SeesawDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"
#include "Gps.h"

#define RETURNTIMING 375 //値を返すタイミング
#define LIGHTBUF 10 //光センサヒストリのバッファサイズ
#define LATEST 0

/**
 * コンストラクタ
 */
SeesawDriver::SeesawDriver()    //初期値の設定
{
    mTimeCounter = 0;
    mState = SeesawDriver::INIT;  //ステート
    mInitState = true;
    mDoDetectWall = false;

}

/**
 * デストラクタ
 */
SeesawDriver::~SeesawDriver(){}

/**
 * 走行する。シーソーを攻略する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
 

bool SeesawDriver::drive()
{
#if 1
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
    LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
    LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
    LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
//    LOGGER_DATAS32[0] = (S32)(mGyroHistory.calcDifference());
#endif
    
    // 初期化関数を作るのが面倒なのでとりあえずここで
    if (mState == SeesawDriver::INIT) {
        gDoMaimai = true;
        mLightPid.reset(70,0,210);
        K_THETADOT = 7.5F;
        mState = SeesawDriver::BEFORELINETRACE;
        mTimeCounter = 0;
        mDoDetectWall = false;
        mWallDetector.setThreshold(90);
    }
    // 階段前のライントレース。段差にぶつかるまで。
    if (mState == SeesawDriver::BEFORELINETRACE) {
        // 段差検知なしでライントレース
        if (! mDoDetectWall) {
            mLineTrace.setForward(50);
            mLineTrace.execute();
            if((-1100.0 < mGps.getYCoordinate()) &&  (mGps.getYCoordinate() < -800.0)
               && (355 < mGps.getDirection())&& (mGps.getDirection() < 365)) {
                { Speaker s; s.playTone(480, 20, 100); }
                mDoDetectWall = true;
            }
        }
        // 段差検知しながらライントレース
        if (mDoDetectWall) {
            mLineTrace.setForward(50);
            mLineTrace.execute();
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = SeesawDriver::ON0THSTAGE_BACK;
                mInitState = true;
                mPrevDirection = mGps.getDirection(); // 角度記憶
                // ぶつかったら座標補正2011
                mGps.adjustXCoordinate(3350.0);//ぶつかったら座標補正
                mGps.adjustYCoordinate(-903.0);//ぶつかったら座標補正
                mGps.adjustDirection(360);
            }
        }
    }
    // ０段目(階段前)。一旦バックして助走距離をとる
    if (mState == SeesawDriver::ON0THSTAGE_BACK) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mDoDetectWall = false;
            mInitState = false;
        }
        // 一旦バック(フラグ名が適切ではないが気にしないでください)
        if (! mDoDetectWall) {
            { Speaker s; s.playTone(1976, 10, 100); }
            mAngleTrace.setForward(-50);
            mAngleTrace.setTargetAngle(mPrevDirection);
            K_THETADOT = 7.5F;
            mAngleTrace.execute();
            if (mGps.getXCoordinate() < 3060) { // 階段側マーカ始点
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // 一旦ストップ(フラグ名が適切ではないが気にしないでください)
        if (mDoDetectWall) {
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(mPrevDirection);
            K_THETADOT = 7.5F;
            mAngleTrace.execute();
            if (mTimeCounter > 100) { // 100カウント間ストップ
                mState = SeesawDriver::ON0THSTAGE_GO;
                mInitState = true;
            }
            mTimeCounter++;
        }
    }
    // ０段目(階段前)。助走距離をとったのでいざ突入
    if (mState == SeesawDriver::ON0THSTAGE_GO) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mDoDetectWall = false;
            mInitState = false;
        }
        // 突入しようと思った直後に段差検知がtrueになってしまうことがあるのでちょっと進んでおく
        if (! mDoDetectWall) {
            mAngleTrace.setForward(100);
            mAngleTrace.setTargetAngle(mPrevDirection);
            K_THETADOT = 7.5F; // Find! １段目１回で載る絶妙な値
            mAngleTrace.execute();
            if (mGps.getXCoordinate() < 3150.0) { // 階段側マーカ始点
                mDoDetectWall = true;
            }
        }
        // いざ突入
        if (mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET + 10);
            mAngleTrace.execute();
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = SeesawDriver::DETECTINGRUN;
                mInitState = true;
            }
        }
    }
    // シーソーに載ってからバタンとなるまで
    else if(mState == SeesawDriver::DETECTINGRUN){
        if (mInitState) {
            mPrevMotor = mRightMotor.getCount();
            mInitState = false;
            mTimeCounter = 0;
        }
        K_THETADOT = 8.5F;
        mAngleTrace.setTargetAngle(360);
        mAngleTrace.setForward(25);
        mAngleTrace.execute();
        // しばし急ブレーキ
        if(mTimeCounter < 100){
           mActivator.reset(USER_GYRO_OFFSET + 0);
        }
        // バタンとなりました
        if(mRightMotor.getCount() - mPrevMotor >= 445) {
            { Speaker s; s.playTone(261, 40, 100); }
            mState = SeesawDriver::BACKRUN;
            mTimeCounter = 0;
        }
    }
    // ダブルクラッチのバック
    else if(mState == SeesawDriver::BACKRUN){
        // バック
        if(mTimeCounter <= 600){
            K_THETADOT = 7.5F;
            mActivator.reset(USER_GYRO_OFFSET - 5);
            mAngleTrace.setTargetAngle(360);
            mAngleTrace.setForward(-30);
            mAngleTrace.execute();
        }
        // ちょっととまってろ
        else{
            mActivator.reset(USER_GYRO_OFFSET);
            mAngleTrace.setTargetAngle(360);
            mAngleTrace.setForward(5); // 坂なので0でも後ろに下がってしまう
            mAngleTrace.execute();
        }
        // ある程度後ろにバックしました
        if(mRightMotor.getCount() - mPrevMotor <= 270){
            { Speaker s; s.playTone(261, 60, 100); }
            mState = SeesawDriver::FORWARDRUN;
            mTimeCounter = 0;
        }
        //シーソーを超えてしまったらシングルで諦めてライン復帰
        if(mGps.getXCoordinate() >= 3350.0 + 450){
            { Speaker s; s.playTone(261, 40, 100); }
            mState = LINERETURN;
            mTimeCounter = 0;
        }
    }
    // ダブルクラッチの前進
    else if(mState == SeesawDriver::FORWARDRUN){
        // ちょっととまってろ
        if(mTimeCounter <= 750){
            K_THETADOT = 7.5F;
            mAngleTrace.setTargetAngle(360);
            mAngleTrace.setForward(10); // 坂なので0でも後ろに下がってしまう
            mAngleTrace.execute();
        }
        // 前進
        else{
            mAngleTrace.setTargetAngle(360);
            mAngleTrace.setForward(50);
            mAngleTrace.execute();
        }
        //シーソー終了場所から300mm先で状態遷移
        if( 3636.0 + 500.0 < mGps.getXCoordinate()){//@todo 要調整
            { Speaker s; s.playTone(261, 60, 100); }
            mState = SeesawDriver::LINERETURN;
            mTimeCounter = 0;
            K_THETADOT = 7.5F;
        }
    }
    // ライン復帰
    else if (mState == SeesawDriver::LINERETURN) {
        if (mInitState) {
            mInitState = false;
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        // しばしまっすぐ進む
        if (! mDoDetectWall) {
            mAngleTrace.execute();
            if (mTimeCounter > 500) {
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // ライン検知(ちょっと右に向かってまっすぐ)
        if (mDoDetectWall) {
            mAngleTrace.setTargetAngle(mPrevDirection + 10);
            K_THETADOT = 7.5F;
            mAngleTrace.setForward(30);
            mAngleTrace.execute();
            if (mLineDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = SeesawDriver::AFTERLINETRACE;
                mInitState = true;
            }
        }
    }else if (mState == SeesawDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 7.5F;
            mLineTrace.setForward(30);
            mTimeCounter = 0;
            mInitState = false;
        }
        mLineTrace.execute();
        if (mTimeCounter > 250) {
            mLineTrace.setForward(60);
        }
        if (mTimeCounter > 500) {
            mLineTrace.setForward(100);
        }
    }
    mTimeCounter++;
    return mState == SeesawDriver::AFTERLINETRACE; // 終了しました
}



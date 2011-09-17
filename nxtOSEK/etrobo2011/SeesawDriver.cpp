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
    beforeRMS = 0;
    backCounter = 0;
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
    gDoMaimai = true;
    mLightPid.reset(70,0,210);
    //float gyro = mGyroHistory.calcDifference();
#if 1
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
    LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
    LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
    LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
//    LOGGER_DATAS32[0] = (S32)(mGyroHistory.calcDifference());
#endif
    
    if (mState == SeesawDriver::INIT) {
        if((-1100.0 < mGps.getYCoordinate()) &&  (mGps.getYCoordinate() < -800.0)
            && (355 < mGps.getDirection())&& (mGps.getDirection() < 365)
        )
        {
            { Speaker s; s.playTone(480, 20, 100); }
            mWallDetector.setThreshold(90);
            mState = SeesawDriver::BEFORELINETRACE;
            backCounter = 0;
        }else{
            if(500 < backCounter){
                mLineTrace.setForward(50);
            }else{
                mLineTrace.setForward(40);
            }
            K_THETADOT = 7.5F;
            mLineTrace.execute();
            backCounter++;
        }
    }
/*    if(mState == SeesawDriver::BEFOREANGLETRACE){
        mActivator.reset(USER_GYRO_OFFSET + 4);
        K_THETADOT = 7.5F;
        mAngleTrace.setTargetAngle(360);
        mAngleTrace.setForward(100);
        mAngleTrace.execute();
        if(mWallDetector.detect()){
            { Speaker s; s.playTone(261, 20, 100); }
            mState = SeesawDriver::DETECTINGRUN;
            beforeRMS = mRightMotorHistory.get(0);
            backCounter = 0;
            mGps.adjustXCoordinate(3350.0);//ぶつかったら座標補正
            mGps.adjustYCoordinate(-903.0);//ぶつかったら座標補正
            mGps.adjustDirection(360);
            mActivator.reset(USER_GYRO_OFFSET - 15);//急ブレーキ
//          beforeRMS = mRightMotorHistory.get(0);
        }
    }*/
    
//      if (mState == SeesawDriver::INIT) {
//        mState = SeesawDriver::BEFORELINETRACE;
//        mInitState = true;
 //       mWallDetector.setThreshold(110);
//    }
    // 階段前のライントレース。段差にぶつかるまで。
    if (mState == SeesawDriver::BEFORELINETRACE) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mDoDetectWall = true;
            mInitState = false;
        }
        // 段差検知しながらライントレース
        if (mDoDetectWall) {
            { Speaker s; s.playTone(300, 10, 100); }
            mLightPid.reset(50,0,100);
            mLineTrace.setForward(70);
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
            mInitState = false;
            mDoDetectWall = false;
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
            mAngleTrace.execute();
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = SeesawDriver::DETECTINGRUN;
                mInitState = true;
            }
        }
    }
    else if(mState == SeesawDriver::DETECTINGRUN){
        if(backCounter > 100){
           mActivator.reset(USER_GYRO_OFFSET);
        }
        backCounter++;
        K_THETADOT = 8.5F;
        mAngleTrace.setTargetAngle(360);
        mAngleTrace.setForward(25);
        mAngleTrace.execute();
//      if(mTailMotor.getCount() <= 70){
        if(mRightMotorHistory.get(0) - beforeRMS >= 445){
            { Speaker s; s.playTone(261, 40, 100); }
            mState = SeesawDriver::BACKRUN;
            backCounter = 0;
        }
    }else if(mState == SeesawDriver::BACKRUN){
        if(3350.0 + 450 <= mGps.getXCoordinate()){//シーソーを超えてしまったらライン復帰に切り替えてシングル
            { Speaker s; s.playTone(261, 40, 100); }
            mState = LINERETURN;
        }
        if(backCounter <= 600){
//        if(mRightMotorHistory.get(0) - beforeRMS >= 150){//後退する
        K_THETADOT = 7.5F;
        mActivator.reset(USER_GYRO_OFFSET - 5);
        mAngleTrace.setTargetAngle(360);
        mAngleTrace.setForward(-30);
        mAngleTrace.execute();
        }else{
        mActivator.reset(USER_GYRO_OFFSET);
        mAngleTrace.setTargetAngle(360);
        mAngleTrace.setForward(5);
        mAngleTrace.execute();
        }
        backCounter++;
        if(mRightMotorHistory.get(0) - beforeRMS <= 270){
            { Speaker s; s.playTone(261, 60, 100); }
//            mActivator.reset(USER_GYRO_OFFSET + 5);
            mState = SeesawDriver::HEREWEGO;
            backCounter = 0;
        }
    }else if(mState == SeesawDriver::HEREWEGO){
        if(backCounter <= 750){
        K_THETADOT = 7.5F;
        mAngleTrace.setTargetAngle(360);
        mAngleTrace.setForward(10);
        mAngleTrace.execute();
        }else{
        mAngleTrace.setTargetAngle(360);//@todo 要調整 シーソ左側へ降りるように微調整
        mAngleTrace.setForward(50);
        mAngleTrace.execute();
        }
        //シーソー終了場所から300mm先で状態遷移
        if( 3636.0 + 500.0 < mGps.getXCoordinate()){//@todo 要調整
            { Speaker s; s.playTone(261, 60, 100); }
            mState = SeesawDriver::LINERETURN;
            backCounter = 0;
        }
        backCounter++;
    }else if (mState == SeesawDriver::LINERETURN) {
        if (mInitState) {
            mInitState = false;
            K_THETADOT = 7.5F;
        }
        if (mLineDetector.detect()) {
            mState = SeesawDriver::AFTERLINETRACE;
            mInitState = true;
        }
        VectorT<F32> command(15, 5);//@todo 要調整（2010コードより推測）
        mActivator.run(command);
    }else if (mState == SeesawDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 6.5F;
            mLineTrace.setForward(20);
//          mLineTrace.reset();
            mInitState = false;
        }
        if(mTimeCounter > 500){
            K_THETADOT = 7.5F;
            mLineTrace.setForward(100);
        }
        mLineTrace.execute();
        mTimeCounter++;
    }
    return mState == SeesawDriver::AFTERLINETRACE; // 終了しました
}



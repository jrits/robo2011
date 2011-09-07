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
    mInitState = false;
    mState = StairwayDriver::INIT;  //ステート
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
//  float gyro = mGyroHistory.calcDifference();
#if 0
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
//  LOGGER_DATAS16[0] = (S16)();
//  LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
//  LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
//  LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
    LOGGER_DATAS32[0] = (S32)(mGyroHistory.calcDifference());
#endif
    float xCo = mGps.getXCoordinate();
//  float yCo = mGps.getYCoordinate();

//  段差攻略
    if (mState == SeesawDriver::INIT) {
        if(backCounter <= 500){
            mAngleTrace.setTargetAngle(180);
            mAngleTrace.setForward(0);
            mAngleTrace.execute();
            backCounter++;
        }else{
            mWallDetector.setThreshold(110);
//          nxt_motor_set_count(NXT_PORT_A, 0);
            mState = SeesawDriver::BEFOREANGLETRACE;
            backCounter = 0;
        }
    }
    if(mState == SeesawDriver::BEFOREANGLETRACE){
        K_THETADOT = 9.5F;
        mAngleTrace.setTargetAngle(180);
        mAngleTrace.setForward(100);
        mAngleTrace.execute();
        if(mWallDetector.detect()){
            { Speaker s; s.playTone(261, 20, 100); }
            mState = SeesawDriver::DETECTINGRUN;
            beforeRMS = mRightMotorHistory.get(0);
            backCounter = 0;
//          beforeRMS = mRightMotorHistory.get(0);
        }
    }else if(mState == SeesawDriver::DETECTINGRUN){
        K_THETADOT = 8.5F;
        mAngleTrace.setTargetAngle(180);
        mAngleTrace.setForward(30);
        mAngleTrace.execute();
//      if(mTailMotor.getCount() <= 70){
        if(mRightMotorHistory.get(0) - beforeRMS >= 410){
            { Speaker s; s.playTone(261, 40, 100); }
            mState = SeesawDriver::BACKRUN;
            backCounter = 0;
        }
    }else if(mState == SeesawDriver::BACKRUN){
        if(backCounter <= 500){
        K_THETADOT = 5.5F;
        mAngleTrace.setTargetAngle(180);
        mAngleTrace.setForward(-45);
        mAngleTrace.execute();
        }else{
        mAngleTrace.setTargetAngle(180);
        mAngleTrace.setForward(-20);
        mAngleTrace.execute();
        }
        backCounter++;
        if(mRightMotorHistory.get(0) - beforeRMS <= 360){
            { Speaker s; s.playTone(261, 60, 100); }
            mState = SeesawDriver::HEREWEGO;
            backCounter = 0;
        }
    }else if(mState == SeesawDriver::HEREWEGO){
        if(backCounter <= 500){
        mAngleTrace.setTargetAngle(180);
        mAngleTrace.setForward(0);
        mAngleTrace.execute();
        }else{
        mAngleTrace.setTargetAngle(180);
        mAngleTrace.setForward(70);
        mAngleTrace.execute();
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
        VectorT<F32> command(15, -5);
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



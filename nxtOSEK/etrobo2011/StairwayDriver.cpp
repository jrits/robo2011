///////////////////////////////////////////////////////////
//  StairwayDriver.cpp
//  Implementation of the Class StairwayDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: nseo
///////////////////////////////////////////////////////////

#include "StairwayDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"

/**
 * コンストラクタ
 */
StairwayDriver::StairwayDriver()
{
    mState = StairwayDriver::INIT;
    mTimeCounter = 0;
    mInitState = true;
    mDoDetectWall = false;

}

/**
 * 走行する。階段を攻略する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool StairwayDriver::drive()
{
#if 0 // ログ送信
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
    LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
    LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
    LOGGER_DATAS32[3] = (S32)(mGps.getDistance());
#endif
#if 0 // DEBUG
    //DESK_DEBUG = true; // モータを回さないデバグ
    if (mTimeCounter % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "StairwayDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.putf("dn", (S32)(mGyroHistory.calcDifference()));
        lcd.putf("dn", (S32)(K_THETADOT*10));
        lcd.disp();
    }
#endif

    if (mState == StairwayDriver::INIT) {
        mState = StairwayDriver::BEFORELINETRACE;
        mInitState = true;
        mWallDetector.setThreshold(40);
    }
    if (mState == StairwayDriver::BEFORELINETRACE) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mLineTrace.setForward(100);
            K_THETADOT = 7.0F; // Find! 階段前曲線をきれいにライントレースできる絶妙な値
            if (mGps.getXCoordinate() < 4200.0) {//@todo 要調整2011
                K_THETADOT = 11.0F;
            }
            if (mGps.getXCoordinate() < 4100.0) {//@todo 要調整2011
                mPrevDirection = mGps.getDirection();
                mLineTrace.setForward(100);
                mDoDetectWall = true;
                mTimeCounter = 0;
            }
            mLineTrace.execute();
        }
        if (mDoDetectWall) {
            mAngleTrace.setForward(100);
            mAngleTrace.setTargetAngle(mPrevDirection);
            K_THETADOT = 11.0F; // Find! １段目１回で載る絶妙な値
            //if (mTimeCounter > 250 && mWallDetector.detect()) {
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                //mActivator.reset(USER_GYRO_OFFSET + 10); // 大きくして急発進
                mState = StairwayDriver::ON1STSTAGE;
                mInitState = true;
            	
            	mGps.adjustXCoordinate(3975.0);//ぶつかったら座標補正 2011
        		mGps.adjustYCoordinate(-1500.0);//ぶつかったら座標補正 2011
            }
            mAngleTrace.execute();    
        }
    }
    else if (mState == StairwayDriver::ON1STSTAGE) {
        // 段差１に載った直後
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET - 30); // 小さくして急ブレーキ
            K_THETADOT = 6.0F; // Find! ２段目１回で載る絶妙な値
            if (mTimeCounter > 100 && mLeftMotorHistory.calcDifference() < 0) {
                mActivator.reset(USER_GYRO_OFFSET);
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
            mAngleTrace.execute();
        }
        if (mDoDetectWall) {
            if (mLeftMotor.getCount() - mPrevMotor > 270 && 
                mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                //mActivator.reset(USER_GYRO_OFFSET + 20); // 大きくして急発進
                //mActivator.reset(USER_GYRO_OFFSET); // すでに勢いがついている
                mState = StairwayDriver::ON2NDSTAGE;
                mInitState = true;
            }
            mAngleTrace.execute();
        }
        if (mLeftMotor.getCount() < mPrevMotor) { // 戻っちゃった
            mState = StairwayDriver::BEFORELINETRACE;
            mInitState = true;
            mDoDetectWall = true;
        }
    }
    else if (mState == StairwayDriver::ON2NDSTAGE) {
        // 段差２に載った直後
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET - 15); // 小さくして急ブレーキ
            if (mTimeCounter > 25 && mLeftMotorHistory.calcDifference() < 0) {
                //mActivator.reset(USER_GYRO_OFFSET); // これで前傾になってしまう
                mActivator.reset(USER_GYRO_OFFSET - 5); // これでも少し進む
                mAngleTrace.setForward(0); // USER_GYRO_OFFSET - 5 で 0 でも少し進む
                mLineTrace.setForward(0); // USER_GYRO_OFFSET - 5 で 0 でも少し進む
                K_THETADOT = 5.0F;
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
            mAngleTrace.execute();
        }
        // ドスン検知
        if (mDoDetectWall) {
            mAngleTrace.execute();
            if ((((mLeftMotor.getCount() - mPrevMotor) > 360) && mWallDetector.detect())
            	|| (mGps.getXCoordinate() < 3492.0 - 300.0)) //@todo 要調整 座標条件追加 2011
        	{
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::DROPDOWN;
                mInitState = true;
            }
        }
        if (mLeftMotor.getCount() < mPrevMotor) { // 戻っちゃった
            mState = StairwayDriver::BEFORELINETRACE;
            mInitState = true;
            mDoDetectWall = true;
        }
    }
    else if (mState == StairwayDriver::DROPDOWN) {
        // ドスン直後
        if (mInitState) {
            //mActivator.reset(USER_GYRO_OFFSET - 50); // 小さくして急ブレーキ
            mInitState = false;
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        if (! mDoDetectWall) {
            mAngleTrace.execute();
            if (mTimeCounter > 250) {
                //&& mLeftMotorHistory.calcDifference() < 0) {
                //mActivator.reset(USER_GYRO_OFFSET);
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // ライン検知
        if (mDoDetectWall) {
            //VectorT<float> command(30, 0); // 0 で左に曲がる
            //mActivator.run(command);
            mAngleTrace.setTargetAngle(mPrevDirection - 10);//@todo 要調整 2011
            K_THETADOT = 9.5F;
            mAngleTrace.setForward(30);
            mAngleTrace.execute();
            if (mLineDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::AFTERLINETRACE;
                mInitState = true;
            }
        }
    }
    else if (mState == StairwayDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 9.5F;
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
    return mState == StairwayDriver::AFTERLINETRACE; // 終了しました
}

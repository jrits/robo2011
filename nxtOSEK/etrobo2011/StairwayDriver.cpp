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
#if 1 // ログ送信
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
    LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
    LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
    LOGGER_DATAS32[3] = (S32)(mGyroHistory.calcDifference());
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

    // 初期化関数を作るのが面倒くさいのでとりあえずここで
    if (mState == StairwayDriver::INIT) {
        gDoMaimai = true;
        gDoForwardPid = false;
        mLightPid.reset(50, 0, 100);
        mState = StairwayDriver::BEFORELINETRACE;
        mInitState = true;
        mWallDetector.setThreshold(110); 
    }
    // 階段前のライントレース。段差にぶつかるまで。
    if (mState == StairwayDriver::BEFORELINETRACE) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mDoDetectWall = false;
            mInitState = false;
            mLineTrace.setForward(30);
            K_THETADOT = 7.5F; // Find! 階段前曲線をきれいにライントレースできる絶妙な値
        }
        // とりあえず段差検知なしでライントレース（開始直後に車体がぶれて段差検知がtrueを返すことがあるため)
        if (! mDoDetectWall) {
            mLineTrace.execute();
            if (mGps.getXCoordinate() < 4100.0) { // 階段側マーカ始点
                 mDoDetectWall = true;
            }
        }
        // 段差検知しながらライントレース
        if (mDoDetectWall) {
            mLineTrace.execute();
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::ON0THSTAGE_BACK;
                mInitState = true;
                mPrevDirection = mGps.getDirection(); // 角度記憶
                // ぶつかったら座標補正2011
                mGps.adjustXCoordinate(3975.0);  // 階段始点
                mGps.adjustYCoordinate(-1500.0); // 階段始点
            }
        }
    }
    // ０段目(階段前)。一旦バックして助走距離をとる
    if (mState == StairwayDriver::ON0THSTAGE_BACK) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mInitState = false;
            mDoDetectWall = false;
        }
        // 一旦バック(フラグ名が適切ではないが気にしないでください)
        if (! mDoDetectWall) {
            mAngleTrace.setForward(-50);
            mAngleTrace.setTargetAngle(mPrevDirection);
            K_THETADOT = 7.5F;
            mAngleTrace.execute();
            if (mGps.getXCoordinate() > 4050.0) { // 階段側マーカ始点
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
                mState = StairwayDriver::ON0THSTAGE_GO;
                mInitState = true;
            }
        }
    }
    // ０段目(階段前)。助走距離をとったのでいざ突入
    if (mState == StairwayDriver::ON0THSTAGE_GO) {
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
            if (mGps.getXCoordinate() < 4050.0) { // 階段側マーカ始点
                mDoDetectWall = true;
            }
        }
        // いざ突入
        if (mDoDetectWall) {
            mAngleTrace.execute();
            if (mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::ON1STSTAGE;
                mInitState = true;
            }
        }
    }
    // 段差１に載った直後
    else if (mState == StairwayDriver::ON1STSTAGE) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        // 小さくして急ブレーキ
        if (! mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET - 30); // 小さくして急ブレーキ
            mAngleTrace.execute();
            if (mTimeCounter > 100) { // 100カウント間急ブレーキ
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // 大きくして急発進
        if (mDoDetectWall) {
	    mActivator.reset(USER_GYRO_OFFSET + 0); 
            mAngleTrace.execute();
            // 前進しているのを確認しつつ段差検知
            if (mLeftMotor.getCount() - mPrevMotor > 270 && mWallDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::ON2NDSTAGE;
                mInitState = true;
            }
        }
    }
    // 段差２に載った直後
    else if (mState == StairwayDriver::ON2NDSTAGE) {
        if (mInitState) {
            mPrevMotor = mLeftMotor.getCount();
            mTimeCounter = 0;
            mDoDetectWall = false;
            mInitState = false;
        }
        // 小さくして急ブレーキ
        if (! mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET - 30); // 小さくして急ブレーキ
            mAngleTrace.execute();
            if (mTimeCounter > 100) { // 100カウント間急ブレーキ
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // 大きくして急発進 
        if (mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET + 0); // リセット
            mAngleTrace.setForward(5);
            mAngleTrace.execute();
            // ドスン検知
            if (mGps.getXCoordinate() < 3450) { // 今回は座標でやる
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
    // おりました
    else if (mState == StairwayDriver::DROPDOWN) {
        if (mInitState) {
            mInitState = false;
            mTimeCounter = 0;
            mDoDetectWall = false;
        }
        // しばしとどまる
        if (! mDoDetectWall) {
            mActivator.reset(USER_GYRO_OFFSET + 0); // リセット
            mAngleTrace.setForward(0);
            mAngleTrace.execute();
            if (mTimeCounter > 500) {
                mTimeCounter = 0;
                mDoDetectWall = true;
            }
        }
        // ライン検知(ちょっと左に向かってまっすぐ)
        if (mDoDetectWall) {
            mAngleTrace.setTargetAngle(mPrevDirection + 15);
            K_THETADOT = 7.5F;
            mAngleTrace.setForward(15);
            mAngleTrace.execute();
            if (mLineDetector.detect()) {
                { Speaker s; s.playTone(1976, 10, 100); }
                mState = StairwayDriver::AFTERLINETRACE;
                mInitState = true;
            }
        }
    }
    // ライン合流後
    else if (mState == StairwayDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 7.5F;
            mLightPid.reset(80, 0, 160);
            mLineTrace.setForward(0);
            mTimeCounter = 0;
            mInitState = false;
        }
        mLineTrace.execute();
        if (mTimeCounter > 250) {
            mLineTrace.setForward(30);
        }
        if (mTimeCounter > 750) {
            mLineTrace.setForward(75);
        }
    }
    mTimeCounter++;
    return mState == StairwayDriver::AFTERLINETRACE; // 終了しました
}

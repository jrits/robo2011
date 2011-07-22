///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"

/**
 * コンストラクタ
 */
TestDriver::TestDriver()
{
    mState = -1; // 初期化状態
}

bool TestDriver::drive()
{
// SORA    
#if 0 // ログ送信
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(mLineDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLightHistory.calcDifference());
#endif
#if 0 // DEBUG
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // staticは原則禁止だが今だけ
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "TestDriver");
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        //lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        //lcd.putf("dn", (S32)(mRightMotor.getCount()));
        lcd.putf("dn", (S32)(mLineDetector.detect()));
        lcd.putf("dn", (S32)(mLightHistory.calcDifference()));
        lcd.disp();
    }
#endif
    //// ライントレーステスト
    // mLineTrace.setForward(50);
    // mLineTrace.execute();
    // mStraightDetector.detect(); // 直線検知テスト

    //// 向きトレーステスト
    // if (mState == -1) {
    //     // 開始向き
    //     mGps.adjustDirection(180);
    //     // 目標向き
    //     mAngleTrace.setTargetAngle(180);
    //     mAngleTrace.setForward(50);
    //     mState = 0;
    // }
    // mAngleTrace.execute();

    //// 座標トレーステスト
    // if (mState == -1) {
    //     // 開始座標
    //     mGps.adjustXCoordinate(GPS_COURSE_START_X);
    //     mGps.adjustYCoordinate(GPS_COURSE_START_Y);
    //     mGps.adjustDirection(GPS_COURSE_START_DIRECTION);
    //     // 目標座標
    //     mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_GARAGE_X, GPS_GARAGE_Y));
    //     mCoordinateTrace.setForward(50);
    //     mState = 0;
    // }
    // mCoordinateTrace.execute();

    //// 仮想ライントレーステスト
    if (mState == -1) { // 初期化状態
         mVirtualLineTrace.setControlPoints(TEST_LINE, NUM_TEST_LINE);
         mVirtualLineTrace.reset();
         mState = 0;
     }
     mVirtualLineTrace.execute();

    //// 座標指定着地テスト
/*
    if (mState == -1) { // 初期化状態
        // 開始座標
        mGps.adjustXCoordinate(GPS_COURSE_START_X);
        mGps.adjustYCoordinate(GPS_COURSE_START_Y);
        mGps.adjustDirection(GPS_COURSE_START_DIRECTION);
        // 目標座標
        mCoordinateStop.setTargetCoordinate(MakePoint(GPS_GARAGE_X, GPS_GARAGE_Y));
//        mCoordinateStop.setTargetCoordinate(MakePoint(GPS_COURSE_START_X, GPS_COURSE_START_Y));
        mCoordinateStop.setForward(100);
//        mCoordinateStop.setSlowdownDistance(500);
        //SORA test
        //急停止を行う距離。目標座標に250[mm]以内に立ち入ると急停止を行う。
        mCoordinateStop.setSlowdownDistance(210);
        //SORA test
        mState = 0;
    }
    mCoordinateStop.execute();
    */
    //// SlowdownSkill(mAngleTrace)テスト。mSlowdownPid調整用
    // if (mState == -1) { // 初期化状態
    //     // 開始座標
    //     mGps.adjustXCoordinate(0.0);
    //     mGps.adjustYCoordinate(0.0);
    //     mGps.adjustDirection(0.0);
    //     // 目標向き
    //     mAngleTrace.setTargetAngle(0.0);
    //     mAngleTrace.setForward(50.0);
    //     // 停止目標距離
    //     mSlowdownSkill.setSkill(&mAngleTrace); // 向きトレースをデコレート
    //     mSlowdownSkill.setTargetDistance(600.0); // 60cm
    //     //mSlowdownSkill.setMinimumForward(0); // 停止
    //     //mSlowdownSkill.setMinimumForward(-1); // バック許可
    //     mSlowdownSkill.setMinimumForward(-30); // 行ったりきたり許可
    //     mState = 0;
    // }
    // mSlowdownSkill.execute(); // SlowdownSkillを実行

    //// StopSkill(mAngleTrace)テスト。
    // if (mState == -1) { // 初期化状態
    //     // 開始座標
    //     K_THETADOT = 7.5F;
    //     mGps.adjustXCoordinate(0.0);
    //     mGps.adjustYCoordinate(0.0);
    //     mGps.adjustDirection(0.0);
    //     // 目標向き
    //     mAngleTrace.setTargetAngle(0.0);
    //     mAngleTrace.setForward(100.0);
    //     // 停止目標距離
    //     mStopSkill.setSkill(&mAngleTrace); // 向きトレースをデコレート
    //     mStopSkill.setTargetDistance(600.0); // 60cm
    //  	mStopSkill.setAllowableError(100.0); // 50cm地点でフォワード値0
    //     mState = 0;
    // }
    // mStopSkill.execute(); // StopSkillを実行

    return true;
}

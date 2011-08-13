///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
extern bool gDoMaimai;
extern bool gDoForwardPid;
extern "C" extern void tail_control(signed int angle);

/* sample_c3マクロ */
#define TAIL_ANGLE_STAND_UP 108 /* 完全停止時の角度[度] */
#define TAIL_ANGLE_TRIPOD_DRIVE 95 /* ３点走行時の角度[度] */
#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */

/**
 * コンストラクタ
 */
TestDriver::TestDriver()
{
    mState = -1; // 初期化状態
}

bool TestDriver::drive()
{
#if 0 // ログ送信
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(mLineDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLightHistory.calcDifference());
#endif
#if 1 // DEBUG
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
    // デフォルト
    tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
    gDoMaimai = false; /* まいまい式は使わない */
    gDoForwardPid = false; 
    VectorT<float> command(50, 0);

    // テスト 通常走行
    if (0) {
        mActivator.run(command);
    }
    // テスト フォーワードPID
    if (0) {
        gDoForwardPid = true;
        mActivator.run(command);
    }
    // テスト ３点走行
    if (0) {
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodActivator.run(command);
    }
    // テスト ３点走行 with フォワードPID
    if (0) {
        gDoForwardPid = true;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodActivator.run(command);
    }
    // テスト ライントレース.
    if (0) {
        mLineTrace.setForward(50);
        mLineTrace.execute();
    }
    // テスト ライントレース with フォワードPID
    if (0) {
        gDoForwardPid = true;
        mLineTrace.setForward(50);
        mLineTrace.execute();
    }
    // テスト まいまい式ライントレース
    if (0) {
        gDoMaimai = true;
        mLineTrace.setForward(50);
        mLineTrace.execute();
    }
    // テスト ３点走行ライントレース
    if (0) {
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodLineTrace.setForward(50);
        mTripodLineTrace.execute();
    }
    // テスト ３点走行ライントレース with フォワードPID
    if (1) {
        gDoForwardPid = true;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodLineTrace.setForward(50);
        mTripodLineTrace.execute();
    }
    // テスト まいまい式３点走行ライントレース
    if (0) {
        gDoMaimai = true;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodLineTrace.setForward(50);
        mTripodLineTrace.execute();
    }

    // mSitDownSkill.execute();
    return true;
}

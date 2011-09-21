///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
extern "C" extern void tail_control(signed int angle);

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
        lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        lcd.putf("dn", (S32)(mRightMotor.getCount()));
        //lcd.putf("dn", (S32)(mLineDetector.detect()));
        //lcd.putf("dn", (S32)(mLightHistory.calcDifference()));
        lcd.disp();
    }
#endif
    // デフォルト
    tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
    VectorT<float> command(50, 0);

    // テスト 通常走行
    if (0) {
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
    /* 参考：GPSは以下の値で初期化されている
    mXCoordinate(GPS_COURSE_START_X),
    mYCoordinate(GPS_COURSE_START_Y),
    mDirection(GPS_COURSE_START_DIRECTION),
    */
    // テスト ３点走行アングルトレース
    if (1) {
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodAngleTrace.setTargetAngle(540);
    	mTripodAngleTrace.setForward(0);
        mTripodAngleTrace.execute();
    }
	// テスト ３点走行位置トレース
    if (0) {
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_COURSE_START_X - 500.0, GPS_COURSE_START_Y -500.0));//
        mCoordinateTrace.setForward(50);
        mCoordinateTrace.execute();
    }

    return true;
}

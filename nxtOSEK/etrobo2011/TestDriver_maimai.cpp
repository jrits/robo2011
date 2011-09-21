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
#if 1 // DEBUG
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // staticは原則禁止だが今だけ
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "TestDriver");
        lcd.putf("sdn", "Light  = ", (S32)(mLightSensor.get()));
        lcd.putf("sdn", "Maimai = ", (S32)(gMaimaiValue*100));
        lcd.disp();
    }
#endif

    // テスト ライントレース.
    if (0) {
        gDoMaimai = false;
        tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
        mLineTrace.setForward(50);
        mLineTrace.execute();
    }
    // テスト まいまい式ライントレース
    if (0) {
        gDoMaimai = true;
        tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
        mLineTrace.setForward(50);
        mLineTrace.execute();
    }
    // テスト ３点走行ライントレース
    if (0) {
        gDoMaimai = false;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodLineTrace.setForward(50);
        mTripodLineTrace.execute();
    }
    // テスト まいまい式３点走行ライントレース
    if (1) {
        gDoMaimai = true;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        mTripodLineTrace.setForward(50);
        mTripodLineTrace.execute();
    }
    // テスト まいまい式三点走行 helloworld
    if (0) {
        gDoMaimai = true;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
    }

    return true;
}

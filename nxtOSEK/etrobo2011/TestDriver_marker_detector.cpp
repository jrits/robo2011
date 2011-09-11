///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
extern bool gDoForwardPid;
extern bool gDoMaimai;
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
        lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        lcd.putf("dn", (S32)(mRightMotor.getCount()));
        lcd.putf("dn", (S32)(mLightSensor.get()));
        lcd.disp();
    }
#endif
    // デフォルト
    tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
    gDoForwardPid = false;
    VectorT<float> command(50, 0);

    // テスト マーカー検知(1)
    // 条件: ON/OFFライントレース with フォワードPID without まいまい
    if (1) {
        // スタートが難しかったのでしばし(2s)PIDライントレース
        if (count < 500) {
            gDoMaimai = false;
            mLineTrace.setForward(50);
            mLineTrace.execute();
        // ここからON/OFFライントレース
        } else {
            K_PHIDOT = 25.0F*2.5F;
            K_THETADOT = 7.5F;
            gDoForwardPid = false;
            gDoMaimai = false;
            mLineTrace.setForward(50);
            mLineTrace.setDoOnOffTrace(true);
            mLineTrace.execute();
            if (mMarkerDetector.detect()) { // マーカー検知
                Speaker speaker;
                speaker.playTone(1976, 10, 100); // Hz:33-1976 , 10ms, volume:0-100
            }
        }
    }
    // テスト マーカー検知(2) まだ作っていない。
    // 条件: PIDライントレース with フォワードPID with まいまい
    if (0) {
        gDoForwardPid = true;
        gDoMaimai = true;
        mLineTrace.setForward(50);
        mLineTrace.setDoOnOffTrace(true);
        mLineTrace.execute();
    }

    return true;
}

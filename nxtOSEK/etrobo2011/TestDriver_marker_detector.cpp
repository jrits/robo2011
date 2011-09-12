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
        lcd.putf("dn", (S32)(mLightSensor.get()));
        lcd.disp();
    }
#endif
    // デフォルト
    tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
    gDoForwardPid = false;
    LineTrace *lineTrace;

    // テスト 非マイマイ式ON/OFFライントレースマーカー検知
    if (0) {
        gDoMaimai = false;
        lineTrace = &mLineTrace;
    }
    // テスト マイマイ式ON/OFFライントレースマーカー検知
    if (0) {
        gDoMaimai = true;
        lineTrace = &mLineTrace;
    }
    // テスト 非マイマイ式ON/OFF３点傾立ライントレースマーカー検知
    if (0) {
        gDoMaimai = false;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE);
        lineTrace = &mTripodLineTrace;
    }
    // テスト マイマイ式ON/OFF３点傾立ライントレースマーカー検知
    if (1) {
        gDoMaimai = true;
        tail_control(TAIL_ANGLE_TRIPOD_DRIVE);
        lineTrace = &mTripodLineTrace;
    }

    // 以下、共通ロジック
    {
        static int timeCounter = 0;
        timeCounter++;
        // スタートが難しかったのでしばし(2s)PIDライントレース
        if (timeCounter < 500) {
            lineTrace->setForward(50);
            lineTrace->execute();
        }
        // ここからON/OFFライントレース
        else {
            lineTrace->setDoOnOffTrace(true);
            lineTrace->setForward(50);
            lineTrace->execute();
            // マーカー検知
            if (mMarkerDetector.detect()) {
                Speaker speaker;
                speaker.playTone(1976, 10, 100); // Hz:33-1976 , 10ms, volume:0-100
            }
        }
    }

    return true;
}

///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"

/* sample_c3マクロ */
#define TAIL_ANGLE_STAND_UP 108 /* 完全停止時の角度[度] */
#define TAIL_ANGLE_TRIPOD_DRIVE 95 /* ３点走行時の角度[度] */
#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */
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
    DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // staticは原則禁止だが今だけ
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "TestDriver");
        //lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        //lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        //lcd.putf("dn", (S32)(mGps.getDirection()));
        //lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        lcd.putf("dn", (S32)(mRightMotor.getCount()));
        lcd.putf("dn", (S32)(mTailMotor.getCount()));
        //lcd.putf("dn", (S32)(mLineDetector.detect()));
        //lcd.putf("dn", (S32)(mLightHistory.calcDifference()));
        lcd.disp();
    }
#endif
    VectorT<float> command(50, 0);

    // テスト 走りながら起き上がり→ダメ(暴走する)
    if (0) {
        static int count = 0;

        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        if (count < 300) {
            // ３点走行
            mTripodActivator.run(command);
        } else if (count < 325) {
            // 起き上がり
            mActivator.reset(USER_GYRO_OFFSET + 15); // 大きくして前のめり
            mActivator.run(command);
        } else if (count < 10000) {
            // ２点走行
            mActivator.reset(USER_GYRO_OFFSET);
            mActivator.run(command);
        }
        count++;
    }
    // テスト 止まってから起き上がり→まだダメ
    if (1) {
        static int count = 0;

        tail_control(TAIL_ANGLE_TRIPOD_DRIVE); /* ３点走行用角度に制御 */
        if (count < 300) {
            // ３点走行
            mTripodActivator.run(command);
        } else if (count < 400) {
            // 停止
            mTripodActivator.stop();
        } else if (count < 500) {
            // 起き上がり
            mActivator.reset(USER_GYRO_OFFSET + 15); // 大きくして前のめり
            mActivator.run(command);
        } else if (count < 10000) {
            // ２点走行
            mActivator.reset(USER_GYRO_OFFSET);
            mActivator.run(command);
        }
        count++;
    }

    // mSitDownSkill.execute();
    return true;
}

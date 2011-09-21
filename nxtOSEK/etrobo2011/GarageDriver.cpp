//
// GarageDriver.cpp
//

#include "GarageDriver.h"
#include "factory.h"
#include "constants.h"

/**
 * コンストラクタ
 */
GarageDriver::GarageDriver()
{
    mState = GarageDriver::INIT;
}

/**
 * ガレージ・イン区間を攻略する
 *
 * ガレージ・イン前直線(坂道直線との交点)から、ガレージ・インまでを担当する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool GarageDriver::drive()
{
#if 0 // ログ送信(0：解除、1：実施)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
    LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
    LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
//  LOGGER_DATAS32[3] = (S32)(mGps.getDistance());

#endif
#if 0 // デバッグ(0：解除、1：実施)
    //DESK_DEBUG = true; // モータを回さないデバッグ
    static int count = 0; // staticは基本禁止だが、デバッグなので
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "GarageDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    // 初期化関数を作るのが面倒くさいのでここで
    if (mState == GarageDriver::INIT) {
        gDoMaimai = true; // まいまい式
        gDoForwardPid = false;
        mTimeCounter = 0;
        K_THETADOT = 7.5F;
        mLineTrace.setForward(50);
        mState = GarageDriver::READYGO;
    }
    // いきなり問答無用でONOFFするよりも、ちゃんとテストで確認できたスピードにしてからやる。
    if (mState == GarageDriver::READYGO) {
        mLineTrace.setForward(50);
        mLineTrace.execute();
        // 1s たった
        if (mTimeCounter > 250) {
            mState = GarageDriver::MARKER;
        }
    }
    // ONOFFライントレースをしながらマーカを見つける。
    if (mState == GarageDriver::MARKER) {
        mLineTrace.setDoOnOffTrace(true);
        mLineTrace.setForward(50);
        mLineTrace.execute();
        // マーカー検知
        if (mMarkerDetector.detect()) {
            mState = GarageDriver::STOP;
            mPrevDistance = mGps.getDistance();
        }
    }
    // マーカを見つけてから数cm進んで停止
    if (mState == GarageDriver::STOP) {
        //mLineTrace.setDoOnOffTrace(true);
        //mLineTrace.setForward(50);
        //mStopSkill.setSkill(&mLineTrace);
        mAngleTrace.setForward(50);
        mAngleTrace.setTargetAngle(450);
        mStopSkill.setSkill(&mAngleTrace);
        mStopSkill.setTargetDistance(mPrevDistance + 130); // mm
        mStopSkill.execute();
        if (mStopSkill.isArrived()) {
            mState = GarageDriver::SITDOWN;
        }
    }
    // 座る
    if (mState == GarageDriver::SITDOWN) {
        mSitDownSkill.execute();
    }

    mTimeCounter++;
    return false; // 終了しない(最後なのでなんでも良い)
}


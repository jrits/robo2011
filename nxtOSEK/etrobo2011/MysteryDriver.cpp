//
// MysteryDriver.cpp
//

#include "MysteryDriver.h"
#include "factory.h"
#include "constants.h"

//ミステリーサークルのルート(4パターン)
#include "MysteryLineLL.h"
#include "MysteryLineLR.h"
#include "MysteryLineRL.h"
#include "MysteryLineRR.h"

/**
 * コンストラクタ
 */
MysteryDriver::MysteryDriver()
{
    mState = MysteryDriver::INIT;
    mInitState = true;
}

/**
 * ミステリーサークル区間を攻略する
 *
 * サークル前直線ライントレースから、ライン復帰後ライントレースまでを担当する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool MysteryDriver::drive()
{
#if 0 // ログ送信部分（0：解除、1：実施）
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
	LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
	LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
	LOGGER_DATAS32[3] = (S32)(mGps.getDistance());
#endif
    
#if 1 // デバッグ部分
    // DESK_DEBUG = true; // モータを回さない時
    static int count = 0; // staticは基本禁止だが、デバッグだから使用
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "MysteryDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    
    if (mState == MysteryDriver::INIT) { // 初期化状態
        mRoute       = mEnigmaDriver.decode();  // エニグマドライバから、ルート情報が送られてくる。
        mInitState   = true;
        
        mState = MysteryDriver::BEFORELINETRACE;
    }
    // 手前直線ライントレース(インコース、アウトコースシーソー後マーカー終わり地点)
    if (mState == MysteryDriver::BEFORELINETRACE) {
        if (mInitState) {
            mInitState = false;
        }
        mLineTrace.execute();
        if (mGps.getXCoordinate() > 3200) {
            mState = MysteryDriver::MYSTERYROUTE;
            mInitState = true;
        }
    }
    if (mState == MysteryDriver::MYSTERYROUTE) {    // 縦横無尽走行
        if (mInitState) {
            mInitState = false;

        switch (mRoute) {//ルート分け
            case 0: //右-右コース
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_RR, NUM_MYSTERY_LINE_RR);
                break;
            case 1: //右-左コース
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_RL, NUM_MYSTERY_LINE_RL);
                break;
            case 2: //左-右コース
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_LR, NUM_MYSTERY_LINE_LR);
                break;
            case 3: //左-左コース
                mVirtualLineTrace.setControlPoints(MYSTERY_LINE_LL, NUM_MYSTERY_LINE_LL);
                break;
            }
            mVirtualLineTrace.reset();
        }
        mVirtualLineTrace.execute();
        // （注意）ラインに向かって斜めに突進するところまで含めておくこと。
        if (mVirtualLineTrace.isLast()) {   // ライン復帰できるかどうかの判断。
            if (mLineDetector.detect() != LineDetector::ON_WHITE) {
                mState = MysteryDriver::AFTERLINETRACE;
                mInitState = true;
            }
        }
    }
    else if (mState == MysteryDriver::AFTERLINETRACE) { //縦横無尽走行終了後
        if (mInitState) {
            mInitState = false;
            mLineTrace.reset(); // INIT_FORWARD復活
        }
        mLineTrace.execute();   // ライントレース走行実行
    }
    
    return mState == MysteryDriver::AFTERLINETRACE; // 終了しました
}

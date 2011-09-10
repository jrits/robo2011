//
// InGarageDriver.cpp
//

#include "InGarageDriver.h"
#include "factory.h"
#include "constants.h"

/**
 * コンストラクタ
 */
InGarageDriver::InGarageDriver()
{
    mState = InGarageDriver::INIT;
}

/**
 * インコースガレージ・イン区間を攻略する
 *
 * 坂道直線ライントレースから、ガレージ・インまでを担当する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool InGarageDriver::drive()
{
#if 0 // ログ送信(0：解除、1：実施)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
    LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
    LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
    LOGGER_DATAS32[3] = (S32)(mGps.getDistance());
#endif
#if 0 // デバッグ(0：解除、1：実施)
    //DESK_DEBUG = true; // モータを回さないデバッグ
    static int count = 0; // staticは基本禁止だが、デバッグなので使用
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "InGarageDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    if (mState == InGarageDriver::INIT) { // 初期化状態
        mTimeCounter = 0;
        mState = InGarageDriver::BEFORELINETRACE;
     }

    // 【インコース・ガレージインのために】
    // インコース坂手前の出っ張りから走行し、直線を検知した段階で距離を測る。
    // インコース・ガレージの正面まで移動させて、角度トレースで左90度回転・FORWARDは0。
    // その後、角度トレースでガレージにIN
    
    
    // まず手間直線ライントレース(インコース坂手前カーブの一番出っ張っている所から)
    if (mState == InGarageDriver::BEFORELINETRACE) {
        mLineTrace.execute();   
        if (mStraightDetector.detect() == true) {
            mState = InGarageDriver::STRAIGHTLINETRACE;
            mSlowdownSkill.setSkill(&mLineTrace);
            // 直線検知してから走る距離。ここを手当たり次第？
            mSlowdownSkill.setTargetDistance(3450 - 135);
            mSlowdownSkill.setMinimumForward(0);
        }
    }
    else if (mState == InGarageDriver::STRAIGHTLINETRACE) {
        mSlowdownSkill.execute();
        if (mSlowdownSkill.isArrived()) {
            mState = InGarageDriver::PREPAREENTERGARAGE;
            mAngleTrace.setTargetAngle(270 -2);
            mAngleTrace.setForward(0);
        }
    }
    else if (mState == InGarageDriver::PREPAREENTERGARAGE) {
        mAngleTrace.execute();
        if (mAngleTrace.isArrived()) {
            mState = InGarageDriver::ENTERGARAGE;
            mAngleTrace.setTargetAngle(270 - 2);
            // FORWARD値。ここを主に調整する
            mAngleTrace.setForward(20);
            mSlowdownSkill.setSkill(&mAngleTrace);
            mSlowdownSkill.setTargetDistance(100 - 50);
            mSlowdownSkill.setMinimumForward(0);
        }
    }
    else if (mState == InGarageDriver::ENTERGARAGE) {
        mSlowdownSkill.execute();
    }
    mTimeCounter++;
    return false; // 終了しない(最後なのでなんでも良い)
}

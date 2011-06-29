///////////////////////////////////////////////////////////
//  FailDetector.cpp
//  Implementation of the Class FailDetector
//  Created on:      10-9-2010 19:42:52
//  Original author: ksuzuki
///////////////////////////////////////////////////////////

#include "FailDetector.h"
#include "factory.h"

/**
 * コンストラクタ
 */
FailDetector::FailDetector()
{
    mTimeCounter = 0;
    mPrevLeftCount = 0;
    mPrevRightCount = 0;
}

/**
 * 転倒を検知する
 *
 * @retval true 検知した
 * @retval false 検知しなかった
 */
bool FailDetector::detect()
{
#if 0 // Lcd
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // staticは原則禁止だが今だけ
    if (count++ % FAILSAFE_SAMPLECOUNT == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "FailDetector");
        lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        lcd.putf("dn", (S32)(mRightMotor.getCount()));
        lcd.putf("dn", (S32)(mPrevLeftCount));
        lcd.putf("dn", (S32)(mPrevRightCount));
        lcd.putf("dn", (S32)(mLeftMotor.getCount() - mPrevLeftCount));
        lcd.putf("dn", (S32)(mRightMotor.getCount() - mPrevRightCount));
        lcd.disp();
    }
#endif

    bool ret = false;
    if (mTimeCounter++ % FAILSAFE_SAMPLECOUNT == 0) {
        int speedL = mLeftMotor.getCount() - mPrevLeftCount;
        int speedR = mRightMotor.getCount() - mPrevRightCount;
        mPrevLeftCount = mLeftMotor.getCount();
        mPrevRightCount = mRightMotor.getCount();
        //左右モータエンコーダ値の微分値が、ともに閾値を超えていた場合、空転とみなしモータを停止させる
        ret = (speedL < -FAILSAFE_THRESHOLD || speedL > +FAILSAFE_THRESHOLD)
            && (speedR < -FAILSAFE_THRESHOLD || speedR > +FAILSAFE_THRESHOLD);
    }
    return ret;
}

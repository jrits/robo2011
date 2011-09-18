///////////////////////////////////////////////////////////
//  StandupDriver.h
//  Definition of the Class StandupDriver
//  Created on:      10-9-2010 19:43:43
///////////////////////////////////////////////////////////
#ifndef StandupDriver_H_
#define StandupDriver_H_

#include "Driver.h"

/**
 * スタンドアップドライバ
 */
class StandupDriver : public Skill
{ 
public:
    enum eSubSection {
        INIT = -1,
        TAILPUSH,
        STOPTAILPUSH,
        FINISH
    };
    StandupDriver();
    ~StandupDriver(){}
    bool drive();
    bool isArrived();
private:
    int mState;//状態遷移変数
    int mCounter;//停止用カウンター
    int mTargetTailAngle;//尻尾の目標角度
    int mPrevTailAngle;//開始時の尻尾角度一時保存用
    float mPrevDirection;//開始時の方向一時保存用
    float mTailPWM;//尻尾制御用PWM
};

#endif

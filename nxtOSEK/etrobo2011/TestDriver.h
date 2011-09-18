///////////////////////////////////////////////////////////
//  TestDriver.h
//  Definition of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
///////////////////////////////////////////////////////////
#ifndef TESTDRIVER_H_
#define TESTDRIVER_H_
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */

#include "Driver.h"

/**
 * テストドライバ
 */
class TestDriver : public Driver
{ 
public:
    enum eSubSection {
        INIT = -1,
        BEFOREANGLETRACE,
        AFTERFIRSTSTEP,
        FIRSTSLOWRUN,
        BACKRUN,
        SECONDSLOWRUN,
        LINERETURN,
        AFTERLINETRACE
    };
    TestDriver();
    ~TestDriver(){}
    bool drive();
//    void mTail_control(signed int angle);
    int mState;
    int SSC;//State Switch Counter
    int count;
    float beforeRMH; //状態変化時のモータ回転角度（積算値）
    signed char mPwm_L;
    signed char mPwm_R;
};

#endif

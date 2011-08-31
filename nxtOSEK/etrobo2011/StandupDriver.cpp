///////////////////////////////////////////////////////////
//  StandupDriver.cpp
//  Implementation of the Class StandupDriver
//  Created on:      24-8-2011 20:51:45
//  Original author: hayama
///////////////////////////////////////////////////////////
#include "StandupDriver.h"
#include "factory.h"
#include "Speaker.h"

/**
 * コンストラクタ
 */
StandupDriver::StandupDriver()
{
    mState = INIT; // 初期化状態
	mTargetTailAngle = 0;//初期角度
	mCounter = 0;
}

bool StandupDriver::drive()
{
	mWallDetector.setThreshold(100);
    if(mState == INIT){
        nxt_motor_set_count(NXT_PORT_A, mTailMotor.getCount()); //現在の尻尾の角度を一度だけ取得、保持
        mState = TAILPUSH;
    }
    if(mState == TAILPUSH){
        if(mTailMotor.getCount() <= mTargetTailAngle){ //尻尾を目標角度まで動かす
            nxt_motor_set_speed(NXT_PORT_A, 60, 1);
        }else{
            nxt_motor_set_speed(NXT_PORT_A, 0, 1);//一度尻尾を止める
            mState = STOPTAILPUSH;
        }
   	}else if(mState == STOPTAILPUSH){
        if(mTailMotor.getCount() >= 2){//尻尾を初期状態に戻す
            nxt_motor_set_speed(NXT_PORT_A, -100, 1);
        }
		if(mCounter <= 500){//2秒間安定させる
		    //アングルトレースを使ってその場に立つ
			mAngleTrace.setForward(0);
			mAngleTrace.setTargetAngle(180);
			mAngleTrace.execute();
			mCounter++;
		}else{
			mState = FINISH;
		}
   	}
    return   mState == FINISH;
}

/**
 * 尻尾の目標角度を指定する
 *
 * @return なし
 */
void StandupDriver::setTargetTailAngle(float targetTailAngle)
{
	mTargetTailAngle = targetTailAngle;
}


///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
#include "Speaker.h"

/**
 * コンストラクタ
 */
TestDriver::TestDriver()
{
//    mState = BEFOREANGLETRACE; // 初期化状態
    mState = INIT; // 初期化状態
}

bool TestDriver::drive()
{
	mWallDetector.setThreshold(100);
    if(mState == INIT){
        nxt_motor_set_count(NXT_PORT_A, 0); //現在の尻尾の角度を一度だけ取得、保持
        mState = TAILPUSH;
    }
    if(mState == TAILPUSH){
        if(mTailMotor.getCount() <= 114){ //尻尾を目標角度まで動かす
            nxt_motor_set_speed(NXT_PORT_A, 60, 1);
        }else{
            nxt_motor_set_speed(NXT_PORT_A, 0, 1);//一度尻尾を止める
            mState = STOPTAILPUSH;
        }
   	}else if(mState == STOPTAILPUSH){
        if(mTailMotor.getCount() >= 2){//尻尾を初期状態に戻す
            nxt_motor_set_speed(NXT_PORT_A, -100, 1);
        }
  		K_THETADOT = 7.5;
   	    //アングルトレースを使ってその場に立つ
  		mAngleTrace.setForward(0);
  		mAngleTrace.setTargetAngle(180);
  		mAngleTrace.execute();
  		}
  	}

    return true;
}

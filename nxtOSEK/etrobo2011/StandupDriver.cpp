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
<<<<<<< HEAD
    mState = INIT; // 初期化状態
	mTargetTailAngle = 120; // 尻尾の目標角度
	mCounter = 0;
    mPrevTailAngle = 0;
    mPrevDirection = 0;
    mTailPWM = 50;
=======
  mState = INIT; // 初期化状態
  mTargetTailAngle = 0;//初期角度
  mCounter = 0;
>>>>>>> feature/lookupgate_seo_again
}

bool StandupDriver::drive()
{
<<<<<<< HEAD
#if 1 // DEBUG
    //DESK_DEBUG = true; // モータを回さないデバグ
    if (mCounter % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "StandupDriver");
        lcd.putf("dn", (S32)(mTailPWM));
        lcd.putf("dn", (S32)(mTailMotor.getCount()));
        lcd.disp();
    }
#endif

    if(mState == INIT){
        mState = TAILPUSH;
        mPrevTailAngle = mTailMotor.getCount();
        mPrevDirection = mGps.getDirection();
        mLeftMotor.setPWM(0);
        mRightMotor.setPWM(0);
    }
    // 尻尾でたつ
    if(mState == TAILPUSH){
        //尻尾を目標角度まで動かす
        if(mTailMotor.getCount() <= mTargetTailAngle){
            // 尻尾が全く動いていなければPWMを強める
            if (mTailMotor.getCount() <= mPrevTailAngle + 10) {
                mTailPWM += 0.1;
            }
            mTailMotor.setPWM((int)mTailPWM);
        }
        //目標角度まで動いたら止める
        else {
            mTailMotor.setPWM(0);
            mCounter = 0;
            mState = STOPTAILPUSH;
        }
   	}
    // その場で安定させる
    else if(mState == STOPTAILPUSH){
        //アングルトレースを使って速度0で2秒間安定させる
		if(mCounter <= 500){
			mAngleTrace.setForward(0);
			mAngleTrace.setTargetAngle(mPrevDirection);
			mAngleTrace.execute();
			mCounter++;
		}else{
			mState = FINISH;
		}
   	}
    return isArrived();
=======
  //	mWallDetector.setThreshold(100);
  if(mState == INIT){
    //        nxt_motor_set_count(NXT_PORT_A, mTailMotor.getCount()); //現在の尻尾の角度を一度だけ取得、保持
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
>>>>>>> feature/lookupgate_seo_again
}

bool StandupDriver::isArrived()
{
<<<<<<< HEAD
    return mState == FINISH;
=======
  mTargetTailAngle = targetTailAngle;
>>>>>>> feature/lookupgate_seo_again
}


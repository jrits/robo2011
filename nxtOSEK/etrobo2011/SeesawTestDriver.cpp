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
    mState = BEFOREANGLETRACE; // 初期化状態
	SSC = 0;//State Switch Counter
	beforeRMH = 0.0; //状態変化時のモータ回転角度（積算値）
	mPwm_L = mPwm_R = 0; /* 左右モータPWM出力 */

}


bool TestDriver::drive()
{
// SORA    
#if 0 // ログ送信
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(mLineDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLightHistory.calcDifference());
#endif

	
	mWallDetector.setThreshold(100);
	
	if(mState == BEFOREANGLETRACE){
  		K_THETADOT = 9.5;
  		mAngleTrace.setTargetAngle(180);
  		mAngleTrace.setForward(100);
  		mAngleTrace.execute();
  		if(mWallDetector.detect()){
  			{ Speaker s; s.playTone(261, 10, 80); }
  			beforeRMH = mRightMotorHistory.get(0);
			mState = AFTERFIRSTSTEP;
  		}
  	}else if(mState == AFTERFIRSTSTEP){
  		K_THETADOT = 7.5;
  		mAngleTrace.setTargetAngle(180);
  		mAngleTrace.setForward(70);
  		mAngleTrace.execute();
  		if(mRightMotorHistory.get(0) - beforeRMH >= 180){
  			{ Speaker s; s.playTone(293, 10, 80); }
  			beforeRMH = mRightMotorHistory.get(0);
	  		mState = FIRSTSLOWRUN;
  		}
  	}else if(mState == FIRSTSLOWRUN){
  		K_THETADOT = 7.5;
  		mAngleTrace.setForward(25);
  		mAngleTrace.setTargetAngle(175);
  		mAngleTrace.execute();
  		if(mWallDetector.detect()){
  			{ Speaker s; s.playTone(329, 10, 80); }
  			beforeRMH = mRightMotorHistory.get(0);
  			USER_GYRO_OFFSET = USER_GYRO_OFFSET + 20;
  			mState = BACKRUN;
  		}
  	}else if(mState == BACKRUN){
  		mPwm_L = mPwm_R = -40;
		nxt_motor_set_speed(NXT_PORT_C, mPwm_L, 1); 
		nxt_motor_set_speed(NXT_PORT_B, mPwm_R, 1); 
  		if(mRightMotorHistory.get(0) - beforeRMH <= -180){
  			{ Speaker s; s.playTone(329, 10, 80); }
  			beforeRMH = mRightMotorHistory.get(0);
	  		mState = SECONDSLOWRUN;
  		}
	}else if(mState == SECONDSLOWRUN){
  		mPwm_L = mPwm_R = 0;
		nxt_motor_set_speed(NXT_PORT_C, mPwm_L, 1); 
		nxt_motor_set_speed(NXT_PORT_B, mPwm_R, 1); 
  		if(mWallDetector.detect()){
  			{ Speaker s; s.playTone(349, 10, 80); }
  			beforeRMH = mRightMotorHistory.get(0);
	  		mState = SECONDSLOWRUN;
  		}
  	}else if(mState == LINERETURN){
  	}else if(mState == AFTERLINETRACE){
  	}
    //// ライントレーステスト
    // mLineTrace.setForward(50);
    // mLineTrace.execute();
    // mStraightDetector.detect(); // 直線検知テスト

    //// 向きトレーステスト
    // if (mState == -1) {
    //     // 開始向き
    //     mGps.adjustDirection(180);
    //     // 目標向き
    //     mAngleTrace.setTargetAngle(180);
    //     mAngleTrace.setForward(50);
    //     mState = 0;
    // }
    // mAngleTrace.execute();
/*
	  	if(balanceFlag){
  		K_THETADOT = 8.5;
		mAngleTrace.setTargetAngle(180.0);
		mAngleTrace.setForward(70.0);
		mAngleTrace.execute();
		mWallDetector.setThreshold(80);
  	}
  	if(mWallDetector.detect() && oneFlag == false){
		s.playTone(1976, 10, 100);
  		hogeBefore = mRightMotorHistory.get(0);
  		oneFlag = true;
  		balanceFlag = false;
  		pwm_L = pwm_R = 50;
  	}
  	if(oneFlag == true){//&&  (() ){
//		s.playTone(1976, 10, 100);
		nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); 
		nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); 
		tail_control(60);
  		if((mRightMotorHistory.get(0) - hogeBefore) > -10 && (mRightMotorHistory.get(0) - hogeBefore) <= 720){
    		pwm_L = pwm_R = 60;
  		}
  		if((mRightMotorHistory.get(0) - hogeBefore) > 720){
    		pwm_L = pwm_R = 0;
  			timecounter++;
  		}
  		if(timecounter > 250){
  			backFlag = true;
  		}
  		if(backFlag){
	  		pwm_L = pwm_R = -50;
//  			balanceFlag = true;
//  			oneFlag = false;
  		}
  	}
*/
 
    return true;
}
/*
void TestDriver::mTail_control(signed int angle)
{
  float pwm = (float)(angle - nxt_motor_get_count(NXT_PORT_A))*P_GAIN; 
  if (pwm > PWM_ABS_MAX)
  {
    pwm = PWM_ABS_MAX;
  }
  else if (pwm < -PWM_ABS_MAX)
  {
    pwm = -PWM_ABS_MAX;
  }

  nxt_motor_set_speed(NXT_PORT_A, (signed char)pwm, 1);
}
*/

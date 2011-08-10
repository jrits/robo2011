///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"

/**
 * コンストラクタ
 */
TestDriver::TestDriver()
{
    mTimeCounter = 0;
	mInitState = false;             
	mState = StairwayDriver::INIT;  //ステート
}

bool TestDriver::drive()
{
    	float xCo = mGps.getXCoordinate();
//	float yCo = mGps.getYCoordinate();

//	段差攻略
	if (mState == SeesawDriver::INIT) {
        mState = SeesawDriver::BEFORELINETRACE;
        mInitState = true;
		mWallDetector.setThreshold(80);
    }
	if(mState == SeesawDriver::BEFORELINETRACE){
    	if(mInitState){
			K_THETADOT = 10.5F;
			mAngleTrace.setTargetAngle(354.0);
			mAngleTrace.setForward(100);
			mLineTrace.setForward(100);
    		mInitState = false;
    	}
		if(1200 < xCo && xCo < 1500)
		{
			mAngleTrace.execute();
		}else if(xCo <= 1200){
			mLineTrace.execute();
		}
		else if(xCo >= 1500){
	    	if(mWallDetector.detect()){
				mStartAngleCounter = mLeftMotorHistory.get();
    			mState = SeesawDriver::ONTHESEESAW;
    			mInitState = true;
	    	}
			mLineTrace.execute();
		}
	}else if(mState == SeesawDriver::ONTHESEESAW){
		if(mInitState){
			K_THETADOT = 5.0F;
			mAngleTrace.setTargetAngle(350.0);
			mAngleTrace.setForward(25);
			mInitState = false;
		}
		if (fabs(mLeftMotorHistory.calcDifference()) < 5 && (mLeftMotorHistory.get() - mStartAngleCounter) > 360) {
            mState = SeesawDriver::LINERETURN;
            mInitState = true;
		}
		mAngleTrace.execute();
	}else if (mState == SeesawDriver::LINERETURN) {
        if (mInitState) {
            mInitState = false;
            K_THETADOT = 7.5F;
        }
        if (mLineDetector.detect()) {
            mState = SeesawDriver::AFTERLINETRACE;
            mInitState = true;
        }
        VectorT<F32> command(15, -5);
        mActivator.run(command);
    }else if (mState == SeesawDriver::AFTERLINETRACE) {
        if (mInitState) {
            K_THETADOT = 6.5F;
            mLineTrace.setForward(20);
//        	mLineTrace.reset();
            mInitState = false;
        }
    	if(mTimeCounter > 500){
            K_THETADOT = 7.5F;
            mLineTrace.setForward(100);
    	}
        mLineTrace.execute();
    	mTimeCounter++;
    }
    
/*こっちが本物！        
    if(timecounter < 750){
        mAngleTrace.setTargetAngle(180.0);
        mAngleTrace.setForward(100);
        mAngleTrace.execute();
    }else if(750 <= timecounter && timecounter < 1000){
        mAngleTrace.setTargetAngle(180.0);
        mAngleTrace.setForward(0);
        mAngleTrace.execute();
        tail_control(75);
    }else if(1000 <= timecounter && timecounter < 1100){
        tail_control(75);
        pwm_L = pwm_R = 100;
        nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); 
        nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); 
    }else if(1100 <= timecounter && timecounter < 1350){
        tail_control(75);
        pwm_L = pwm_R = 0;
        nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); 
        nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); 
    }else if(1350 <= timecounter && timecounter < 5000){
        tail_control(75);
        pwm_L = pwm_R = -40;
        nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); 
        nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); 
    }
    timecounter++;

*/    
    
    
//	return mState == SeesawDriver::AFTERLINETRACE; // 終了しました
    return true;
}

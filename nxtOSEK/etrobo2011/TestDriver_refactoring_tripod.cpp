///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
extern "C" extern void tail_control(signed int angle);

/**
 * コンストラクタ
 */
TestDriver::TestDriver()
{
    mState = -1; // 初期化状態
    
}

bool TestDriver::drive()
{
#if 0 // ログ送信
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(mLineDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLightHistory.calcDifference());
#endif
#if 0 // DEBUG
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // staticは原則禁止だが今だけ
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "TestDriver");
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        lcd.putf("dn", (S32)(mRightMotor.getCount()));
        //lcd.putf("dn", (S32)(mLineDetector.detect()));
        //lcd.putf("dn", (S32)(mLightHistory.calcDifference()));
        lcd.disp();
    }
#endif
    // デフォルト

    /* 参考：GPSは以下の値で初期化されている
    mXCoordinate(GPS_COURSE_START_X),
    mYCoordinate(GPS_COURSE_START_Y),
    mDirection(GPS_COURSE_START_DIRECTION),
    */
    // テスト ３点走行ライントレース
    
    // テスト
    //gDoTripod = false;
    // テスト 普通のライントレース
#if 1
    gDoMaimai = false;
    gDoTripod = false;
    mLineTrace.setForward(50);
    mLineTrace.execute();
#endif
    
    
#if 1
    gDoMaimai = true;
    if(!gDoTripod){
        mSitDownSkill.execute();
    }else{
        gDoTripod = true;//SitDownSkillでtureにしているが念のため明示的に記載する
        mLineTrace.setForward(50);
        mLineTrace.execute();
    }
#endif
    

#if 0
    if(!gDoTripod){
        mSitDownSkill.execute();
    }else{
        gDoTripod = true;//SitDownSkillでやっているが念のため明示的に記載する
        mAngleTrace.setTargetAngle(GPS_COURSE_START_DIRECTION + 45);
    	mAngleTrace.setForward(50);
        mAngleTrace.execute();
    }
#endif
	// テスト ３点走行位置トレース
#if 0
    if(!gDoTripod){
        mSitDownSkill.execute();
    }else{
        gDoTripod = true;//SitDownSkillでやっているが念のため明示的に記載する
        mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_COURSE_START_X - 500.0, GPS_COURSE_START_Y -500.0));//
        mCoordinateTrace.setForward(50);
        mCoordinateTrace.execute();
    }
#endif
    return true;
}

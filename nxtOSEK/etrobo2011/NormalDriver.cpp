///////////////////////////////////////////////////////////
//  NormalDriver.cpp
//  Implementation of the Class NormalDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "NormalDriver.h"
#include "factory.h"
extern bool gDoForwardPid;
extern bool gDoMaimai;

/**
 * コンストラクタ
 */
NormalDriver::NormalDriver()
{
}

/**
 * ノーマル走行アルゴリズム
 *
 * ただのライントレース
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool NormalDriver::drive()
{
    gDoForwardPid = true;
    gDoMaimai = true; //!< まいまい式発動フラグ
	
#ifdef INCOURSE
	// スタートから坂道頂上まで
    if(0 < mGps.getDistance() && mGps.getDistance() < 2300.0) {
        mLightPid.reset(30,0,90);
        mForwardPid.reset(0.003, 0, 0.01);
        mLineTrace.setForward(150);
    }
	
	// 坂道頂上直前から坂道頂上直後まで
	else if(2300.0 < mGps.getDistance() && mGps.getDistance() < 2400.0) {
        mLightPid.reset(30,0,90);
        mForwardPid.reset(0.003, 0, 0.01);
        mLineTrace.setForward(90);
    }
	
	// 坂道頂上直後から下り坂中腹まで
	else if(2400.0 < mGps.getDistance() && mGps.getDistance() < 2600.0) {
        mLightPid.reset(30,0,90);
        mForwardPid.reset(0.3, 0, 0.9);
		mLineTrace.setForward(60);
    }
	// 下り坂中腹から第1コーナーの入り口まで
	else if(2600.0 < mGps.getDistance() && mGps.getDistance() < 2900.0) {
        mLightPid.reset(30,0,90);
        mForwardPid.reset(0.1, 0, 0.3);
		mLineTrace.setForward(90);
    }
	// 第1コーナーの入り口から第1コーナー出口まで
    else if(2900.0 < mGps.getDistance() && mGps.getDistance() < 4900.0) {
        mLightPid.reset(70,0,210);
    	mForwardPid.reset(0.003, 0, 0.01);
    	mLineTrace.setForward(100);
    }
	
	// 第1コーナーの出口から第2コーナーの入り口まで
    else if(4900.0 < mGps.getDistance() && mGps.getDistance() < 6000.0) {
        mLightPid.reset(70,0,210);
    	mForwardPid.reset(0.003, 0, 0.01);
    	mLineTrace.setForward(120);
    }	
	
	// 第2コーナーの入り口から最後まで
    else if(6000.0 < mGps.getDistance()) {
        mLightPid.reset(100,0,300);
    	mForwardPid.reset(0.003, 0, 0.01);
    	mLineTrace.setForward(100);
    }
#else
#endif
	
    mLineTrace.execute();
    return true;
}

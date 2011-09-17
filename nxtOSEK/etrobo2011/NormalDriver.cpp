///////////////////////////////////////////////////////////
//  NormalDriver.cpp
//  Implementation of the Class NormalDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "NormalDriver.h"
#include "factory.h"


extern bool	gDoForwardPid = true;
extern bool	gDoMaimai = true; //!< まいまい式発動フラグ
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
	
    if(6000.0 < mGps.getDistance()){
        mLineTrace.setForward(100);
    }else{
    	mLineTrace.setForward(150);
    }
    mLineTrace.execute();
    return true;
}

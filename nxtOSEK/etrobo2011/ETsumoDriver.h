///////////////////////////////////////////////////////////
//  ETsumoDriver.h
//
//  Created on:      16-7-2011 08:43:43
//  Original author: Takahashi
///////////////////////////////////////////////////////////

#ifndef ETSUMODRIVER_H
#define ETSUMODRIVER_H

#include "Driver.h"
#include "Motor.h"
#include "constants.h"
#include "macro.h"
#include "struct.h"
#include <math.h>
#include <float.h>

/**
  ET相撲ドライバ
*/
class ETsumoDriver : public Driver
{
private:
    /**
     * サブ区間
     */
    enum eSubSection {
        INIT = -1,
    	TEST,
    	BEFORELINETRACE,
    	PREPARE_SPOTSEARCH,
        SPOTSEARCH,
    	SPOTSEARCH_to_SWINGSEARCH,
        SWINGSEARCH,
    	SCAN,
    	REVERSE,
    	DOHYO_IN,
        HAKKE_READY,
    	NOKOTTA_GO,
        KACHI_NANORI,
        AFTERLINETRACE
    };
    eSubSection mState;    //!< 状態
	 /**
     * ターゲットとの位置関係
     */
    enum eScanState {
        UNKNOWN = -1,
    	DETECT,
        SWINGRIGHT,
    	SWINGLEFT,
    	CALC,
    	LOCKON,
    };
	eScanState mScanState; 
	
    bool mInitState;       //!< 状態初期化フラグ
    int  mTimeCounter;     //!< タイムカウンタ
    bool mIsArrived;       //!< isArrived保存バッファ
    int  mSonarTotalDistance;
	float mSonarTotalAngle;
    float mOrigK_PHIDOT;   //!< 変更前のK_PHIDOTを保存しておくバッファ
	int  mSonarDetectCount;
	float mTargetX;
	float mTargetY;
	float mTargetAngle;
	float mTargetDistance;
	int mFailScanCounter;
	float mTargetTotalX;
	float mTargetTotalY;
	int mPrevSonarDetectCount;
	bool mOshidashiFlag;
public:
	ETsumoDriver();
	~ETsumoDriver();
    bool drive();
private:
	//void calcTargetCoordinates();
	void updateTargetCoordinates();
	//void swingTimer(int time_interval, float direction_interval);
	float calcTargetAngle(float targetX, float targetY);
};
#endif // !ETSUMODRIVER_H

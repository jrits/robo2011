///////////////////////////////////////////////////////////
//  StandupDriver.h
//  Definition of the Class StandupDriver
//  Created on:      10-9-2010 19:43:43
///////////////////////////////////////////////////////////
#ifndef StandupDriver_H_
#define StandupDriver_H_
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */


#include "Driver.h"

/**
 * スタンドアップドライバ
 */
class StandupDriver : public Skill
{ 
public:
		enum eSubSection {
        INIT = -1,
        TAILPUSH,
        STOPTAILPUSH,
		FINISH
    };
	StandupDriver();
	~StandupDriver(){}
    bool drive();
	int mState;//状態遷移変数
	int mCounter;//停止用カウンター
	float mTargetTailAngle;
	void setTargetTailAngle(float targetTailAngle);//尻尾の目標角度
};

#endif

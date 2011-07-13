///////////////////////////////////////////////////////////
//  StairwayDriver.h
//
//  Created on:      10-9-2010 19:43:43
//  Original author: hayama
///////////////////////////////////////////////////////////

#ifndef STAIRWAYDRIVER_H
#define STAIRWAYDRIVER_H

#include "Driver.h"

/**
 * 階段ドライバ
 */
class StairwayDriver : public Driver
{
public:
    /**
     * サブ区間
     */
    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        ON1STSTAGE,
        ON2NDSTAGE,
        DROPDOWN,
        AFTERLINETRACE
    };
private:
    int mState; //!< 階段状態
    bool mInitState; //!< 状態初期化フラグ
    int mTimeCounter; //!< タイムカウンタ
    int mPrevMotor; //!< 前のモータの回転数
    bool mDoDetectWall; //!< 障壁を発見フラグ
    float mPrevDirection; //!< 前の方向
public:
    StairwayDriver();
	~StairwayDriver(){}
    bool drive();
};
#endif // STAIRWAYDRIVER_H


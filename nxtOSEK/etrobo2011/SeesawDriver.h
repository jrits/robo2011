///////////////////////////////////////////////////////////
//  SeesawDriver.h
//
//  Created on:      10-9-2010 19:43:43
//  Original author: masui
///////////////////////////////////////////////////////////

#ifndef SEESAWDRIVER_H
#define SEESAWDRIVER_H

#include "Driver.h"

/**
  シーソードライバ
*/
class SeesawDriver : public Driver
{
public:
    /**
     * サブ区間
     */
    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        ON0THSTAGE_BACK,
        ON0THSTAGE_GO,
        DETECTINGRUN,
        BACKRUN,
        FORWARDRUN,
        LINERETURN,
        AFTERLINETRACE
    };
private:
    int mState;    //!< 状態
    int mTimeCounter; //!< タイマカウンタ
    bool mInitState; //!< 初期状態
    float mStartAngleCounter; //!< 初期角度カウンタ
    float mPrevDirection; //!< 前の方向
    int mPrevMotor; //!< 前のモータの回転数
    bool mDoDetectWall; //!< 障壁を発見フラグ

public:
    SeesawDriver();
    ~SeesawDriver();
    bool drive();
};
#endif // !SEESAWDRIVER_H

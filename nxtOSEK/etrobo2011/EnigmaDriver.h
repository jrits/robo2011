//
// EnigmaDriver.h
//

#ifndef EnigmaDriver_H_
#define EnigmaDriver_H_

#include "Driver.h"

/**
 * エニグマ・デコーディングドライバ
 */
class EnigmaDriver : public Driver
{
private:
    /**
     * サブ区間
     */
    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        PREPAREENTERPET,
        ENTERPET,
        LEFTPET,
        RIGHTPET,
        EXITPET,
        LINERETURN,
        AFTERLINETRACE
    };
    eSubSection mState;    //!< 状態
    bool mInitState;       //!< 状態初期化フラグ
    int  mTimeCounter;     //!< タイムカウンタ
    bool mIsArrived;       //!< isArrived保存バッファ
    bool mObstacleFlag[2]; //!< 衝立検知フラグ。２本しかみない
    float mOrigK_PHIDOT;   //!< 変更前のK_PHIDOTを保存しておくバッファ
public:
	EnigmaDriver();
	~EnigmaDriver(){}
    bool drive();
public:
    bool detect();
    int decode();
};

#endif

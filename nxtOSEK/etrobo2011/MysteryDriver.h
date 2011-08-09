//
// MysteryDriver.h
//

#ifndef MysteryDriver_H_
#define MysteryDriver_H_

#include "Driver.h"

/**
 * ミステリーサークルドライバ
 */
class MysteryDriver : public Driver
{
private:
    /**
     * サブ区間
     */
    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        MYSTERYROUTE,
        LINERETURN,
        AFTERLINETRACE
    };
    eSubSection mState; //!< 状態
    bool mInitState;    //!< 状態初期化フラグ
    int mTimeCounter;   //!< タイムカウンタ
    int mRoute;         //!< 0-3. どのルートを辿るか
public:
	MysteryDriver();    //コンストラクタ
	~MysteryDriver(){}  //デストラクタ
    bool drive();       //!< ミステリードライバ実行メソッド
};

#endif

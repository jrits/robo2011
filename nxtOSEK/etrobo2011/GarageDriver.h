//
// GarageDriver.h
//

#ifndef GarageDriver_H_
#define GarageDriver_H_

#include "Driver.h"

/**
 * アウトコースガレージ・インドライバ
 */
class GarageDriver : public Driver
{
private:
    /**
     * サブ区間
     */
    enum eSubSection {
        INIT = -1,
        READYGO,
        SITDOWN,
        MARKER,
        STOP,
    };
    eSubSection mState; //!< 状態
    int mTimeCounter;   //!< タイムカウンタ
    float mPrevDistance; //!< 直前走行距離
public:
    GarageDriver();
    ~GarageDriver(){}
    bool drive();
};

#endif

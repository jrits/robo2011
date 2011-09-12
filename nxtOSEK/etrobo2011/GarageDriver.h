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
        MARKER,
        ENTER,
        STOP
    };
    eSubSection mState; //!< 状態
    int mTimeCounter;   //!< タイムカウンタ
public:
    GarageDriver();
    ~GarageDriver(){}
    bool drive();
};

#endif

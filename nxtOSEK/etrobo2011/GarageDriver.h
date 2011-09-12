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
        BEFORELINETRACE,
        STRAIGHTLINETRACE,
        PREPAREENTERGARAGE,
        STRAIGHTANGLETRACE,
        GOGARAGE,
        ENTERGARAGE,
        FIN_GARAGEIN,
    };
    eSubSection mState; //!< 状態
    int mTimeCounter;   //!< タイムカウンタ
    bool mGrayThroughFlag; //!< 大会調整。灰色区間ジャンプ
public:
    GarageDriver();
    ~GarageDriver(){}
    bool drive();
};

#endif

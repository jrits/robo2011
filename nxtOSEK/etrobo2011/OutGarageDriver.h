//
// OutGarageDriver.h
//

#ifndef OutGarageDriver_H_
#define OutGarageDriver_H_

#include "Driver.h"

/**
 * アウトコースガレージ・インドライバ
 */
class OutGarageDriver : public Driver
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
	OutGarageDriver();
	~OutGarageDriver(){}
    bool drive();
};

#endif

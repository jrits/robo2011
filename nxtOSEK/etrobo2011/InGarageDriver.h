//
// InGarageDriver.h
//

#ifndef InGarageDriver_H_
#define InGarageDriver_H_

#include "Driver.h"

/**
 * インコースガレージ・インドライバ
 */
class InGarageDriver : public Driver
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
    	ENTERGARAGE,
    };
    eSubSection mState; //!< 状態
    int mTimeCounter;   //!< タイムカウンタ
public:
	InGarageDriver();
	~InGarageDriver(){}
    bool drive();
};

#endif

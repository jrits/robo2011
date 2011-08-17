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

public:
	EnigmaDriver();
	~EnigmaDriver(){}
    bool drive();
public:
    bool detect();
    int decode();
};

#endif

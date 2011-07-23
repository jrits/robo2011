//
// EnigmaDriver.h
//

#ifndef EnigmaDriver_H_
#define EnigmaDriver_H_

#include "Driver.h"

/**
 * �G�j�O�}�E�f�R�[�f�B���O�h���C�o
 */
class EnigmaDriver : public Driver
{
private:
    /**
     * �T�u���
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
    eSubSection mState;    //!< ���

public:
	EnigmaDriver();
	~EnigmaDriver(){}
    bool drive();
public:
    bool detect();
    int decode();
};

#endif

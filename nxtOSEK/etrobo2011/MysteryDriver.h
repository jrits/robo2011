//
// MysteryDriver.h
//

#ifndef MysteryDriver_H_
#define MysteryDriver_H_

#include "Driver.h"

/**
 * �~�X�e���[�T�[�N���h���C�o
 */
class MysteryDriver : public Driver
{
private:
    /**
     * �T�u���
     */
    enum eSubSection {
        INIT = -1,

    };
    eSubSection mState; //!< ���
    bool mInitState;    //!< ��ԏ������t���O
public:
	MysteryDriver();    //�R���X�g���N�^
	~MysteryDriver(){}  //�f�X�g���N�^
    bool drive();       //!< �~�X�e���[�h���C�o���s���\�b�h
};

#endif

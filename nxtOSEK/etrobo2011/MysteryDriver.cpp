//
// MysteryDriver.cpp
//

#include "MysteryDriver.h"
#include "factory.h"
#include "constants.h"

//�~�X�e���[�T�[�N���̃��[�g(4�p�^�[��)
//#include "MysteryLineLL.h"
//#include "MysteryLineLR.h"
//#include "MysteryLineRL.h"
//#include "MysteryLineRR.h"

/**
 * �R���X�g���N�^
 */
MysteryDriver::MysteryDriver()
{
    mState = MysteryDriver::INIT;
    mInitState = true;
}

/**
 * �~�X�e���[�T�[�N����Ԃ��U������
 *
 * �T�[�N���O�������C���g���[�X����A���C�����A�ド�C���g���[�X�܂ł�S������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
bool MysteryDriver::drive()
{
    return 0;
}

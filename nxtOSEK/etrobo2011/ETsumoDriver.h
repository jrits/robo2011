///////////////////////////////////////////////////////////
//  ETsumoDriver.h
//
//  Created on:      16-7-2011 08:43:43
//  Original author: Takahashi
///////////////////////////////////////////////////////////

#ifndef ETSUMODRIVER_H
#define ETSUMODRIVER_H

#include "Driver.h"
#include "Motor.h"
#include "constants.h"
#include "macro.h"
#include "struct.h"
#include <math.h>
#include <float.h>

/**
  ET���o�h���C�o
*/
class ETsumoDriver : public Driver
{
private:
    /**
     * �T�u���
     */
    enum eSubSection {
        INIT = -1,
        TEST,
        BEFORELINETRACE,
        PREPARE_SPOTSEARCH,
        SPOTSEARCH,
        SPOTSEARCH_to_SWINGSEARCH,
        SWINGSEARCH,
        SCAN,
        REVERSE,
        DOHYO_IN,
        HAKKE_READY,
        NOKOTTA_GO,
        KACHI_NANORI,
        AFTERLINETRACE
    };
    eSubSection mState;    //!< ���
     /**
     * �^�[�Q�b�g�Ƃ̈ʒu�֌W
     */
    enum eScanState {
        UNKNOWN = -1,
        DETECT,
        SWINGRIGHT,
        SWINGLEFT,
        CALC,
        LOCKON,
    };
    eScanState mScanState; 
    
    bool mInitState;       //!< ��ԏ������t���O
    int  mTimeCounter;     //!< �^�C���J�E���^
    bool mIsArrived;       //!< isArrived�ۑ��o�b�t�@
    bool mOshidashiFlag;   //!< �����o������t���O
    float mOrigK_THETADOT; //!< �ύX�O��THETADOT��ۑ����Ă����o�b�t�@
    float mOrigK_PHIDOT;   //!< �ύX�O��K_PHIDOT��ۑ����Ă����o�b�t�@
    int  mSonarDetectCount;
    int  mPrevSonarDetectCount;
    int  mFailScanCounter;
    float mTargetX;
    float mTargetY;
    float mTargetTotalX;
    float mTargetTotalY;
    float mTargetAngle;

public:
    ETsumoDriver();
    ~ETsumoDriver();
    bool drive();
private:
    //void calcTargetCoordinates();
    void updateTargetCoordinates();
    //void swingTimer(int time_interval, float direction_interval);
    float calcTargetAngle(float targetX, float targetY);
};
#endif // !ETSUMODRIVER_H

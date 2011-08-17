///////////////////////////////////////////////////////////
//  TestDriver.h
//  Definition of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
///////////////////////////////////////////////////////////
#ifndef TESTDRIVER_H_
#define TESTDRIVER_H_
#define P_GAIN             2.5F /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60 /* ���S��~�p���[�^����PWM��΍ő�l */


#include "Driver.h"

/**
 * �e�X�g�h���C�o
 */
class TestDriver : public Driver
{ 
public:
		enum eSubSection {
        INIT = -1,
        TAILPUSH,
        STOPTAILPUSH
    };
	TestDriver();
	~TestDriver(){}
    bool drive();
//	void mTail_control(signed int angle);
	int mState;
	int SSC;//State Switch Counter
	float beforeRMH; //��ԕω����̃��[�^��]�p�x�i�ώZ�l�j
	signed char mPwm_L;
	signed char mPwm_R;
};

#endif

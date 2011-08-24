///////////////////////////////////////////////////////////
//  StandupDriver.h
//  Definition of the Class StandupDriver
//  Created on:      10-9-2010 19:43:43
///////////////////////////////////////////////////////////
#ifndef StandupDriver_H_
#define StandupDriver_H_
#define P_GAIN             2.5F /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60 /* ���S��~�p���[�^����PWM��΍ő�l */


#include "Driver.h"

/**
 * �X�^���h�A�b�v�h���C�o
 */
class StandupDriver : public Skill
{ 
public:
		enum eSubSection {
        INIT = -1,
        TAILPUSH,
        STOPTAILPUSH,
		FINISH
    };
	StandupDriver();
	~StandupDriver(){}
    bool drive();
	int mState;//��ԑJ�ڕϐ�
	int mCounter;//��~�p�J�E���^�[
	float mTargetTailAngle;
	void setTargetTailAngle(float targetTailAngle);//�K���̖ڕW�p�x
};

#endif

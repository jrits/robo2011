///////////////////////////////////////////////////////////
//  StandupDriver.cpp
//  Implementation of the Class StandupDriver
//  Created on:      24-8-2011 20:51:45
//  Original author: hayama
///////////////////////////////////////////////////////////
#include "StandupDriver.h"
#include "factory.h"
#include "Speaker.h"

/**
 * �R���X�g���N�^
 */
StandupDriver::StandupDriver()
{
    mState = INIT; // ���������
	mTargetTailAngle = 0;//�����p�x
	mCounter = 0;
}

bool StandupDriver::drive()
{
	mWallDetector.setThreshold(100);
    if(mState == INIT){
        nxt_motor_set_count(NXT_PORT_A, mTailMotor.getCount()); //���݂̐K���̊p�x����x�����擾�A�ێ�
        mState = TAILPUSH;
    }
    if(mState == TAILPUSH){
        if(mTailMotor.getCount() <= mTargetTailAngle){ //�K����ڕW�p�x�܂œ�����
            nxt_motor_set_speed(NXT_PORT_A, 60, 1);
        }else{
            nxt_motor_set_speed(NXT_PORT_A, 0, 1);//��x�K�����~�߂�
            mState = STOPTAILPUSH;
        }
   	}else if(mState == STOPTAILPUSH){
        if(mTailMotor.getCount() >= 2){//�K����������Ԃɖ߂�
            nxt_motor_set_speed(NXT_PORT_A, -100, 1);
        }
		if(mCounter <= 500){//2�b�Ԉ��肳����
		    //�A���O���g���[�X���g���Ă��̏�ɗ���
			mAngleTrace.setForward(0);
			mAngleTrace.setTargetAngle(180);
			mAngleTrace.execute();
			mCounter++;
		}else{
			mState = FINISH;
		}
   	}
    return   mState == FINISH;
}

/**
 * �K���̖ڕW�p�x���w�肷��
 *
 * @return �Ȃ�
 */
void StandupDriver::setTargetTailAngle(float targetTailAngle)
{
	mTargetTailAngle = targetTailAngle;
}


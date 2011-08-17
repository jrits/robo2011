///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
#include "Speaker.h"

/**
 * �R���X�g���N�^
 */
TestDriver::TestDriver()
{
//    mState = BEFOREANGLETRACE; // ���������
    mState = INIT; // ���������
}

bool TestDriver::drive()
{
	mWallDetector.setThreshold(100);
    if(mState == INIT){
        nxt_motor_set_count(NXT_PORT_A, 0); //���݂̐K���̊p�x����x�����擾�A�ێ�
        mState = TAILPUSH;
    }
    if(mState == TAILPUSH){
        if(mTailMotor.getCount() <= 114){ //�K����ڕW�p�x�܂œ�����
            nxt_motor_set_speed(NXT_PORT_A, 60, 1);
        }else{
            nxt_motor_set_speed(NXT_PORT_A, 0, 1);//��x�K�����~�߂�
            mState = STOPTAILPUSH;
        }
   	}else if(mState == STOPTAILPUSH){
        if(mTailMotor.getCount() >= 2){//�K����������Ԃɖ߂�
            nxt_motor_set_speed(NXT_PORT_A, -100, 1);
        }
  		K_THETADOT = 7.5;
   	    //�A���O���g���[�X���g���Ă��̏�ɗ���
  		mAngleTrace.setForward(0);
  		mAngleTrace.setTargetAngle(180);
  		mAngleTrace.execute();
  		}
  	}

    return true;
}

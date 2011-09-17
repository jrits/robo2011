///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
extern "C" extern void tail_control(signed int angle);

/**
 * �R���X�g���N�^
 */
TestDriver::TestDriver()
{
    mState = -1; // ���������
    
}

bool TestDriver::drive()
{
#if 0 // ���O���M
    LOGGER_SEND = 2;
	LOGGER_DATAS08[0] = (S8)(mLineDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLightHistory.calcDifference());
#endif
#if 0 // DEBUG
    //DESK_DEBUG = true; // ���[�^���񂳂Ȃ��f�o�O
    static int count = 0; // static�͌����֎~����������
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "TestDriver");
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.putf("dn", (S32)(mLeftMotor.getCount()));
        lcd.putf("dn", (S32)(mRightMotor.getCount()));
        //lcd.putf("dn", (S32)(mLineDetector.detect()));
        //lcd.putf("dn", (S32)(mLightHistory.calcDifference()));
        lcd.disp();
    }
#endif
    // �f�t�H���g

    /* �Q�l�FGPS�͈ȉ��̒l�ŏ���������Ă���
    mXCoordinate(GPS_COURSE_START_X),
    mYCoordinate(GPS_COURSE_START_Y),
    mDirection(GPS_COURSE_START_DIRECTION),
    */
    // �e�X�g �R�_���s���C���g���[�X
    
    // �e�X�g
    //gDoTripod = false;
    // �e�X�g ���ʂ̃��C���g���[�X
#if 1
    gDoMaimai = false;
    gDoTripod = false;
    mLineTrace.setForward(50);
    mLineTrace.execute();
#endif
    
    
#if 1
    gDoMaimai = true;
    if(!gDoTripod){
        mSitDownSkill.execute();
    }else{
        gDoTripod = true;//SitDownSkill��ture�ɂ��Ă��邪�O�̂��ߖ����I�ɋL�ڂ���
        mLineTrace.setForward(50);
        mLineTrace.execute();
    }
#endif
    

#if 0
    if(!gDoTripod){
        mSitDownSkill.execute();
    }else{
        gDoTripod = true;//SitDownSkill�ł���Ă��邪�O�̂��ߖ����I�ɋL�ڂ���
        mAngleTrace.setTargetAngle(GPS_COURSE_START_DIRECTION + 45);
    	mAngleTrace.setForward(50);
        mAngleTrace.execute();
    }
#endif
	// �e�X�g �R�_���s�ʒu�g���[�X
#if 0
    if(!gDoTripod){
        mSitDownSkill.execute();
    }else{
        gDoTripod = true;//SitDownSkill�ł���Ă��邪�O�̂��ߖ����I�ɋL�ڂ���
        mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_COURSE_START_X - 500.0, GPS_COURSE_START_Y -500.0));//
        mCoordinateTrace.setForward(50);
        mCoordinateTrace.execute();
    }
#endif
    return true;
}

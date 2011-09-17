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
        lcd.putf("dn", (S32)(mLightSensor.get()));
        lcd.disp();
    }
#endif
    // �f�t�H���g
    tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */
    gDoForwardPid = false;

    // �e�X�g ��}�C�}�C�����C���g���[�X�}�[�J�[���m
    if (0) {
        gDoMaimai = false;
    }
    // �e�X�g �}�C�}�C�����C���g���[�X�}�[�J�[���m
    if (1) {
        gDoMaimai = true;
    }

    // �ȉ��A���ʃ��W�b�N
    {
        mAngleTrace.setForward(30);
        mAngleTrace.setTargetAngle(180);
        mAngleTrace.execute();
        // ���C�����m
        if (mLineDetector.detect()) {
            Speaker speaker;
            speaker.playTone(1976, 10, 100); // Hz:33-1976 , 10ms, volume:0-100
        }
    }

    return true;
}

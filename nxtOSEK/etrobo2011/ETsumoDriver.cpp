///////////////////////////////////////////////////////////
//  ETsumoDriver.cpp
//  Implementation of the Class ETsumoDriver
//  Created on:      16-7-2011 08:43:43
//  Original author: Takahashi
///////////////////////////////////////////////////////////

#include "ETsumoDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"

#define K_PHIDOT_FOR_SEARCH 15.0F
#define RIKISHI_FORWARD 10.0

#define SUMO_DEBUG 1

extern bool gDoSonar; //!< �\�i�[�Z���T�����t���O
extern bool gSonarIsDetected; //!< �՗����m�̌���
extern int  gSonarTagetDistance;//!< �\�i�[�Z���T�̋�������
extern float gSonarTagetAngle;


/**
 * �R���X�g���N�^
 */
ETsumoDriver::ETsumoDriver()    //�����l�̐ݒ�
{
    mTimeCounter = 0;
    mInitState = false;             
    mState = ETsumoDriver::INIT;  //�X�e�[�g
}

/**
 * �f�X�g���N�^
 */
ETsumoDriver::~ETsumoDriver(){}

/**
 * ���s����BET���o���U������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
 

bool ETsumoDriver::drive()
{        
    if (mState == ETsumoDriver::INIT) { // ���������
        gDoSonar = false;
        mTimeCounter = 0;
        mOrigK_THETADOT =  K_THETADOT; // ��Ŗ߂����߂ɕۑ�
        mOrigK_PHIDOT = K_PHIDOT; // ��Ŗ߂����߂ɕۑ�
        K_THETADOT = 6.5F;
        K_PHIDOT = 25.0F;
        mScanState = UNKNOWN;
        mLightSensor.setLamp(0);//�����Ƀ��C�g�Z���TOFF
        
        //��ԑJ��
        mInitState = true;
        mState = ETsumoDriver::PREPARE_SPOTSEARCH;
    }

    if (mState == ETsumoDriver::PREPARE_SPOTSEARCH) {
        if (mInitState) {
            gDoSonar = false;
            K_THETADOT = 6.5F;
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X, GPS_ETSUMO_SEARCH_Y));// ��todo�v�Đݒ�
            mCoordinateTrace.setForward(30.0);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
        }
        // �ړ�����
        if (mCoordinateTrace.isArrived()) {
            mInitState = true;
            mState = ETsumoDriver::SPOTSEARCH;
        }
        mCoordinateTrace.execute();
    }
    
    
    if (mState == ETsumoDriver::SPOTSEARCH) {
        if (mInitState) {
            gDoSonar = false; // 
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(360.0);
            mAngleTrace.setAllowableError(2.0); // 2�x
            mInitState = false;
            mIsArrived = false;
            mSonarDetectCount = 0;
            mTargetTotalX = 0;
            mTargetTotalY = 0;
        }
        // �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            gDoSonar = true;
            mIsArrived = true;
        }
        // �����]���������Ă���X�|�b�g�T�[�`�J�n
        if(mIsArrived && (mTimeCounter % 20 == 0) && (mTimeCounter >= 1000)){
            if(gSonarIsDetected){
                mSonarDetectCount++;
                updateTargetCoordinates();
                if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
            }
            else if(mTimeCounter % 100 == 0){
                float setangle = mGps.getDirection() - 10;//���̐��l�͏\���傫�����߁A�����s�v�̂͂�
                mAngleTrace.setTargetAngle(setangle);
            }
        }
        
        //�X�|�b�g�T�[�`�͈͓��ŒT�m�����I
        if(mSonarDetectCount >= 3){
            mInitState = true;
            mTargetX = mTargetTotalX / mSonarDetectCount;
            mTargetY = mTargetTotalY / mSonarDetectCount;
            mState = ETsumoDriver::DOHYO_IN;
        }
        //�X�|�b�g�T�[�`�͈͓��Ƀy�b�g�{�g�����������Ƃ��m�F�A�X�C���O�T�[�`�ֈڍs
        else if((Gps::marge180(mGps.getDirection()) <= -90.0) && mIsArrived){
            mInitState = true;
            mState = ETsumoDriver::SPOTSEARCH_to_SWINGSEARCH;
        }
        mAngleTrace.execute();
    }
    
    if (mState == ETsumoDriver::SPOTSEARCH_to_SWINGSEARCH) {
        if (mInitState) {
            gDoSonar = false;
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X + 200.0, GPS_ETSUMO_SEARCH_Y - 200.0));// ��todo�v�Đݒ�
            mCoordinateTrace.setForward(RIKISHI_FORWARD);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
        }
        // �ړ�����
        if (mCoordinateTrace.isArrived()) {
            mInitState = true;
            mState = ETsumoDriver::SWINGSEARCH;
        }
        mCoordinateTrace.execute();
    }
        
    if (mState == ETsumoDriver::SWINGSEARCH) {
        if (mInitState) {
            gDoSonar = false;
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mSonarDetectCount = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(480);
            mAngleTrace.setAllowableError(2.0); // 2�x
            mInitState = false;
            mIsArrived = false;
            mTargetTotalX = 0;
            mTargetTotalY = 0;
        }
        // �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
            K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            gDoSonar = true; // �\�i�[�N��
        }
        // �����]���������Ă���X�C���O�T�[�`�J�n
        if(mIsArrived && (mTimeCounter % 20 == 0)){
            if(gSonarIsDetected){
                mSonarDetectCount++;
                updateTargetCoordinates();
                if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
            }
            else if((mTimeCounter % 100 == 0) && (mTimeCounter > 500)){
                float setangle = mGps.getDirection() - 10;
                mAngleTrace.setTargetAngle(setangle);
            }
        }
        
        //�X�C���O�T�[�`�͈͓��ŒT�m�����I
        if(mSonarDetectCount >= 3){
            mInitState = true;
            mTargetX = mTargetTotalX / mSonarDetectCount;
            mTargetY = mTargetTotalY / mSonarDetectCount;
            mState = ETsumoDriver::DOHYO_IN;
        }
        mAngleTrace.execute();
    }
    
    if (mState == ETsumoDriver::DOHYO_IN) {
        if (mInitState) {
            mTimeCounter = 0;
            K_PHIDOT = 60.0F;
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//�^�[�Q�b�g�̃A���O����-180�`180�ŕԂ�
            if((mTargetAngle > -45) && (mTargetAngle < 135)){
                mCoordinateTrace.setTargetCoordinate(MakePoint(mTargetX - 300, mTargetY));
            }
            else{
                mCoordinateTrace.setTargetCoordinate(MakePoint(mTargetX, mTargetY + 300));
            }
            mCoordinateTrace.setForward(RIKISHI_FORWARD);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
            gDoSonar = false; 
        }
        // �ړ�����
        if (! mIsArrived && mCoordinateTrace.isArrived()) {
            mInitState = true;
            mState = ETsumoDriver::HAKKE_READY;
        }
        mCoordinateTrace.execute();
    }
    
    
    if (mState == ETsumoDriver::HAKKE_READY) {
        if (mInitState) {
            gDoSonar = false; 
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            K_PHIDOT = 10.0;
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//�^�[�Q�b�g�̃A���O����-180�`180�ŕԂ�
            if((mTargetAngle > -45) && (mTargetAngle < 135)){
                mAngleTrace.setTargetAngle(45);
            }
            else{
                mAngleTrace.setTargetAngle(-45);
            }            
            mAngleTrace.setAllowableError(2.0); // 2�x
            mInitState = false;
            mIsArrived = false;
        }
        // �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        // �����]���������Ă��痎�������܂őҋ@
        if(mIsArrived && (mTimeCounter > 1000)){
            mInitState = true;
            mState = ETsumoDriver::SCAN;
        }
        
        mAngleTrace.execute();
    }
    if (mState == ETsumoDriver::SCAN) {
        if (mInitState) {
            gDoSonar = true; // �\�i�[�N��
            K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            mTimeCounter = 0;
            mSonarDetectCount = 0;
            mFailScanCounter = 0;
            mTargetTotalX = 0;
            mTargetTotalY = 0;
            mAngleTrace.setForward(0);
            mInitState = false;
            mIsArrived = false;
            mScanState = SWINGRIGHT;
        }
        //�ŏ��͉E���ɃX�L�����A�^�[�Q�b�g���X�g��ɍ����ɃX�L����
        if(mTimeCounter % 20 == 0){
            if(gSonarIsDetected){
                mFailScanCounter = 0; //���m�����猟�m���s�񐔂����Z�b�g
                mSonarDetectCount++;
                updateTargetCoordinates();
                if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
            }
            else{
                mFailScanCounter++;
                if(mFailScanCounter > 255){
                    mFailScanCounter = 255;//�O�̂���
                }
            }
            //�E���ɃX�L����
            if((mTimeCounter % 100 == 0) && (mScanState == SWINGRIGHT)){
                float setangle = mGps.getDirection() - 10;
                mAngleTrace.setTargetAngle(setangle);
            }
            //�����ɃX�L����
            else if((mTimeCounter % 100 == 0) && (mScanState == SWINGLEFT)){
                float setangle = mGps.getDirection() + 10;
                mAngleTrace.setTargetAngle(setangle);
            }
        }
        //�^�[�Q�b�g���X�g��ɏ�ԑJ��
        if((mFailScanCounter >= 10) && (mSonarDetectCount >= 2)){
            //�E���Ȃ獶���֏�ԑJ��
            if(mScanState == SWINGRIGHT){
                mFailScanCounter = 0;
                mPrevSonarDetectCount = mSonarDetectCount; //�E���̎��̌��m�񐔂�ۑ�
                mSonarDetectCount = 0;                     //�E���̎��̌��m�񐔂��N���A
                mTimeCounter = 0;
                mScanState = SWINGLEFT;
            }
            //
            else if(mScanState == SWINGLEFT){
                gDoSonar = false;
                mTimeCounter = 0;
                mTargetX = mTargetTotalX / (mSonarDetectCount + mPrevSonarDetectCount);
                mTargetY = mTargetTotalY / (mSonarDetectCount + mPrevSonarDetectCount);
                mInitState = true;
                mState = ETsumoDriver::NOKOTTA_GO;
            }
        }
        mAngleTrace.execute();
    }

    if (mState == ETsumoDriver::NOKOTTA_GO) {
        if (mInitState) {
            gDoSonar = false; //
            K_PHIDOT = 30.0F;
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//�^�[�Q�b�g�̃A���O����-180�`180�ŕԂ�
            mAngleTrace.setTargetAngle(mTargetAngle + 0);//���E���͂̈Ⴂ?�␳ ��todo�@�̈ˑ����v����
            mAngleTrace.setAllowableError(1.0); // 1�x
            mInitState = false;
            mIsArrived = false;
            mOshidashiFlag = false;
        }
        if (! mIsArrived) {
            mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//���x����̂��߁A���I�Ƀ^�[�Q�b�g�A���O�����X�V����
            mAngleTrace.setTargetAngle(mTargetAngle + 0);//���E���͂̈Ⴂ?�␳ ��todo�@�̈ˑ����v����
        }
            
        // �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        //������艟���o���A���X�����
        if(!mOshidashiFlag && mIsArrived && (mTimeCounter > 100)){
            mAngleTrace.setForward(10);//��todo�x�X�g�Ȓl��v����
        }
        //�����o������
        if((mGps.getXCoordinate() > (GPS_ETSUMO_SEARCH_X + 800)) || (mGps.getYCoordinate() < (GPS_ETSUMO_SEARCH_Y - 800))){//��todo�x�X�g�Ȓl��v����
            mAngleTrace.setForward(-10);//���肪�o������������
            mTimeCounter = 0;
            mOshidashiFlag = true;
            mLightSensor.setLamp(1);//���C�g�Z���TON
        }
        //������4�b�قǌ�ތ��ԑJ��
        if(mOshidashiFlag && (mTimeCounter > 1000)){    
            //mState = ETsumoDriver::KACHI_NANORI;
            //mInitState = true;
        }
        mAngleTrace.execute();
    }
    /*������������Ȃ��H���߃R�����g�A�E�g
    if (mState == ETsumoDriver::KACHI_NANORI) {
        if (mInitState) {
            gDoSonar = false;
            K_PHIDOT = 60.0F;
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(0, 0));
            mCoordinateTrace.setForward(30);
            mCoordinateTrace.setAllowableError(30);
            mInitState = false;
            mIsArrived = false;
        }
        // �ړ�����
        if (mCoordinateTrace.isArrived()) {
            mInitState = true;
            mState = ETsumoDriver::SPOTSEARCH_to_SWINGSEARCH;
        }
        mCoordinateTrace.execute();
    }
    */
    mTimeCounter++;
    return 0;
}

//���������y�b�g�{�g���̂����悻�̈ʒu���v�Z���AmTargetTotalX�AmTargetTotalY�ɉ�����֐�
void ETsumoDriver::updateTargetCoordinates()
{
    //
    float rad  = gSonarTagetAngle * 3.1416 / 180.0;
    float x = mGps.getXCoordinate() + gSonarTagetDistance * cosf(rad);
    float y = mGps.getYCoordinate() + gSonarTagetDistance * sinf(rad);
    
    //UPDATE
    mTargetTotalX += x;
    mTargetTotalY += y;

    //�ȉ��̌v�Z���ƁA�Ȃ������{��������
    //mTargetX = mTargetX * (c -1.0) / c + x / c;
    //mTargetY = mTargetY * (c -1.0) / c + y / c;
    
    return;
}

//CoordinateTrace���ؗp�A�A���O���݂̂��v�Z������
//�^�[�Q�b�g�̃A���O����-180�`180�ŕԂ�
float ETsumoDriver::calcTargetAngle(float targetX, float targetY)
{
        
    // ���ݍ��W�擾
    float currentX = mGps.getXCoordinate();
    float currentY = mGps.getYCoordinate();

    // ���ݍ��W�ƖڕW���W�Ƃ̍�
    float diffX = targetX - currentX;
    float diffY = targetY - currentY;

    // �ڕW�ւ̊p�x���Z�o
    float targetDirection;
    if (diffX == 0.0 && diffY == 0.0) {
        targetDirection = mGps.getDirection();
    }
    else {
        double radian = Gps::atan2(diffY, diffX);
        if (isinf(radian)) targetDirection = mGps.getDirection();
        else targetDirection = Gps::radianToDegree(radian);
    }
    Gps::marge180(targetDirection);
    
    return targetDirection;
}

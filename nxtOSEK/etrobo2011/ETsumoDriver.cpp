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
 * ���s����B�V�[�\�[���U������B
 *
 * @retval true �ŏI��ԁB����ȍ~�̏�ԑJ�ڂȂ��B
 * @retval false �܂���ڂ��������Ă��܂���B
 */
 

bool ETsumoDriver::drive()
{	
	/*
	K_PHIDOT = 15.0F;
	K_THETADOT = 10.0F;
	mAngleTrace.setForward(0);
	mAngleTrace.setTargetAngle(0.0);
	mAngleTrace.execute();
	return 0;
	*/
	
	if (mState == ETsumoDriver::INIT) { // ���������
    	gDoSonar = true;
        mTimeCounter = 0;
		K_THETADOT = 6.5F;
		mOrigK_PHIDOT = K_PHIDOT; // ��Ŗ߂����߂ɕۑ�
		K_PHIDOT = 25.0;
		mScanState = UNKNOWN;
		
		//��ԑJ��
        mInitState = true;
        //mState = ETsumoDriver::BEFORELINETRACE;
		mState = ETsumoDriver::PREPARE_SPOTSEARCH;//
		gDoSonar = false;
		//mRescanFlag = false;
		
		if(SUMO_DEBUG){//�T���v���R�[�X�̏���̈ʒu����ь������w��A������GPS�ɏ㏑��
			//mGps.adjustXCoordinate(500);
			//mGps.adjustYCoordinate(700);
			//mGps.adjustDirection(180);
		}
    }

	
	/*
	 // �܂��X�^�[�g�������C���g���[�X(�C���R�[�X�K���[�W�E�C����O�����ӂ肩����Ɨǂ�)
    if (mState == ETsumoDriver::BEFORELINETRACE) {
        if (mInitState) {
            mTimeCounter = 0;
            mLineTrace.setForward(30);
            mInitState = false;
        }

        // if (mSlowdownSkill.isArrived()) { 
        if (mTimeCounter > 2000) {//�e�X�g�p @todo: ���W ||�}�[�J�[���m �ɕς��邱��
            mState = ETsumoDriver::PREPARE_SPOTSEARCH;
            mInitState = true;
        }
        mLineTrace.execute();
    }
    */
    if (mState == ETsumoDriver::PREPARE_SPOTSEARCH) {
        if (mInitState) {
			gDoSonar = false;
        	K_THETADOT = 6.5F;
			K_PHIDOT = 62.5F;
	        mTimeCounter = 0;
			mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X, GPS_ETSUMO_SEARCH_Y));// ��todo�v�Đݒ�
			mCoordinateTrace.setForward(30);
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
	        K_PHIDOT = 62.5F;
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
				float setangle = mGps.getDirection() - 10;//���l�͒����s�v�̂͂�
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
			K_PHIDOT = 62.5F;
	        mTimeCounter = 0;
			mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X + 200.0, GPS_ETSUMO_SEARCH_Y - 200.0));// ��todo�v�Đݒ�
			mCoordinateTrace.setForward(30);
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
			K_PHIDOT = 30.0F;
	        mTimeCounter = 0;
			mSonarDetectCount = 0;
	        mAngleTrace.setForward(0);
	        mAngleTrace.setTargetAngle(90);
	        mAngleTrace.setAllowableError(2.0); // 2�x
	        mInitState = false;
	        mIsArrived = false;
			mTargetTotalX = 0;
			mTargetTotalY = 0;
        }
		// �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
        	K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            mTimeCounter = 0;
        	
        	gDoSonar = true; // �\�i�[�N��
        }
        // �����]���������Ă���X�C���O�T�[�`�J�n
		if(mIsArrived && (mTimeCounter % 20 == 0)){
			if(gSonarIsDetected){
				mSonarDetectCount++;
				mSonarTotalDistance += gSonarTagetDistance;
				if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
				updateTargetCoordinates();
			}
			else if(mTimeCounter % 100 == 0){
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
	
	if (mState == ETsumoDriver::SCAN) {
		if (mInitState) {
			gDoSonar = true; // �\�i�[�N��
			K_PHIDOT = K_PHIDOT_FOR_SEARCH;
	        mTimeCounter = 0;
			mSonarDetectCount = 0;
			mFailScanCounter = 0;
			mSonarTotalDistance = 0;
			mTargetTotalX = 0;
			mTargetTotalY = 0;
			
			mAngleTrace.setForward(0);
			mSonarTotalAngle = 0;
	        mInitState = false;
	        mIsArrived = false;
			mScanState = SWINGRIGHT;
			
			//mTargetTotalX = 0;
			//mTargetTotalY = 0;
		}
		
		if(mTimeCounter % 20 == 0){
			if(gSonarIsDetected){
				if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
				mFailScanCounter = 0; //���m�����猟�m���s�񐔂����Z�b�g
				mSonarDetectCount++;
				//if(mScanState == SWINGLEFT){
					mSonarTotalDistance += gSonarTagetDistance;
					mSonarTotalAngle += gSonarTagetAngle;
					mTargetDistance = (float)gSonarTagetDistance;
					mTargetAngle = gSonarTagetAngle;
					updateTargetCoordinates();
					//Speaker s;
					//s.playTone(1000, 1, 100);
				//}
			}
			else{
				mFailScanCounter++;
				if(mFailScanCounter > 5)
				{
					mFailScanCounter = 5;
				}
			}
			
			if((mTimeCounter % 100 == 0) && (mScanState == SWINGRIGHT)){
				float setangle = mGps.getDirection() - 10;
				mAngleTrace.setTargetAngle(setangle);
			}
			else if((mTimeCounter % 100 == 0) && (mScanState == SWINGLEFT)){
				float setangle = mGps.getDirection() + 10;
				mAngleTrace.setTargetAngle(setangle);
			}
		}
		if((mFailScanCounter >= 5) && (mSonarDetectCount >= 2)){
			if(mScanState == SWINGRIGHT){
				mFailScanCounter = 0;
				mPrevSonarDetectCount = mSonarDetectCount;
				mSonarDetectCount = 0;
				mTimeCounter = 0;
				mScanState = SWINGLEFT;
			}
			else if(mScanState == SWINGLEFT){
				
				gDoSonar = false;
				//mTargetDistance = (float)mSonarTotalDistance / mSonarDetectCount;
				//mTargetAngle = mSonarTotalAngle / mSonarDetectCount;
				//mScanState = CALC;
				//calcTargetCoordinates();
				if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
				mTimeCounter = 0;
				mTargetX = mTargetTotalX / (mSonarDetectCount + mPrevSonarDetectCount);
				mTargetY = mTargetTotalY / (mSonarDetectCount + mPrevSonarDetectCount);
				mInitState = true;
				mState = ETsumoDriver::NOKOTTA_GO;
/*
				mInitState = true;
				
				if(!mRescanFlag){
					mState = ETsumoDriver::DOHYO_IN;
					mRescanFlag = true;
				}
				else{
					mState = ETsumoDriver::NOKOTTA_GO;
				}
				*/	
			}
			/*
			else if((mScanState == CALC) && (mTimeCounter > 100) ){
				//calcTargetCoordinates();
				
				mInitState = true;
				if(!mRescanFlag){
					mState = ETsumoDriver::DOHYO_IN;
					mRescanFlag = true;
				}
				else{
					mState = ETsumoDriver::NOKOTTA_GO;
				}
			}
			*/
		}
		mAngleTrace.execute();
	}


	if (mState == ETsumoDriver::DOHYO_IN) {
		if (mInitState) {
	        mTimeCounter = 0;
			K_PHIDOT = 62.5F;
			mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//�^�[�Q�b�g�̃A���O����-180�`180�ŕԂ�
			if((mTargetAngle > -45) && (mTargetAngle > 135)){
				mCoordinateTrace.setTargetCoordinate(MakePoint(mTargetX - 300, mTargetY));
			}
			else{
				mCoordinateTrace.setTargetCoordinate(MakePoint(mTargetX, mTargetY + 300));
			}
			mCoordinateTrace.setForward(10);
			mCoordinateTrace.setAllowableError(30);
	        mInitState = false;
	        mIsArrived = false;
	        gDoSonar = false; // �\�i�[�N���O�͈�U���Z�b�g
        }
		// �ړ�����
        if (! mIsArrived && mCoordinateTrace.isArrived()) {
        	mState = ETsumoDriver::HAKKE_READY;
			mInitState = true;
        }
		mCoordinateTrace.execute();
	}
	
	
	if (mState == ETsumoDriver::HAKKE_READY) {
		if (mInitState) {
	        gDoSonar = false; // �\�i�[�N��
	        mTimeCounter = 0;
			mSonarTotalDistance = 0;
	        mAngleTrace.setForward(0);
			K_PHIDOT = K_PHIDOT_FOR_SEARCH;
			if((mTargetAngle > -45) && (mTargetAngle > 225)){
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
        // �����]���������Ă���X�C���O�T�[�`�J�n
		if(mIsArrived && (mTimeCounter > 1000)){
			mInitState = true;
			mState = ETsumoDriver::SCAN;
		}
		
		mAngleTrace.execute();
    }
	
	if (mState == ETsumoDriver::NOKOTTA_GO) {
		if (mInitState) {
	        gDoSonar = true; // �\�i�[�N��
			K_PHIDOT = 20.0;
	        mTimeCounter = 0;
			mSonarTotalDistance = 0;
	        mAngleTrace.setForward(0);
			mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//�^�[�Q�b�g�̃A���O����-180�`180�ŕԂ�
	        mAngleTrace.setTargetAngle(mTargetAngle + 0);//���E���͂̈Ⴂ?�␳ ��todo�@�̈ˑ����v����
	        mAngleTrace.setAllowableError(1.0); // 1�x
	        mInitState = false;
	        mIsArrived = false;
			mOshidashiFlag = false;
        }
		if (! mIsArrived) {
			mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//���x����̂��߁A���I�Ƀ^�[�Q�b�g���X�V����
	        mAngleTrace.setTargetAngle(mTargetAngle + 0);//���E���͂̈Ⴂ?�␳ ��todo�@�̈ˑ����v����
		}
			
		// �����]������
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
		if(!mOshidashiFlag && mIsArrived && (mTimeCounter > 100)){
			mAngleTrace.setForward(10);
		}
		//�����o������
		if((mGps.getXCoordinate() > (GPS_ETSUMO_SEARCH_X + 800)) || (mGps.getYCoordinate() < (GPS_ETSUMO_SEARCH_Y - 800))){
			mAngleTrace.setForward(-10);
			mTimeCounter = 0;
			mOshidashiFlag = true;
		}
		//������4�b�قǌ��
		if(mOshidashiFlag && (mTimeCounter > 1000)){	
			//mState = ETsumoDriver::KACHI_NANORI;
			//mInitState = true;
		}
		mAngleTrace.execute();
    }
    /*
    if (mState == ETsumoDriver::KACHI_NANORI) {
		if (mInitState) {
			gDoSonar = false;
			K_PHIDOT = 62.5F;
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

//���������y�b�g�{�g���̂����悻�̈ʒu���v�Z���AmTargetX�AmTargetY�ɑ������֐�
	/*
void ETsumoDriver::calcTargetCoordinates()
{
	float x = mGps.getXCoordinate();
	float y = mGps.getYCoordinate();
	mTargetAngle = Gps::marge180(mTargetAngle);
	float rad  = mTargetAngle * 3.14 / 180.0;
	
	rad = Gps::marge180(rad);
	
	mTargetX = x + mTargetDistance * cosf(rad);//cos���g���Ƃ��܂Ƀ��{��������A�߂�l���_�u���̂��߁H�H
	mTargetY = y + mTargetDistance * sinf(rad);//�����������l
}
*/
//���������y�b�g�{�g���̂����悻�̈ʒu���v�Z���AmTargetX�AmTargetY�ɑ������֐�
void ETsumoDriver::updateTargetCoordinates()
{
	//mTargetAngle = Gps::marge180(mTargetAngle);
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
/*
void ETsumoDriver::swingTimer(int time_interval, float direction_interval)
{
	if(mTimeCounter % time_interval == 0){
		float setangle = mGps.getDirection() + direction_interval;
		mAngleTrace.setTargetAngle(setangle);
	}
}
*/
/*
bool ETsumoDriver::searchTimer(float time_intervalTime)
{
	if((mTimeCounter % time_intervalTime == 0) && gSonarIsDetected){
    	return true;
	}
	return false;
}
*/

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

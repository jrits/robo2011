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

extern bool gDoSonar; //!< ソナーセンサ発動フラグ
extern bool gSonarIsDetected; //!< 衝立検知の結果
extern int  gSonarTagetDistance;//!< ソナーセンサの距離結果
extern float gSonarTagetAngle;


/**
 * コンストラクタ
 */
ETsumoDriver::ETsumoDriver()    //初期値の設定
{
	mTimeCounter = 0;
	mInitState = false;             
	mState = ETsumoDriver::INIT;  //ステート
}

/**
 * デストラクタ
 */
ETsumoDriver::~ETsumoDriver(){}

/**
 * 走行する。シーソーを攻略する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
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
	
	if (mState == ETsumoDriver::INIT) { // 初期化状態
    	gDoSonar = true;
        mTimeCounter = 0;
		K_THETADOT = 6.5F;
		mOrigK_PHIDOT = K_PHIDOT; // 後で戻すために保存
		K_PHIDOT = 25.0;
		mScanState = UNKNOWN;
		
		//状態遷移
        mInitState = true;
        //mState = ETsumoDriver::BEFORELINETRACE;
		mState = ETsumoDriver::PREPARE_SPOTSEARCH;//
		gDoSonar = false;
		//mRescanFlag = false;
		
		if(SUMO_DEBUG){//サンプルコースの所定の位置および向きを指定、強引にGPSに上書き
			//mGps.adjustXCoordinate(500);
			//mGps.adjustYCoordinate(700);
			//mGps.adjustDirection(180);
		}
    }

	
	/*
	 // まずスタート直線ライントレース(インコースガレージ・イン手前直線辺りからやると良い)
    if (mState == ETsumoDriver::BEFORELINETRACE) {
        if (mInitState) {
            mTimeCounter = 0;
            mLineTrace.setForward(30);
            mInitState = false;
        }

        // if (mSlowdownSkill.isArrived()) { 
        if (mTimeCounter > 2000) {//テスト用 @todo: 座標 ||マーカー検知 に変えること
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
			mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X, GPS_ETSUMO_SEARCH_Y));// ＠todo要再設定
			mCoordinateTrace.setForward(30);
			mCoordinateTrace.setAllowableError(30);
	        mInitState = false;
	        mIsArrived = false;
        }
		// 移動完了
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
	        mAngleTrace.setAllowableError(2.0); // 2度
	        mInitState = false;
	        mIsArrived = false;
			mSonarDetectCount = 0;
			mTargetTotalX = 0;
			mTargetTotalY = 0;
        }
		// 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
        	K_PHIDOT = K_PHIDOT_FOR_SEARCH;
        	gDoSonar = true;
            mIsArrived = true;
        }
        // 方向転換完了してからスポットサーチ開始
		if(mIsArrived && (mTimeCounter % 20 == 0) && (mTimeCounter >= 1000)){
			if(gSonarIsDetected){
				mSonarDetectCount++;
				updateTargetCoordinates();
				if(SUMO_DEBUG) {mSpeaker.playTone(1000, 1, 10);}
			}
			else if(mTimeCounter % 100 == 0){
				float setangle = mGps.getDirection() - 10;//数値は調整不要のはず
				mAngleTrace.setTargetAngle(setangle);
			}
		}
		
		//スポットサーチ範囲内で探知成功！
		if(mSonarDetectCount >= 3){
			mInitState = true;
			mTargetX = mTargetTotalX / mSonarDetectCount;
			mTargetY = mTargetTotalY / mSonarDetectCount;
			mState = ETsumoDriver::DOHYO_IN;
		}
		//スポットサーチ範囲内にペットボトルが無いことを確認、スイングサーチへ移行
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
			mCoordinateTrace.setTargetCoordinate(MakePoint(GPS_ETSUMO_SEARCH_X + 200.0, GPS_ETSUMO_SEARCH_Y - 200.0));// ＠todo要再設定
			mCoordinateTrace.setForward(30);
			mCoordinateTrace.setAllowableError(30);
	        mInitState = false;
	        mIsArrived = false;
        }
		// 移動完了
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
	        mAngleTrace.setAllowableError(2.0); // 2度
	        mInitState = false;
	        mIsArrived = false;
			mTargetTotalX = 0;
			mTargetTotalY = 0;
        }
		// 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
        	K_PHIDOT = K_PHIDOT_FOR_SEARCH;
            mTimeCounter = 0;
        	
        	gDoSonar = true; // ソナー起動
        }
        // 方向転換完了してからスイングサーチ開始
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
		
		//スイングサーチ範囲内で探知成功！
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
			gDoSonar = true; // ソナー起動
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
				mFailScanCounter = 0; //検知したら検知失敗回数をリセット
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
			mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//ターゲットのアングルを-180～180で返す
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
	        gDoSonar = false; // ソナー起動前は一旦リセット
        }
		// 移動完了
        if (! mIsArrived && mCoordinateTrace.isArrived()) {
        	mState = ETsumoDriver::HAKKE_READY;
			mInitState = true;
        }
		mCoordinateTrace.execute();
	}
	
	
	if (mState == ETsumoDriver::HAKKE_READY) {
		if (mInitState) {
	        gDoSonar = false; // ソナー起動
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
	        mAngleTrace.setAllowableError(2.0); // 2度
	        mInitState = false;
	        mIsArrived = false;
        }
		// 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        // 方向転換完了してからスイングサーチ開始
		if(mIsArrived && (mTimeCounter > 1000)){
			mInitState = true;
			mState = ETsumoDriver::SCAN;
		}
		
		mAngleTrace.execute();
    }
	
	if (mState == ETsumoDriver::NOKOTTA_GO) {
		if (mInitState) {
	        gDoSonar = true; // ソナー起動
			K_PHIDOT = 20.0;
	        mTimeCounter = 0;
			mSonarTotalDistance = 0;
	        mAngleTrace.setForward(0);
			mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//ターゲットのアングルを-180～180で返す
	        mAngleTrace.setTargetAngle(mTargetAngle + 0);//左右視力の違い?補正 ＠todo機体依存か要調査
	        mAngleTrace.setAllowableError(1.0); // 1度
	        mInitState = false;
	        mIsArrived = false;
			mOshidashiFlag = false;
        }
		if (! mIsArrived) {
			mTargetAngle = calcTargetAngle(mTargetX, mTargetY);//精度向上のため、動的にターゲットを更新する
	        mAngleTrace.setTargetAngle(mTargetAngle + 0);//左右視力の違い?補正 ＠todo機体依存か要調査
		}
			
		// 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
		if(!mOshidashiFlag && mIsArrived && (mTimeCounter > 100)){
			mAngleTrace.setForward(10);
		}
		//押し出し判定
		if((mGps.getXCoordinate() > (GPS_ETSUMO_SEARCH_X + 800)) || (mGps.getYCoordinate() < (GPS_ETSUMO_SEARCH_Y - 800))){
			mAngleTrace.setForward(-10);
			mTimeCounter = 0;
			mOshidashiFlag = true;
		}
		//そっと4秒ほど後退
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
		// 移動完了
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

//発見したペットボトルのおおよその位置を計算し、mTargetX、mTargetYに代入する関数
	/*
void ETsumoDriver::calcTargetCoordinates()
{
	float x = mGps.getXCoordinate();
	float y = mGps.getYCoordinate();
	mTargetAngle = Gps::marge180(mTargetAngle);
	float rad  = mTargetAngle * 3.14 / 180.0;
	
	rad = Gps::marge180(rad);
	
	mTargetX = x + mTargetDistance * cosf(rad);//cosを使うとたまにロボがこける、戻り値がダブルのため？？
	mTargetY = y + mTargetDistance * sinf(rad);//こっちも同様
}
*/
//発見したペットボトルのおおよその位置を計算し、mTargetX、mTargetYに代入する関数
void ETsumoDriver::updateTargetCoordinates()
{
	//mTargetAngle = Gps::marge180(mTargetAngle);
	float rad  = gSonarTagetAngle * 3.1416 / 180.0;
	
	float x = mGps.getXCoordinate() + gSonarTagetDistance * cosf(rad);
	float y = mGps.getYCoordinate() + gSonarTagetDistance * sinf(rad);
	
	//UPDATE
	mTargetTotalX += x;
	mTargetTotalY += y;

	//以下の計算だと、なぜかロボがこける
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

//CoordinateTraceより借用、アングルのみを計算させる
//ターゲットのアングルを-180～180で返す
float ETsumoDriver::calcTargetAngle(float targetX, float targetY)
{
		
    // 現在座標取得
    float currentX = mGps.getXCoordinate();
    float currentY = mGps.getYCoordinate();

    // 現在座標と目標座標との差
    float diffX = targetX - currentX;
    float diffY = targetY - currentY;

    // 目標への角度を算出
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

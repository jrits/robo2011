//
// OutGarageDriver.cpp
//

#include "OutGarageDriver.h"
#include "factory.h"
#include "constants.h"

/**
 * コンストラクタ
 */
OutGarageDriver::OutGarageDriver()
{
    mState = OutGarageDriver::INIT;
    //K_THETADOTをゆるくする（坂道で転倒したため）
    //K_THETADOT = 7.0F;
    mGrayThroughFlag = false;
}

/**
 * アウトコースガレージ・イン区間を攻略する
 *
 * 坂道直線ライントレースから、ガレージ・インまでを担当する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool OutGarageDriver::drive()
{
#if 0 // ログ送信(0：解除、1：実施)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
	LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
	LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
//	LOGGER_DATAS32[3] = (S32)(mGps.getDistance());

#endif
#if 1 // デバッグ(0：解除、1：実施)
    //DESK_DEBUG = true; // モータを回さないデバッグ
    static int count = 0; // staticは基本禁止だが、デバッグなので
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "OutGarageDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    if (mState == OutGarageDriver::INIT) { // 初期化状態
        mTimeCounter = 0;
        K_THETADOT = 7.0F;
    	mState = OutGarageDriver::BEFORELINETRACE;
        mLineTrace.setForward(100);
        mAngleTrace.setForward(100);
        mGrayThroughFlag = false;
     }
    // ライントレース(階段後直線、緑の端から)
    if (mState == OutGarageDriver::BEFORELINETRACE) {
        if(! mGrayThroughFlag && mGps.getXCoordinate() < 4323) { // 角度トレース準備
            mGrayThroughFlag = true;
            mAngleTrace.setTargetAngle(mGps.getDirection());
        }
        if (mGrayThroughFlag && mGps.getXCoordinate() < 4014) { // 角度トレース解除
            mGrayThroughFlag = false;
        }
        if (mGrayThroughFlag) { // 角度トレース実施
            mAngleTrace.execute();
        }
        else {  // ライントレース実施
            mLineTrace.execute();
        }
        // 直線検知がどこで発動するかわからないのがまずい？？？
        // 試しに時間で判断。(11/30)
		if (mTimeCounter > 250 && mStraightDetector.detect() == true) {
			mState = OutGarageDriver::STRAIGHTLINETRACE;
	    	mSlowdownSkill.setSkill(&mLineTrace);
		    // 【ライントレース（坂道手前～ゴール地点）の走行距離：ここを変える！！】
    		mSlowdownSkill.setTargetDistance(4300);
    		mSlowdownSkill.setMinimumForward(100);
		}
        // if (mGps.getXCoordinate() < ?????) {
        //     mState = OutGarageDriver::GARAGEIN;
        //     mTimeCounter = 0;
        // }
    }
	
// 座標指定走行用ソース↓
/*
	else if(mState == OutGarageDriver::STRAIGHTLINETRACE) {
		mSlowdownSkill.execute();
		if (mSlowdownSkill.isArrived()) {

		    mState = OutGarageDriver::ENTERGARAGE;
			//目標座標の設定
		    //K_THETAをもっときつく
	        //K_THETADOT = 5.0F;
			mCoordinateTrace.setTargetCoordinate(MakePoint((GPS_GARAGE_X + 50), (GPS_GARAGE_Y + 50) ));
			mCoordinateTrace.setForward(100);
			mCoordinateTrace.setAllowableError(50); //5cm    
		}
	}
*/
// 座標指定走行用ソース↑ 

	
// 角度トレース版ソース↓	
	// ライントレース後の向き回転
    else if (mState == OutGarageDriver::STRAIGHTLINETRACE) {
        if(! mGrayThroughFlag && 4014 < mGps.getXCoordinate() && mGps.getXCoordinate() < 4323) { // 角度トレース準備
            mGrayThroughFlag = true;
            mAngleTrace.setTargetAngle(mGps.getDirection());
        }
        if (mGrayThroughFlag && mGps.getXCoordinate() < 4014) { // 角度トレース解除
            mGrayThroughFlag = false;
        }
        if (mGrayThroughFlag) { // 角度トレース実施
            mAngleTrace.execute();
        }
        else { // スローダウンスキル実施
            mSlowdownSkill.execute();
        }
		
		if (mSlowdownSkill.isArrived()) {
			mState = OutGarageDriver::PREPAREENTERGARAGE;
			//mAngleTrace.setTargetAngle(90);
			//mAngleTrace.setForward(0);
		    //SORA【ガレージに向かう角度：ここを変える･･･必要はあったりなかったり】
		    mAngleTrace.setTargetAngle(175 - 7);
			mAngleTrace.setForward(100);
   			mSlowdownSkill.setSkill(&mAngleTrace);
		    //SORA【ガレージに向かうときの距離：ここを変える！！】
			mSlowdownSkill.setTargetDistance(1200 + 200);
			mSlowdownSkill.setMinimumForward(0);

    	}
    }
	
	//向きが移転後、車庫と同じY座標へ前進
	else if (mState == OutGarageDriver::PREPAREENTERGARAGE) {   //ガレージイン準備
		//mAngleTrace.execute();
		//if (mAngleTrace.isArrived()) {
		//	mState = OutGarageDriver::STRAIGHTANGLETRACE;
		//	mAngleTrace.setForward(50);
		//	mSlowdownSkill.setSkill(&mAngleTrace);
		//	mSlowdownSkill.setTargetDistance(5);
		//	mSlowdownSkill.setMinimumForward(0);
		//}
		    
	    mSlowdownSkill.execute();
		    if(mSlowdownSkill.isArrived()) {
		        mState = OutGarageDriver::FIN_GARAGEIN;
		        mAngleTrace.setTargetAngle(180);
	    		mAngleTrace.setForward(0);        
		    }
	    		
	}
/* pass
    // 向きを車庫に向ける
	else if (mState == OutGarageDriver::STRAIGHTANGLETRACE) {
		mSlowdownSkill.execute();
		if (mSlowdownSkill.isArrived()) {
			mState = OutGarageDriver::GOGARAGE;
			mAngleTrace.setTargetAngle(180);
			mAngleTrace.setForward(0);
		}
	}
	// 車庫入れ動作
	else if (mState == OutGarageDriver::GOGARAGE) {
		mAngleTrace.execute();
		if (mAngleTrace.isArrived()) {
			mState = OutGarageDriver::ENTERGARAGE;
			mAngleTrace.setForward(50);
			mSlowdownSkill.setSkill(&mAngleTrace);
			mSlowdownSkill.setTargetDistance(500);
			mSlowdownSkill.setMinimumForward(0);
		}
	}
    
pass */
    // 車庫入れ完了
    else if (mState == OutGarageDriver::FIN_GARAGEIN) {
        mAngleTrace.execute();
    
    }
    mTimeCounter++;
    return false; // 終了しない(最後なのでなんでも良い)
}
    
    
// 角度トレース版実装終了↑    

    
// 座標指定走行↓ 
/*    

    //車庫入れ完了
	else if (mState == OutGarageDriver::ENTERGARAGE) {
		mCoordinateTrace.execute();
	    if(mCoordinateTrace.isArrived()) {
	        mState = OutGarageDriver::FIN_GARAGEIN;
	        
			mAngleTrace.setForward(0);
		    mAngleTrace.setTargetAngle(180);
		}
	}
	
    else if (mState == OutGarageDriver::FIN_GARAGEIN) {
        mAngleTrace.execute();
    
    }
	//else if (mState == OutGarageDriver::GARAGEIN) { 
    //}
	
    mTimeCounter++;
    return false; // 終了しない(最後なのでなんでも良い)
}

// 座標指定走行 ↑
*/


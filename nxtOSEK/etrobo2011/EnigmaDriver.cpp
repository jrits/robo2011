//
// EnigmaDriver.cpp
//

#include "EnigmaDriver.h"
#include "factory.h"
#include "constants.h"
#include "Speaker.h"

#define DETECT_THRESH (25)
#define MAX_DISTANCE (255)
#define DETECT_RANGE (20) // 検知に若干余裕をもたせる
#define ENIGMA_SAMPLECOUNT 100 // *4ms
extern bool gDoSonar; //!< ソナーセンサ発動フラグ
extern bool gSonarIsDetected; //!< 衝立検知の結果

/**
 * コンストラクタ
 */
EnigmaDriver::EnigmaDriver()
{
    mState = EnigmaDriver::INIT;
    mObstacleFlag[0] = false;
    mObstacleFlag[1] = false;
}

/**
 * エニグマ・デコーディング区間を攻略する
 *
 * スタート直線ライントレースから、ライン復帰後ライントレースまでを担当する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool EnigmaDriver::drive()
{
#if 0 // ログ送信
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS32[0] = (S32)(mGps.getXCoordinate());
	LOGGER_DATAS32[1] = (S32)(mGps.getYCoordinate());
	LOGGER_DATAS32[2] = (S32)(mGps.getDirection());
	LOGGER_DATAS32[3] = (S32)(mGps.getDistance());
#endif
#if 1 // DEBUG
    //DESK_DEBUG = true; // モータを回さないデバグ
    if (mTimeCounter % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "EnigmaDriver");
        lcd.putf("dn", mState);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.disp();
    }
#endif
    if (mState == EnigmaDriver::INIT) { // 初期化状態
    	gDoSonar = true;
        mTimeCounter = 0;
        mInitState = true;
        mState = EnigmaDriver::BEFORELINETRACE;
        // 自宅テスト用。ちょっとだけライントレースしてからENTERPET
        // mGps.adjustXCoordinate(430);
        // mGps.adjustYCoordinate(-1200);
        // mGps.adjustDirection(270);
        // 自宅テスト用。ENTERPET から
        // mState = EnigmaDriver::ENTERPET;
        // mGps.adjustXCoordinate(430);
        // mGps.adjustYCoordinate(-1500);
        // mGps.adjustDirection(360);
    }
    // まずスタート直線ライントレース(インコースガレージ・イン手前直線辺りからやると良い)
    if (mState == EnigmaDriver::BEFORELINETRACE) {
        if (mInitState) {
            mTimeCounter = 0;
            mLineTrace.setForward(100);
            mSlowdownSkill.setSkill(&mLineTrace);
            //mSlowdownSkill.setAllowableError(10); // 1cm
            mInitState = false;
        }
        mSlowdownSkill.setTargetDistance(mGps.calcDistanceTo(MakePoint(460,-1515+200)));  // @todo: 絶妙なy座標を指定
        mSlowdownSkill.execute();
        // ラインしか走れないため永遠に辿りつかない場合がある
        // if (mSlowdownSkill.isArrived()) { 
        if (mGps.getYCoordinate() < -1515+200) { // 愚直に座標で // @todo: 絶妙なy座標を指定
            mState = EnigmaDriver::PREPAREENTERPET;
            mInitState = true;
        }
    }
    // 侵入準備として0度方向を向く
    else if (mState == EnigmaDriver::PREPAREENTERPET) {
        if (mInitState) {
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(0);
            mAngleTrace.setAllowableError(2.0); // 2度
            mInitState = false;
        }
        mAngleTrace.execute();
        if (mAngleTrace.isArrived()) {
            //目標座標に到着したら次の状態へ遷移
            mState = EnigmaDriver::ENTERPET;
            mInitState = true;
        }
    }
    // ペットボトルの間に侵入
    else if (mState == EnigmaDriver::ENTERPET) {
        if (mInitState) {
            mTimeCounter = 0;
            mCoordinateTrace.setForward(30);
            mSlowdownSkill.setSkill(&mCoordinateTrace);
            // @todo: 絶妙なy座標を指定
            mCoordinateTrace.setTargetCoordinate(MakePoint(600, -1515+50)); 
            mSlowdownSkill.setTargetDistance(mGps.calcDistanceTo(MakePoint(600, -1515+50)));
            mSlowdownSkill.setAllowableError(10); // 1cm
            mInitState = false;
        }
        mSlowdownSkill.execute();
        if (mSlowdownSkill.isArrived()) {
            mState = EnigmaDriver::LEFTPET;
            mInitState = true;
        }
    }
    //1本目判定
    else if (mState == EnigmaDriver::LEFTPET) {
        if (mInitState) {
            gDoSonar = true; // ソナー起動
            mOrigK_PHIDOT = K_PHIDOT; // 後で戻すために保存
            K_PHIDOT = 62.5F;

            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(90);
            mAngleTrace.setAllowableError(2.0); // 2度
            mInitState = false;
            mIsArrived = false;
            gDoSonar = false; // ソナー起動前は一旦リセット
        }
        mAngleTrace.execute();
        // 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        // 方向転換完了してから衝立検知。
        if (mIsArrived) {
            gDoSonar = true; // ソナー起動
        }
        // 方向転換してから時間経過で終了
        if (mIsArrived && mTimeCounter >= ENIGMA_SAMPLECOUNT) {
            mState = EnigmaDriver::RIGHTPET;
            mInitState = true;
            mTimeCounter = 0;
            mObstacleFlag[0] =  gSonarIsDetected;
            gDoSonar = false; // ソナー停止      
            if (mObstacleFlag[0]) { Speaker s; s.playTone(1000, 1, 100); }
        }
    }
    //2本目判定
    else if(mState == EnigmaDriver::RIGHTPET) {
        if (mInitState) {
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(270);
            mAngleTrace.setAllowableError(2.0); // 2度
            mInitState = false;
            mIsArrived = false;
        	gDoSonar = true; // ソナー起動
        }
        mAngleTrace.execute();
        // 方向転換完了
        if (! mIsArrived && mAngleTrace.isArrived()) {
            mIsArrived = true;
            mTimeCounter = 0;
        }
        // 方向転換完了してから衝立検知。
        if (mIsArrived) {
            gDoSonar = true; // ソナー起動
        }
        // 方向転換してから時間経過で終了
        if (mIsArrived && mTimeCounter >= ENIGMA_SAMPLECOUNT) {
            mState = EnigmaDriver::EXITPET;
            mInitState = true;
            mTimeCounter = 0;
            mObstacleFlag[1] =  gSonarIsDetected;
            gDoSonar = false; // ソナー停止      
            if (mObstacleFlag[1]) { Speaker s; s.playTone(1000, 1, 100); }
        }
    }
    // 脱出のため20度を向く
    else if (mState == EnigmaDriver::EXITPET) { 
        if (mInitState) {
            mTimeCounter = 0;
            mAngleTrace.setForward(0);
            mAngleTrace.setTargetAngle(20);
            mAngleTrace.setAllowableError(2.0); // 2度
            mInitState = false;
        }
        mAngleTrace.execute();
        if (mAngleTrace.isArrived()) {
        	mGps.adjustDirection(Gps::marge360(mGps.getDirection()));        	
            mState = EnigmaDriver::LINERETURN;
            K_PHIDOT = mOrigK_PHIDOT;
            mInitState = true;
        }
    }
    //ラインに向かって斜めに突進。ライン復帰できるか！？
    else if (mState == EnigmaDriver::LINERETURN) { 
        if (mInitState) {
            mTimeCounter = 0;
            mCoordinateTrace.setTargetCoordinate(MakePoint(1500, -1100));
            mCoordinateTrace.setForward(30);
            mCoordinateTrace.setAllowableError(10); // 1cm
            mInitState = false;
        }
        mCoordinateTrace.execute();
        if (mLineDetector.detect() != LineDetector::ON_WHITE) { // ライン検知
            mState = EnigmaDriver::AFTERLINETRACE;
            mInitState = true;
        }
        // else if (mCoordinateTrace.isArrived()) {
        //     // ラインを検出していないのに辿りついてしまった場合。
        //     // もう少しさまようとか？
        //     mState = EnigmaDriver::LINERETURN2;
        // }
    }
    else if (mState == EnigmaDriver::AFTERLINETRACE) {
        if (mInitState) {
            mLineTrace.setForward(30);
            mTimeCounter = 0;
            mInitState = false;
        }
        mLineTrace.execute();
        if (mTimeCounter > 250) {
            mLineTrace.setForward(60);
        }
        if (mTimeCounter > 500) {
            mLineTrace.setForward(100);
        }
    }
    mTimeCounter++;
    return mState == EnigmaDriver::AFTERLINETRACE; // 終了しました
}

/**
 * 衝立を検知する
 *
 * @retval true 検知した
 * @retval false 検知しなかった
 */
bool EnigmaDriver::detect()
{
    //return (10 < gSonarDistance) && (gSonarDistance < 45);
    return false;
}

/**
 * エニグマ・デコ＝ディングをする
 *
 * MysteryDriver が見に来る。
 *
 * @return ルートID
 */
int EnigmaDriver::decode()
{
    if(mObstacleFlag[0] == false && mObstacleFlag[1] == false) {
        //右、右
        return 0;
    }
    else if(mObstacleFlag[0] == false && mObstacleFlag[1] == true) {
        //右、左
        return 1;
    }
    else if(mObstacleFlag[0] == true && mObstacleFlag[1] == false) {
        //左、右
        return 2;
    }
    else if(mObstacleFlag[0] == true && mObstacleFlag[1] == true) {
        //左、左
        return 3;
    }
    return -1;
}

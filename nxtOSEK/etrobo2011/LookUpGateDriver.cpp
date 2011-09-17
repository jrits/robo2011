#include "LookUpGateDriver.h"
#include "factory.h"
extern bool gDoSonar;
extern bool gSonarIsDetected; //!< 衝立検知の結果

LookUpGateDriver::LookUpGateDriver()
    : mCurrentSubSection(INIT)
{
    mTimeCounter = 0;
}

LookUpGateDriver::~LookUpGateDriver()
{
}

bool LookUpGateDriver::drive()
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
        lcd.putf("sn", "LookUpGateDriver");
        lcd.putf("dn", mCurrentSubSection);
        lcd.putf("dn", (S32)(mGps.getXCoordinate()));
        lcd.putf("dn", (S32)(mGps.getYCoordinate()));
        lcd.putf("dn", (S32)(mGps.getDirection()));
        lcd.putf("dn", (S32)(mGps.getDistance()));
        lcd.putf("dn", (S32)(K_THETADOT*10));
        lcd.disp();
    }
#endif

    // 初期化関数を作るのがめんどうなのでとりあえずここで
    if (mCurrentSubSection == INIT) {
        gDoSonar = false;
        gDoMaimai = true;
        gDoForwardPid = false;
        mCurrentSubSection = BEFORELINETRACE;
    }
    switch(mCurrentSubSection){
    case BEFORELINETRACE:
        // ベーシックコースから引き続きライントレース
        mLineTrace.setForward(100);
        mLineTrace.execute();
        if(isDoFindGate()){
            gDoSonar = true;
            gDoMaimai = true;
            { Speaker s; s.playTone(1976, 10, 100); }
            mCurrentSubSection = IN_FRONT_OF_GATE;
            K_THETADOT = 7.5F;
        }
        break;
    case IN_FRONT_OF_GATE:
        // 座り込む。
        mSitDownSkill.setAngle(70);
        mSitDownSkill.execute();
        if (isSitDowned()) {
            { Speaker s; s.playTone(1976, 10, 100); }
            mCurrentSubSection = UNDER_GATE;
        }
        break;
    case UNDER_GATE:
        // 3点傾立走行。
        mTripodAngleTrace.setForward(50);
        mTripodAngleTrace.setTargetAngle(360);
        mTripodAngleTrace.execute();
        if (isGateFound()) {
            // 座標補正(ET相撲からのオーダー)
            // mGps.adjustXCoordinate();
            // mGps.adjustYCoordinate();
        }
        if(isGatePassed()){
            { Speaker s; s.playTone(1976, 10, 100); }
            mCurrentSubSection = BEHIND_GATE;
        }
        break;
    case BEHIND_GATE:
        mStandUpSkill.execute();
        /*立ち上がる。*/
        if(isStandUped()){
            { Speaker s; s.playTone(1976, 10, 100); }
            mCurrentSubSection = DONE;
        }
        break;
    case DONE:
        mAngleTrace.setForward(50);
        mAngleTrace.setTargetAngle(360);
        mAngleTrace.execute();
        break;
    default:
        // assertかなにかするべき
        break;
    }
    
    mTimeCounter++;
    return isDone();
}

bool LookUpGateDriver::isDoFindGate()
{
    // ベーシックステージチェックポイント超えてすぐ
    return mGps.getXCoordinate() > 3000.0;
}

bool LookUpGateDriver::isGatePassed()
{
    // ルックアップゲートチェックポイント超えた辺り
    return mGps.getXCoordinate() > 3600.0;
}

bool LookUpGateDriver::isGateFound()
{
	if(gSonarIsDetected){
		mSpeaker.playTone(1000, 1, 100);
		return 1;
	}
	else{
		return 0;
	}
}

bool LookUpGateDriver::isSitDowned()
{
    static int count = 0;
    count++;

    // 時間でもチェックして落ち着かせる
    return mSitDownSkill.isSeated() && count >= 500;
}

bool LookUpGateDriver::isStandUped()
{
    static int count = 0;
    count++;

    // 時間でもチェックして落ち着かせる
    return mStandUpSkill.isStandUp() && count >= 500;
}

bool LookUpGateDriver::isDone()
{
    return mCurrentSubSection == DONE;
}

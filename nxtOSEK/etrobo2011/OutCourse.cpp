//
// OutCourse.cpp
//
#include "OutCourse.h"
#include "factory.h"

/**
 * コンストラクタ
 *
 * @param[in] aSection どのセクションから始めるか。デフォルトはスタート地点から。
 */
OutCourse::OutCourse(OutCourse::eSection aSection)
    : Course()
{
    mState = aSection;
    switch (mState) {
    case OutCourse::SEESAW:
        // シーソーからテスト
        // インコースガレージ・イン手前線
        mGps.adjustXCoordinate(GPS_SEESAW_START_X);
        mGps.adjustYCoordinate(GPS_SEESAW_START_Y);
        mGps.adjustDirection(GPS_SEESAW_START_DIRECTION);
        break;
    case OutCourse::STAIRWAY:
        // ミステリーサークルからテスト
        // インコース、アウトコースシーソー後マーカー終わり地点
        mGps.adjustXCoordinate(GPS_STAIRWAY_START_X);
        mGps.adjustYCoordinate(GPS_STAIRWAY_START_Y);
        mGps.adjustDirection(GPS_STAIRWAY_START_DIRECTION);
        break;
    case OutCourse::ETSUMO:
        /* ETロボコン2011 追記*/
    	// ET相撲からテスト
        // ET相撲スタート地点
        mGps.adjustXCoordinate(GPS_ETSUMO_START_X);
        mGps.adjustYCoordinate(GPS_ETSUMO_START_Y);
        mGps.adjustDirection(GPS_ETSUMO_START_DIRECTION);
        break;
        /* ETロボコン2011 追記ここまで */
    case OutCourse::GARAGEIN:
        // ガレージインからテスト
        // インコース坂道前マーカー終わり地点
        mGps.adjustXCoordinate(GPS_GARAGEIN_START_X);
        mGps.adjustYCoordinate(GPS_GARAGEIN_START_Y);
        mGps.adjustDirection(GPS_GARAGEIN_START_DIRECTION);
        break;
    case OutCourse::START:
    default:
        // スタート位置からテスト
        mGps.adjustXCoordinate(GPS_COURSE_START_X);
        mGps.adjustYCoordinate(GPS_COURSE_START_Y);
        mGps.adjustDirection(GPS_COURSE_START_DIRECTION);
        break;
    	
    }
}

/**
 * 適切なドライバを選択し、運転させる
 */
void OutCourse::drive()
{
#if 0 // ログ送信(0：解除、1：実施)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
	LOGGER_DATAS08[1] = (S8)(mLineDetector.detect()); // 一瞬だけなのでログに残らない可能性あり
	LOGGER_DATAU16    = (U16)(mWallDetector.detect());
	LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
	LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
	LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
	LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
	LOGGER_DATAS32[0] = (S32)(mLeftMotor.getCount());
	LOGGER_DATAS32[1] = (S32)(mRightMotor.getCount());
	LOGGER_DATAS32[2] = (S32)(mLightSensor.get());
	LOGGER_DATAS32[3] = (S32)(mGyroSensor.get());
#endif
#if 0 // デバッグ(0：解除、1：実施)
    {
    	//DESK_DEBUG = true;
        static int count = 0;
        if (count++ % 25 == 0) {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "OutCourse");
            lcd.putf("dn", mState);
            lcd.putf("dn", (S32)mGps.getXCoordinate());
            lcd.putf("dn", (S32)mGps.getYCoordinate());
            lcd.putf("dn", (S32)mGps.getDirection());
            lcd.putf("dn", (S32)mGps.getDistance());
            lcd.disp();
        }
    }
#endif
    if (mState == OutCourse::START) { // スタート後通常区間
        if (mNormalDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_SEESAW_START, MakePoint(X, Y))) { // 区間をシーソー区間に更新
                mState = OutCourse::SEESAW;
            }
        }
    }
    else if (mState == OutCourse::SEESAW) { // シーソー区間
        if (mSeesawDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_STAIRWAY_START, MakePoint(X, Y))) { // 区間を階段区間に更新
                mState = OutCourse::STAIRWAY;
            }
        }
    }
    else if (mState == OutCourse::STAIRWAY) { // 階段区間
        if (mStairwayDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_GARAGEIN_START, MakePoint(X, Y))) { // 区間をガレージ区間に更新
                mState = OutCourse::GARAGEIN;
            }
        }
    }
    /* ETロボコン2011 追記*/
    else if (mState == OutCourse::ETSUMO) { // ET相撲区間
        if (mETsumoDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_GARAGEIN_START, MakePoint(X, Y))) { // 区間をガレージ区間に更新
                //mState = OutCourse::GARAGEIN;
            }
        }
    }
    /* ETロボコン2011 追記ここまで */
    else if (mState == OutCourse::GARAGEIN) { // ガレージ・イン区間
        mOutGarageDriver.drive();
    }
    // テストドライバ起動
    else {
        mTestDriver.drive();
    }
}

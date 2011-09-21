//
// InCourse.cpp
//
#include "InCourse.h"
#include "factory.h"
extern int gSonarDistance;

/**
 * コンストラクタ
 *
 * @param[in] aSection どのセクションから始めるか。デフォルトはスタート地点から。
 */
InCourse::InCourse(InCourse::eSection aSection)
    : Course()
{
    mNameId = 0;
    mState = aSection;
    switch (mState) {
    
    case InCourse::SEESAW:
        // シーソーからテスト
        mGps.adjustXCoordinate(GPS_SEESAW_START_X);
        mGps.adjustYCoordinate(GPS_SEESAW_START_Y);
        mGps.adjustDirection(GPS_SEESAW_START_DIRECTION);
        break;
    case InCourse::STAIRWAY:
        // STAIRWAYからテスト
        mGps.adjustXCoordinate(GPS_STAIRWAY_START_X);
        mGps.adjustYCoordinate(GPS_STAIRWAY_START_Y);
        mGps.adjustDirection(GPS_STAIRWAY_START_DIRECTION);
        break;
    case InCourse::GARAGEIN:
        // ガレージインからテスト
        mGps.adjustXCoordinate(GPS_GARAGEIN_START_X);
        mGps.adjustYCoordinate(GPS_GARAGEIN_START_Y);
        mGps.adjustDirection(GPS_GARAGEIN_START_DIRECTION);
        break;
    case InCourse::START:
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
void InCourse::drive()
{
#if 0 // ログ送信(0：解除、1：実施)
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(mState);
    LOGGER_DATAS08[1] = (S8)(mEnigmaDriver.detect()); // 一瞬だけなのでログに残らない可能性あり
    LOGGER_DATAU16    = (U16)(mStraightDetector.detect());
    LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
    LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
    LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
    LOGGER_DATAS16[3] = (S16)(mGps.getDistance());
    LOGGER_DATAS32[0] = (S32)(mLeftMotor.getCount());
    LOGGER_DATAS32[1] = (S32)(mRightMotor.getCount());
    LOGGER_DATAS32[2] = (S32)(mLightSensor.get());
    LOGGER_DATAS32[3] = (S32)(gSonarDistance);
#endif
    
#if 0 // デバッグ(0：解除、1：実施)
    {
        static int count = 0;
        if (count++ % 25 == 0) {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "InCourse");
            lcd.putf("dn", mState);
            lcd.disp();
        }
    }
#endif
    
    if (mState == InCourse::START) { // スタート後通常区間
        if (mNormalDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_SEESAW_START, MakePoint(X, Y))) {
                mState = InCourse::SEESAW;
            }
        }
    }
    else if (mState == InCourse::SEESAW) { // シーソー区間
        if (mSeesawDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_STAIRWAY_START, MakePoint(X, Y))) { // 区間を階段区間に更新
                mState = InCourse::STAIRWAY;
            }
        }
    }
    else if (mState == InCourse::STAIRWAY) { // 階段区間
        if (mStairwayDriver.drive()) {
            float X = mGps.getXCoordinate();
            float Y = mGps.getYCoordinate();
            if (inRegion(GPS_GARAGEIN_START, MakePoint(X, Y))) { // 区間をガレージ区間に更新
                mState = InCourse::GARAGEIN;
            }
        }
    }
    else if (mState == InCourse::GARAGEIN) { // ガレージ・イン区間
        mGarageDriver.drive();
    }
    // テストドライバ起動
    else { 
        mTestDriver.drive();
    }     
}

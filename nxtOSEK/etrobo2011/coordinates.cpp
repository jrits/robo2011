#include "factory.h"
#include "define.h"
#include "coordinates.h"

// コース＆ドライバ
#ifdef INCOURSE
Gps mGps(mLeftMotor, mRightMotor, Gps::IN);
InCourse mInCourse(INCOURSE);
Course *mCourse(&mInCourse);
int TRACE_EDGE = LEFT;
//int TRACE_EDGE = RIGHT;
#endif
#ifdef OUTCOURSE
Gps mGps(mLeftMotor, mRightMotor, Gps::OUT);
OutCourse mOutCourse(OUTCOURSE);
Course *mCourse(&mOutCourse);
//int TRACE_EDGE = RIGHT;
int TRACE_EDGE = LEFT;
#endif

// 座標
#ifdef INCOURSE
// インコーススタート座標
float GPS_COURSE_START_X = 5140.0;
float GPS_COURSE_START_Y = -504.0;
float GPS_COURSE_START_DIRECTION = 180.0;

// シーソー開始座標
// 中間ゲート
float GPS_SEESAW_START_X = 2934.0;
float GPS_SEESAW_START_Y = -3093.0;
float GPS_SEESAW_START_DIRECTION = 360.0;

// 階段開始座標
// 灰色マーカ開始場所
float GPS_STAIRWAY_START_X = 4277.0;
float GPS_STAIRWAY_START_Y = -1500.0;
float GPS_STAIRWAY_START_DIRECTION = 180.0;

// インコースガレージ・イン開始座標
// ゴール前直線上（階段の直線の延長線と交わる座標）
float GPS_GARAGEIN_START_X = 4790.0;
float GPS_GARAGEIN_START_Y = -1500.0;
float GPS_GARAGEIN_START_DIRECTION = 450.0;

// インコースガレージ中心座標
float GPS_GARAGE_X = 4790;
float GPS_GARAGE_Y = -870;
float GPS_GARAGE_DIRECTION = 450;

Region GPS_SEESAW_START = MakeRegion(GPS_SEESAW_START_X, GPS_SEESAW_START_Y + 250.0, GPS_SEESAW_START_X + 500.0, GPS_SEESAW_START_Y - 250.0);
Region GPS_STAIRWAY_START = MakeRegion(GPS_STAIRWAY_START_X - 500.0, GPS_STAIRWAY_START_Y  + 250.0, GPS_STAIRWAY_START_X, GPS_STAIRWAY_START_Y - 250.0);
Region GPS_GARAGEIN_START = MakeRegion(GPS_GARAGEIN_START_X - 250.0, GPS_GARAGEIN_START_Y -500.0,GPS_GARAGEIN_START_X + 250.0, GPS_GARAGEIN_START_Y);
#endif

#ifdef OUTCOURSE
// アウトコーススタート座標
float GPS_COURSE_START_X = 4323.0;
float GPS_COURSE_START_Y = -255.0;
float GPS_COURSE_START_DIRECTION = 180.0;

// ルックアップゲート開始座標
// 中間ゲート
float GPS_LOOKUP_START_X = 2934.0;
float GPS_LOOKUP_START_Y = -3350.0;
float GPS_LOOKUP_START_DIRECTION = 360.0;

// ET相撲開始座標
// ルックアップゲート通過後、左カーブ開始場所
float GPS_ETSUMO_START_X = 3918.0; 
float GPS_ETSUMO_START_Y = -3350.0;
float GPS_ETSUMO_START_DIRECTION = 360.0;

// アウトコースガレージ・イン開始座標
// ゴール前直線上（階段の直線の延長線と交わる座標）
float GPS_GARAGEIN_START_X = 5214.0;
float GPS_GARAGEIN_START_Y = -1500.0;
float GPS_GARAGEIN_START_DIRECTION = 450.0;

// アウトコースガレージ中心座標
float GPS_GARAGE_X = 5214.0;
float GPS_GARAGE_Y = -870.0;
float GPS_GARAGE_DIRECTION = 450.0;

Region GPS_LOOKUP_START = MakeRegion(GPS_LOOKUP_START_X, GPS_LOOKUP_START_Y - 250.0, GPS_LOOKUP_START_X + 250.0, GPS_LOOKUP_START_Y + 250.0);
Region GPS_ETSUMO_START = MakeRegion(GPS_ETSUMO_START_X, GPS_ETSUMO_START_Y - 250.0, GPS_ETSUMO_START_X + 250.0, GPS_ETSUMO_START_Y + 250.0);
Region GPS_GARAGEIN_START = MakeRegion(GPS_GARAGEIN_START_X - 250.0, GPS_GARAGEIN_START_Y -500.0,GPS_GARAGEIN_START_X + 250.0, GPS_GARAGEIN_START_Y);
#endif

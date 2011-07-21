#include "factory.h"
#include "coordinates.h"

//#define INCOURSE
#define OUTCOURSE

// コース
#ifdef INCOURSE
Gps mGps(mLeftMotor, mRightMotor, Gps::IN);
//InCourse mInCourse(InCourse::TESTDRIVE); // テストドライバ起動
InCourse mInCourse(InCourse::START);     // 開始位置は下参照
//InCourse mInCourse(InCourse::ENIGMA);    // 開始位置は下参照
//InCourse mInCourse(InCourse::MYSTERY);   // 開始位置は下参照
//InCourse mInCourse(InCourse::GARAGEIN);  // 開始位置は下参照
Course *mCourse(&mInCourse);
//int TRACE_EDGE = LEFT;
int TRACE_EDGE = RIGHT;
#endif
#ifdef OUTCOURSE
Gps mGps(mLeftMotor, mRightMotor, Gps::OUT);
//OutCourse mOutCourse(OutCousrse::TESTDRIVE); // テストドライバ起動
//OutCourse mOutCourse(OutCourse::START);      // 開始位置は下参照
//OutCourse mOutCourse(OutCourse::SEESAW);     // 開始位置は下参照
//OutCourse mOutCourse(OutCourse::STAIRWAY);   // 開始位置は下参照
OutCourse mOutCourse(OutCourse::ETSUMO);       // 開始位置は下参照 ETロボコン2011 追記
//OutCourse mOutCourse(OutCourse::GARAGEIN);   // 開始位置は下参照
Course *mCourse(&mOutCourse);
int TRACE_EDGE = RIGHT;
#endif

// 座標
#ifdef INCOURSE
// インコーススタート座標
float GPS_COURSE_START_X = 2322.0;
float GPS_COURSE_START_Y = -525.0;
float GPS_COURSE_START_DIRECTION = 180.0;

// エニグマスタート座標
// インコースガレージ手前(右)線
float GPS_ENIGMA_START_X = 1860.0;
float GPS_ENIGMA_START_Y = -490.0;
float GPS_ENIGMA_START_DIRECTION = 180.0;

// ミステリーサークル開始座標
// インコース、アウトコースシーソー後マーカー終わり地点
float GPS_MYSTERY_START_X = 3000.0;
float GPS_MYSTERY_START_Y = -3150.0;
float GPS_MYSTERY_START_DIRECTION = 0.0;

// インコースガレージ・イン開始座標
// インコース坂手前カーブの一番出っ張っている所
float GPS_GARAGEIN_START_X = 5019.0;
float GPS_GARAGEIN_START_Y = -957.0;
float GPS_GARAGEIN_START_DIRECTION = 90.0;

// インコースガレージ中心座標
float GPS_GARAGE_X = 1737;
float GPS_GARAGE_Y = -744;
float GPS_GARAGE_DIRECTION = 270;

Region GPS_ENIGMA_START = MakeRegion(X_LEFT, Y_TOP, GPS_ENIGMA_START_X, Y_DOWN);
Region GPS_MYSTERY_START = MakeRegion(GPS_MYSTERY_START_X, Y_TOP, X_RIGHT, Y_DOWN);
Region GPS_GARAGEIN_START = MakeRegion(4500, Y_TOP, X_RIGHT, GPS_GARAGEIN_START_Y);
#endif

#ifdef OUTCOURSE
// アウトコーススタート座標
float GPS_COURSE_START_X = 2020.0;
float GPS_COURSE_START_Y = -237.0;
float GPS_COURSE_START_DIRECTION = 180.0;

// シーソー開始座標
// アウトコース衝立２本目目の前
float GPS_SEESAW_START_X = 1200.0;
float GPS_SEESAW_START_Y = -3393.0;
float GPS_SEESAW_START_DIRECTION = 360.0;
/*
float GPS_SEESAW_START_X = 1500.0;
float GPS_SEESAW_START_Y = -3393.0;
float GPS_SEESAW_START_DIRECTION = 360.0;
*/
// 階段開始座標
// シーソー後直線、x座標は緑の島の右端
//float GPS_STAIRWAY_START_X = 5000.0;
//float GPS_STAIRWAY_START_Y = -2800.0;
//float GPS_STAIRWAY_START_DIRECTION = 450.0;
float GPS_STAIRWAY_START_X = 4500.0;
float GPS_STAIRWAY_START_Y = -3400.0;
float GPS_STAIRWAY_START_DIRECTION = 360.0;

/* ETロボコン2011 追記*/
// ET相撲開始座標
float GPS_ETSUMO_START_X = 0.0; // ＠todo要再設定
float GPS_ETSUMO_START_Y = 0.0; // ＠todo要再設定
float GPS_ETSUMO_START_DIRECTION = 0.0; // ＠todo要再設定
/* ETロボコン2011 追記ここまで */

// アウトコースガレージ・イン開始座標
// 階段後直線、緑の端
float GPS_GARAGEIN_START_X = 5238.0;
float GPS_GARAGEIN_START_Y = -900.0;
float GPS_GARAGEIN_START_DIRECTION = 450.0;

// アウトコースガレージ中心座標
float GPS_GARAGE_X = 156.0;
float GPS_GARAGE_Y = -229.0;
float GPS_GARAGE_DIRECTION = 540.0;

//Region GPS_SEESAW_START = MakeRegion(X_LEFT, GPS_SEESAW_START_Y, X_RIGHT, Y_DOWN);
Region GPS_SEESAW_START = MakeRegion(GPS_SEESAW_START_X, -1500, X_RIGHT, Y_DOWN);
Region GPS_STAIRWAY_START = MakeRegion(GPS_STAIRWAY_START_X, Y_TOP, X_RIGHT, Y_DOWN);
Region GPS_GARAGEIN_START = MakeRegion(X_LEFT, Y_TOP, X_RIGHT, GPS_GARAGEIN_START_Y);
#endif

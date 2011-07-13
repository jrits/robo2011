#ifndef __FACTORY_H_
#define __FACTORY_H_
/*
// ECRobot C++ APIの宣言
#include "TouchSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Lcd.h"
#include "Clock.h"
#include "Nxt.h"
#include "Bluetooth.h"
#include "Daq.h"
#include "Vector.h"
// GPS/ヒストリとそのアダプタ群
#include "Gps.h"
#include "XCoordinate.h"
#include "YCoordinate.h"
#include "Angle.h"
#include "Distance.h"
#include "History.h"
#include "TimeDifference.h"
#include "MovingAverage.h"
// 検出
#include "Detector.h"
#include "DownSlopeDetector.h"
#include "WallDetector.h"
// スキル
#include "Skill.h"
#include "LineTrace.h"
#include "AngleTrace.h"
// ドライバ
#include "Driver.h"
#include "NormalDriver.h"
#include "SeesawDriver.h"
// コース
#include "Navigator.h"
#include "InCourse.h"
#include "OutCourse.h"
// その他
#include "Activator.h"
#include "TouchStarter.h"
#include "Pid.h"
#include "Robot.h"

// デバイスオブジェクト
extern TouchSensor mTouchSensor;
extern LightSensor mLightSensor;
extern GyroSensor mGyroSensor;
extern Motor mLeftMotor;
extern Motor mRightMotor;
extern Lcd mLcd;
extern Clock mClock;
extern Nxt mNxt;
extern Bluetooth mBluetooth;
extern Daq mDaq;
// GPSとそのアダプタ群
extern Gps mGps;
extern XCoordinate mXCoordinate;
extern YCoordinate mYCoordinate;
extern History mHMotorR;
extern History mHMotorL;
extern History mHLight;
extern History mHAccR;
extern History mHAccL;
extern Angle mAngle;
extern Distance mDistance;
extern TimeDifference mTimeDifference;
// その他
extern Activator mActivator;
extern Pid mLightPid;
extern Pid mAnglePid;
// 検出
extern WallDetector mWallDetector;
extern DownSlopeDetector mDownSlopeDetector;
// Skill
extern LineTrace mLineTrace;
extern AngleTrace mAngleTrace;
// Driver
extern NormalDriver mNormalDriver;
// Navigator
extern InCourse mInCourse;
extern OutCourse mOutCourse;
// アクティブオブジェクト
extern Robot mRobot;
*/
//Historyのデータ領域
extern float data_3[3];
extern float data_10[10];

#endif

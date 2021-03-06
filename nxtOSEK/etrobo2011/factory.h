#ifndef __FACTORY_H_
#define __FACTORY_H_

// ECRobot C++ APIの宣言
#include "TouchSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "SonarSensor.h"
#include "Motor.h"
#include "Lcd.h"
#include "Clock.h"
#include "Nxt.h"
#include "Bluetooth.h"
#include "Daq.h"
#include "Vector.h"
// GPS/ヒストリとそのアダプタ群
#include "Gps.h"
#include "History.h"
// 検出
#include "Detector.h"
#include "DownSlopeDetector.h"
#include "WallDetector.h"
#include "LineDetector.h"
#include "StraightDetector.h"
#include "FailDetector.h"
#include "MarkerDetector.h"
// スキル
#include "Skill.h"
#include "LineTrace.h"
#include "TripodLineTrace.h"
#include "AngleTrace.h"
#include "TripodAngleTrace.h"
#include "CoordinateStop.h"
#include "CoordinateTrace.h"
#include "TripodCoordinateTrace.h"
#include "VirtualLineTrace.h"
#include "SlowdownSkill.h"
#include "StopSkill.h"
#include "StandUpSkill.h"
#include "SitDownSkill.h"
#include "StandupDriver.h"

// ドライバ
#include "Driver.h"
#include "NormalDriver.h"
#include "SeesawDriver.h"
#include "StairwayDriver.h"
#include "GarageDriver.h"
#include "TestDriver.h"
#include "LookUpGateDriver.h"
#include "ETsumoDriver.h"
// コース
#include "Course.h"
#include "InCourse.h"
#include "OutCourse.h"
// その他
#include "Activator.h"
#include "TripodActivator.h"
#include "Pid.h"
#include "struct.h"

// デバイスオブジェクト
extern TouchSensor mTouchSensor;
extern SonarSensor mSonarSensor;
extern LightSensor mLightSensor;
extern GyroSensor mGyroSensor;
extern Motor mLeftMotor;
extern Motor mRightMotor;
extern Motor mTailMotor;
extern Lcd mLcd;
extern Clock mClock;
extern Nxt mNxt;
extern Bluetooth mBluetooth;
extern Daq mDaq;
extern Speaker mSpeaker;
// GPS
extern Gps mGps;
// History
extern History mLightHistory;
extern History mRightMotorHistory;
extern History mLeftMotorHistory;
extern History mRightMotorSpeedHistory;
extern History mLeftMotorSpeedHistory;
extern History mDirectionHistory;
extern History mDirectionAverageHistory;
extern History mGyroHistory;
extern History mTurnHistory;
// その他
extern Activator mActivator;
extern TripodActivator mTripodActivator;
extern Pid mLightPid;
extern Pid mForwardPid;
extern Pid mAnglePid;
extern Pid mSlowdownPid;
extern Pid mStopPid;
// 検出
extern WallDetector mWallDetector;
extern DownSlopeDetector mDownSlopeDetector;
extern LineDetector mLineDetector;
extern StraightDetector mStraightDetector;
extern FailDetector mFailDetector;
extern MarkerDetector mMarkerDetector;
// Skill
extern LineTrace mLineTrace;
extern TripodLineTrace mTripodLineTrace;
extern AngleTrace mAngleTrace;
extern TripodAngleTrace mTripodAngleTrace;
extern CoordinateTrace mCoordinateTrace;
extern TripodCoordinateTrace mTripodCoordinateTrace;
extern VirtualLineTrace mVirtualLineTrace;
extern SlowdownSkill mSlowdownSkill;
extern StopSkill mStopSkill;
extern StandUpSkill mStandUpSkill;
extern SitDownSkill mSitDownSkill;
extern CoordinateStop mCoordinateStop;
extern StandupDriver mStandupDriver;

// Driver
extern NormalDriver mNormalDriver;
extern SeesawDriver mSeesawDriver;
extern StairwayDriver mStairwayDriver;
extern GarageDriver mGarageDriver;
extern TestDriver mTestDriver;
extern ETsumoDriver mETsumoDriver;
extern LookUpGateDriver mLookUpGateDriver;
extern TestDriver mSeesawTestDriver;

// Course
extern Course *mCourse;
extern InCourse mInCourse;
extern OutCourse mOutCourse;

#endif

#include "factory.h"
#include "constants.h"
#include "macro.h"

// デバイスオブジェクトの生成
TouchSensor mTouchSensor(PORT_1);
SonarSensor mSonarSensor(PORT_2);
LightSensor mLightSensor(PORT_3);
GyroSensor mGyroSensor(PORT_4);
Motor mLeftMotor(PORT_B);
Motor mRightMotor(PORT_C);
Lcd mLcd;
Clock mClock;
Nxt mNxt;
Bluetooth mBluetooth;
Daq mDaq(mBluetooth);
// History
float mLightBuffer[25]; // % 25
History mLightHistory(mLightBuffer, COUNTOF(mLightBuffer));
float mRightMotorBuffer[25];
History mRightMotorHistory(mRightMotorBuffer, COUNTOF(mRightMotorBuffer));
float mLeftMotorBuffer[25];
History mLeftMotorHistory(mLeftMotorBuffer, COUNTOF(mLeftMotorBuffer));
float mRightMotorSpeedBuffer[25];
History mRightMotorSpeedHistory(mRightMotorSpeedBuffer, COUNTOF(mRightMotorSpeedBuffer));
float mLeftMotorSpeedBuffer[25];
History mLeftMotorSpeedHistory(mLeftMotorSpeedBuffer, COUNTOF(mLeftMotorSpeedBuffer));
float mDirectionBuffer[25];
History mDirectionHistory(mDirectionBuffer, COUNTOF(mDirectionBuffer));
float mDirectionAverageBuffer[25];
History mDirectionAverageHistory(mDirectionAverageBuffer, COUNTOF(mDirectionAverageBuffer));
float mGyroBuffer[25];
History mGyroHistory(mGyroBuffer, COUNTOF(mGyroBuffer));
// その他
Activator mActivator(mLeftMotor, mRightMotor, mGyroSensor, mNxt);
Pid mLightPid(LIGHT_PID_KP, LIGHT_PID_KI, LIGHT_PID_KD); // LineTrace用
Pid mAnglePid(ANGLE_PID_KP, ANGLE_PID_KI, ANGLE_PID_KD); // AngleTrace用
Pid mSlowdownPid(SLOWDOWN_PID_KP, SLOWDOWN_PID_KI, SLOWDOWN_PID_KD); // SlowdownSkill用
Pid mStopPid(STOP_PID_KP, STOP_PID_KI, STOP_PID_KD); // CoordinateStop用 @obsolete
// 検出
WallDetector mWallDetector;
DownSlopeDetector mDownSlopeDetector;
LineDetector mLineDetector;
StraightDetector mStraightDetector;
FailDetector mFailDetector;
// Skill
LineTrace mLineTrace(BLACK, WHITE, LINE_THRESHOLD);
AngleTrace mAngleTrace;
CoordinateTrace mCoordinateTrace;
VirtualLineTrace mVirtualLineTrace;
SlowdownSkill mSlowdownSkill;
StopSkill mStopSkill;
CoordinateStop mCoordinateStop; // @obsolete
// Driver
NormalDriver mNormalDriver;
SeesawDriver mSeesawDriver;
StairwayDriver mStairwayDriver;
OutGarageDriver mOutGarageDriver;
EnigmaDriver mEnigmaDriver;
MysteryDriver mMysteryDriver;
InGarageDriver mInGarageDriver;
TestDriver mTestDriver;

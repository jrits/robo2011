#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define LEFT -1
#define RIGHT 1

#ifdef DEBUG
#include<iostream>
using namespace std;
#endif

#include "coordinates.h"
#include "struct.h"
extern "C" {
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
}

extern char BT_NAME[];
extern int USER_GYRO_OFFSET;

//GPS
extern float WHEEL_RADIUS;
extern float WHEEL_DISTANCE;

//LineTrace
extern int INIT_FORWARD;
extern int INIT_SAMPLECOUNT;
extern int FORWARD;
extern int WHITE;
extern int BLACK;
extern int LINE_THRESHOLD;
extern int TRACE_EDGE;
extern float LIGHT_PID_KP;
extern float LIGHT_PID_KI;
extern float LIGHT_PID_KD;
extern float LIGHT_ONOFF_K;

//AngleTrace
extern float ANGLE_PID_KP;
extern float ANGLE_PID_KI;
extern float ANGLE_PID_KD;

//SlowdownSkill
extern float SLOWDOWN_PID_KP;
extern float SLOWDOWN_PID_KI;
extern float SLOWDOWN_PID_KD;

//CoordinateStop
extern float STOP_PID_KP;
extern float STOP_PID_KI;
extern float STOP_PID_KD;

//転倒検出
extern int FAILSAFE_SAMPLECOUNT;
extern int FAILSAFE_THRESHOLD;

//検知系
extern float WALL_THRESHOLD;
extern float DOWNSLOPE_THRESHOLD;

//balance_control
extern float K_THETADOT;
extern float K_PHIDOT;

//Bluetoothログ送信
extern int LOGGER_SEND;
extern S8  LOGGER_DATAS08[2];
extern U16 LOGGER_DATAU16;
extern S16 LOGGER_DATAS16[4];
extern S32 LOGGER_DATAS32[4];

//Activator
extern bool DESK_DEBUG;

//LineTraceフラグ
extern bool gLineTrace;
extern bool gTripodLineTrace;

//#include "Speaker.h"
//#define beep(r) { Speaker s; s.playTone(r,r,80);}

#endif // __CONSTANTS_H__

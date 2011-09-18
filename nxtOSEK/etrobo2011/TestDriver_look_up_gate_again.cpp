///////////////////////////////////////////////////////////
//  TestDriver.cpp
//  Implementation of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "TestDriver.h"
#include "factory.h"
#include "TestLine.h"
#include "LookUpGateDriver.h"

extern bool gDoForwardPid;
extern bool gDoMaimai;
extern "C" extern void tail_control(signed int angle);

/* sample_c3マクロ */
//#define TAIL_ANGLE_STAND_UP 108 /* 完全停止時の角度[度] */
//#define TAIL_ANGLE_TRIPOD_DRIVE 95 /* ３点走行時の角度[度] */
//#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */

/**
 * コンストラクタ
 */
TestDriver::TestDriver()
{
    mState = -1; // 初期化状態
}

bool TestDriver::drive()
{
  if(mState == -1){
    mState = 0;
  }
  // mStandUpSkill.execute();
  // return false;
  return mLookUpGateDriver.drive();
}

#include "LookUpGateDriver.h"

#include "factory.h"
#include "coordinates.h"

extern bool gDoMaimai;
extern bool gDoSonar; //!< ソナーセンサ発動フラグ
extern bool gSonarIsDetected; //!< 衝立検知の結果
extern bool gDoProgressiveTurn;
extern int gSonarTagetDistance;

extern "C"{
  extern void tail_control(signed int);
}

namespace{
  const int SPEED_UNDER_LOOKUP_GATE = 50;
  const int TAIL_ANGLE_FOR_TRIPOD_LINETRACE = 70;
  // @todo ゲートの座標を。
};
LookUpGateDriver::LookUpGateDriver()
  : mCurrentSubSection(INIT),
    mOverrunDistance(100),
    mAdjustCoordinatesFlag(false)
{
}

LookUpGateDriver::~LookUpGateDriver(){
}

bool
LookUpGateDriver::drive(){
  switch(mCurrentSubSection){
  case INIT:
    mLineTrace.setForward(10);
    mLineTrace.execute();
      
    gDoSonar = true;
    if(foundGate()){
      // 最初にゲートを見つけたときに音を出す。    
      gDoMaimai = true;
      mCurrentSubSection = IN_FRONT_OF_GATE;
    }
    break;
  case IN_FRONT_OF_GATE:
    // 座り込む。
    if(sitDown()){
      gDoProgressiveTurn = false;
      gDoMaimai = false;
      tail_control(TAIL_ANGLE_FOR_TRIPOD_LINETRACE);
      mTripodLineTrace.setForward(SPEED_UNDER_LOOKUP_GATE);
      mTripodLineTrace.execute();
    }
    if(isUnderGate()) {
      mCurrentSubSection = UNDER_GATE;
      mSpeaker.playTone(700, 1, 50);
    }
    break;
  case UNDER_GATE:// ゲートの真下。
    // 3点傾立走行。
    gDoProgressiveTurn = false;
    gDoMaimai = false;
    tail_control(TAIL_ANGLE_FOR_TRIPOD_LINETRACE);
    mTripodLineTrace.setForward(SPEED_UNDER_LOOKUP_GATE);
    mTripodLineTrace.execute();
    if(passedGate()){
      mCurrentSubSection = BEHIND_GATE;
    }
    break;
  case BEHIND_GATE:
    /*立ち上がる。*/
    if(standUp()){
      mCurrentSubSection = DONE;
    }
    mLcd.disp();
    break;
  case DONE:
    mLineTrace.execute();
    break;
  default:
    // assertするべき。
    break;
  }

  return isDone();
}

bool
LookUpGateDriver::foundGate() const{
  return gSonarIsDetected;
}

bool
LookUpGateDriver::isUnderGate() const {
  // ゲートまでの距離が10cm以下ならば、ゲートの真下と判定する。
  return gSonarTagetDistance < 100;
}

bool
LookUpGateDriver::passedGate() {
  // 障害物が検知出来なくなったら、ゲートを通過と判定する。
  // この処理は１回しか実行されない。
  if (!gSonarIsDetected && !mAdjustCoordinatesFlag) {
    mGps.adjustXCoordinate(GPS_LOOKUP_GATE_X);
    mGps.adjustYCoordinate(GPS_LOOKUP_GATE_Y);
    mDistanceAtGate = mGps.getDistance();
    mAdjustCoordinatesFlag = true;
  }

  // ゲートを通過してからmOverrunDistanceだけオーバーランする。
  return (mGps.getDistance() - mDistanceAtGate) > mOverrunDistance;
}

bool
LookUpGateDriver::sitDown() {
  static int count = 0;
  mSitDownSkill.setAngle(80);
  mSitDownSkill.execute();
  count++;

  // とりあえず、2秒経過したら座っていることにする。
  // TODO 座っているか、否かの検出方法を洗練する必要あり。
  return count >= 1000;
}

bool
LookUpGateDriver::standUp() {
  static int stand_up_count = 0;
  mStandUpSkill.execute();
  stand_up_count++;
  
  // とりあえず、2秒経過したら立っていることにする。
  // TODO 立ち上ったか、否かの検出方法を洗練する必要あり。
  return stand_up_count >= 5000;
}

bool
LookUpGateDriver::isDone() const{
  return mCurrentSubSection == DONE;
}

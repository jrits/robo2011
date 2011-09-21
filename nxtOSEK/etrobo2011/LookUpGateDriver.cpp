#include "LookUpGateDriver.h"

#include "factory.h"
extern bool gDoMaimai;
extern bool gDoSonar; //!< ソナーセンサ発動フラグ
extern bool gSonarIsDetected; //!< 衝立検知の結果
extern int gSonarTagetDistance;

extern "C"{
extern void tail_control(signed int);
}

LookUpGateDriver::LookUpGateDriver()
    : mCurrentSubSection(INIT) {

}

LookUpGateDriver::~LookUpGateDriver(){
}

bool
LookUpGateDriver::drive(){
  switch(mCurrentSubSection){
    case INIT:
      //gDoMaimai = true;
      mLineTrace.setForward(20);
      mLineTrace.execute();

      if(foundGate()){
      	gDoSonar = true;
        gDoMaimai = false; //!< まいまい式発動フラグ
        gDoForwardPid = false;
      	//gDoMaimai = true;
        mLcd.clear();
        mLcd.putf("sn","INIT");
        mLcd.disp();
        mCurrentSubSection = IN_FRONT_OF_GATE;
      }
      break;
    case IN_FRONT_OF_GATE:
      // 座り込む。
      if(sitDown()){
        mLcd.clear();
        mLcd.putf("sn","IN_FRONT_OF_GATE");
        mLcd.disp();
        mCurrentSubSection = UNDER_GATE;
      }
      break;
    case UNDER_GATE:
      // 3点傾立走行。
      mTripodAngleTrace.setForward(50);
      mTripodAngleTrace.setTargetAngle(360);
      mTripodAngleTrace.execute();
      if(passedGate()){
        mLcd.clear();
        mLcd.putf("sn","UNDER_GATE");
        mLcd.disp();
        mCurrentSubSection = UNDER_GATE_2;
      }
      break;
    case UNDER_GATE_2:
      static int time = 0;
      time++;
      if(time > 500){
        mCurrentSubSection = DONE;//一時的
      }
      mTripodAngleTrace.setForward(50);
      mTripodAngleTrace.setTargetAngle(360);
      mTripodAngleTrace.execute();
      break;
    case BEHIND_GATE:
      /*立ち上がる。*/
      if(standUp()){
        mLcd.clear();
        mLcd.putf("sn","BEHIND_GATE");
        mLcd.disp();
        mCurrentSubSection = DONE;
      }
      break;
    case DONE:
      mTripodAngleTrace.execute();
      break;
    default:
      // assertするべき。
      break;
  }

  return isDone();
}

bool
LookUpGateDriver::foundGate() const{

	static int count = 0;

	// とりあえず、1秒経過したらゲートを見つけたことにする。
	// TODO ゲートの検知方法を実装する必要あり。
	count++;
	return count >= 250;

}

bool
LookUpGateDriver::passedGate() const{
	if(gSonarIsDetected  && (gSonarTagetDistance < 100.0)){
	    mGps.adjustXCoordinate(3500.0);
		mSpeaker.playTone(1000, 1, 100);
		return 1;
	}
	else{
		return 0;
	}
		/*
  
  // とりあえず、1秒経過したらゲートを通過したことにする。
  // TODO ゲート通過の検知方法を実装する必要あり。
  count++;
  return count >= 250;
		*/
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
  static int count = 0;
  mStandUpSkill.execute();
  count++;
  
  // とりあえず、2秒経過したら立っていることにする。
  // TODO 立ち上ったか、否かの検出方法を洗練する必要あり。
  return count >= 500;
}

bool
LookUpGateDriver::isDone() const{
  return mCurrentSubSection == DONE;
}

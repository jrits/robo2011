#include "LookUpGateDriver.h"

#include "factory.h"

LookUpGateDriver::LookUpGateDriver()
    : mCurrentSubSection(INIT) {

}

LookUpGateDriver::~LookUpGateDriver(){
}

bool
LookUpGateDriver::drive(){
  switch(mCurrentSubSection){
    case INIT:
      mLineTrace.setForward(20);
      mLineTrace.execute();

      if(foundGate()){
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
      mTripodLineTrace.setForward(10);
      mTripodLineTrace.execute();
      tail_control(60);
      if(passedGate()){
        mLcd.clear();
        mLcd.putf("sn","UNDER_GATE");
        mLcd.disp();
        mCurrentSubSection = BEHIND_GATE;
      }
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
  static int count = 0;
  
  // とりあえず、1秒経過したらゲートを見つけたことにする。
  // TODO ゲートの検知方法を実装する必要あり。
  count++;
  return count >= 250;
}

bool
LookUpGateDriver::passedGate() const{
  static int count = 0;
  // とりあえず、1秒経過したらゲートを通過したことにする。
  // TODO ゲート通過の検知方法を実装する必要あり。
  count++;
  return count >= 250;
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

void
LookUpGateDriver::tail_control(signed int angle) const {
  static const float P_GAIN = 2.5F;
  static const float PWM_ABS_MAX = 60; /* 完全停止用モータ制御PWM絶対最大値 */        
  float pwm = (float)(angle - mTailMotor.getCount())*P_GAIN; /* 比例制御 */

  /* PWM出力飽和処理 */
  if (pwm > PWM_ABS_MAX)
  {
    pwm = PWM_ABS_MAX;
  }
  else if (pwm < -PWM_ABS_MAX)
  {
    pwm = -PWM_ABS_MAX;
  }
  mTailMotor.setPWM((S8)pwm);
  mTailMotor.setBrake(true);
}

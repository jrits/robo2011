#ifndef STANDUPSKILL_H
#define STANDUPSKILL_H

#include "balancer.h"
#include "Activator.h"
#include "TripodActivator.h"
#include "Motor.h"

using namespace ecrobot;

class StandUpSkill {
 public:
  StandUpSkill(Activator& activator,
               TripodActivator& tripodActivator,
               Motor& leftMotor,
               Motor& rightMotor,
               Motor& tailMotor)
      : mActivator(activator),
        mTripodActivator(tripodActivator),
        mLeftMotor(leftMotor),
        mRightMotor(rightMotor),
        mTailMotor(tailMotor),
        mIsStandUp(false),
        mStableCount(0),
        mOnlyFirst(true)
  {
  }
  
  ~StandUpSkill(){}

  void execute(){
    static const VectorT<F32> command(1.0F,1.0F);
    if(!mIsStandUp){
      //立ち上がるまでしっぽの角度を上げ続ける。
      mIsStandUp = isStandUp();
      taiL_control(108);
      mTripodActivator.run(command);
    } else if(isStable()){
      if(mOnlyFirst){
        balance_init();
        mLeftMotor.reset();
        mRightMotor.reset();
        mOnlyFirst = false;
      }
      taiL_control(3);
      mActivator.run(command);
    }
    else {
      taiL_control(105);
      mLeftMotor.setPWM(0);
      mRightMotor.setPWM(0);
      mStableCount++;
    } 
  }
 private:
  void taiL_control(signed int angle) const{
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

  bool isStandUp(){
    // モータの角度が一定に達したら立ち上がったと判定する。
    return mTailMotor.getCount() > 100;
  }

  bool isStable(){
    return mStableCount > 1000;
  }
  
  Activator& mActivator;
  TripodActivator& mTripodActivator;
  Motor& mLeftMotor;
  Motor& mRightMotor;
  Motor& mTailMotor;
  bool mIsStandUp;
  int mStableCount;
  bool mOnlyFirst;
};

#endif

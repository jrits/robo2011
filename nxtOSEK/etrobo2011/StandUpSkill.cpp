#include "StandUpSkill.h"
#include "factory.h"
namespace{
static const signed int STANDING_ANGLE = 95;
};

void
StandUpSkill::execute(){
    static const VectorT<F32> command(1.0F,1.0F);

    if(!mIsStandUp){
      //立ち上がるまでしっぽの角度を上げ続ける。
      mIsStandUp = isStandUp();
      mTailMotor.setBrake(false);
      mLcd.putf("sn","Stainding UP");
      tail_control_with_PID(STANDING_ANGLE);
      mTripodActivator.run(command);
    } else if(isStable()){
      if(mOnlyFirst){
        balance_init();
        mLeftMotor.reset();
        mRightMotor.reset();
        mOnlyFirst = false;
      }
      tail_control_with_PID(3);
      mActivator.run(command);
    }
    else {
      mLcd.putf("sn","Stabling");
      tail_control_with_PID(STANDING_ANGLE);
      mLeftMotor.setPWM(0);
      mRightMotor.setPWM(0);
      mStableCount++;
      mTailMotor.setBrake(true);
    } 
}

bool
StandUpSkill::isStandUp() const {
  // モータの角度が一定に達したら立ち上がったと判定する。
  return mTailMotor.getCount() > STANDING_ANGLE - 2;
}

bool
StandUpSkill::isStable() const {
  return mStableCount > 10;
}

void
StandUpSkill::tail_control_with_PID(signed int target_angle){
    static const float K_P = 2.5F;
    static const float K_I = 0.02F;
    static const float K_D = 3.0F;
    signed int diff = target_angle - mTailMotor.getCount();

    float differential = diff - mLastDiff;
    mLastDiff = diff;
    mIterm += diff*K_I;
    float pwm = K_P*diff + mIterm + K_D * differential;

    if(pwm < -100) pwm = -100;
    if(pwm > 100) pwm = 100;

    mTailMotor.setPWM(static_cast<S8>(pwm));
}

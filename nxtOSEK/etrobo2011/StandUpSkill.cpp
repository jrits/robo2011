#include "StandUpSkill.h"
#include "factory.h"
// #include "nxt_motors.h"
// extern "C" extern nxt_motor_get_count(unsigned long);
//extern "C" extern void tail_control(signed int);

void
StandUpSkill::execute(){
    static const VectorT<F32> command(1.0F,1.0F);
    if(!mIsStandUp){
      //立ち上がるまでしっぽの角度を上げ続ける。
      mIsStandUp = isStandUp();
      tail_control_with_PID(108);
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
      tail_control_with_PID(105);
      mLeftMotor.setPWM(0);
      mRightMotor.setPWM(0);
      mStableCount++;
    } 
}

void
StandUpSkill::tail_control_with_PID(signed int target_angle){
    static const float K_P = 2.5F;
    static const float K_I = 0.0F;
    static const float K_D = 0.0F;
    signed int diff = target_angle - mTailMotor.getCount();

    float differential = diff - mLastDiff;
    mLastDiff = diff;
    mIterm += diff*K_I;

    float pwm = K_P*diff + mIterm + K_D * differential;
    mTailMotor.setPWM(static_cast<S8>(pwm)); 
}

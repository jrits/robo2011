#include "StandUpSkill.h"
#include "factory.h"
namespace{
static const signed int STANDING_ANGLE_1 = 90;
static const signed int STANDING_ANGLE_2 = 104;
};

StandUpSkill::StandUpSkill(Activator& activator,
			   TripodActivator& tripodActivator,
			   Motor& leftMotor,
			   Motor& rightMotor,
			   Motor& tailMotor)
      : mActivator(activator),
        mTripodActivator(tripodActivator),
        mLeftMotor(leftMotor),
        mRightMotor(rightMotor),
        mTailMotor(tailMotor),
	mSubState(INIT),
        mIsStandUp(false),
        mStableCount(0),
        mOnlyFirst(true),
        mLastDiff(0.0F),
        mIterm(0.0F)
{
}

void
StandUpSkill::execute(){
    static const VectorT<F32> command(0.0F,0.0F);
    S8 pwm_L = 0, pwm_R = 0;
    static long counter_1 = 0;
    static long counter_2 = 0;
    static S8 pwm_diff_1 = 0, pwm_diff_2 = 0;
    
    S8 pwm_Tail = 60;
    // 前転防止。
    if(mTailMotor.getCount() > (STANDING_ANGLE_2 + 3)){
      tail_control_with_PID(0);
      mSubState = STANDING_UP;
      mStableCount = 0;
    }

    // Activator使用禁止。
    switch(mSubState) {
    case INIT:
      mLeftMotor.setPWM(0);
      mRightMotor.setPWM(0);
      mTailMotor.setBrake(false);
      mSubState = TO_FIRST_ANGLE;
      break;
    case TO_FIRST_ANGLE:
      mSpeaker.playTone(1000, 10, 5);
      if(mTailMotor.getCount() < (STANDING_ANGLE_1 + 2) &&
	 mTailMotor.getCount() > (STANDING_ANGLE_1 - 2)) {
	//	mTailMotor.setPWM(0);
	//	mTailMotor.setBrake(true);
	++mStableCount;
	if(mStableCount > 500){
	  mTailMotor.setBrake(false);
	  mStableCount = 0;
	  mSubState = TO_SECOND_ANGLE;
	}
      } else {
	// tail_control_with_PID(STANDING_ANGLE_1);
	mTailMotor.setBrake(false);
	counter_1++;
	if(counter_1 % 50) {
	  if(mTailMotor.getCount() > STANDING_ANGLE_1){
	    pwm_diff_1--;
	  } else {
	    pwm_diff_1++;
	  }
	}
	mTailMotor.setPWM(pwm_Tail + pwm_diff_1);
      }
      
      break;
    case TO_SECOND_ANGLE:
      mSpeaker.playTone(900, 10, 5);
      if(mTailMotor.getCount() < (STANDING_ANGLE_2) + 1 &&
	 mTailMotor.getCount() > (STANDING_ANGLE_2) - 1) {
	// mTailMotor.setPWM(0);
	// mTailMotor.setBrake(true);
	++mStableCount;	
	if(mStableCount > 500){
	  mTailMotor.setBrake(true);
	  mStableCount = 0;
	  mSubState = TO_STABLE;
	  //mSubState = STANDING_UP;
	}
      } else {
	counter_2++;
	if(counter_2 % 50) {
	  mTailMotor.setBrake(false);
	  if(mTailMotor.getCount() > STANDING_ANGLE_2){
	    pwm_diff_2--;
	  } else {
	    pwm_diff_2++;
	  }
	}

	mTailMotor.setPWM(pwm_Tail + pwm_diff_2);
	// tail_control_with_PID(STANDING_ANGLE_2);
      }
      break;
    case TO_STABLE:
      mSpeaker.playTone(800, 10, 5);
      ++mStableCount;
      if(mStableCount > 500) {
	mSubState = STANDING_UP;
	mStableCount = 0;
      }
      break;
    case STANDING_UP:
      mSpeaker.playTone(700, 10, 5);
      tail_control_with_PID(0);      
      balance_control((float)command.mX,/* 前後進命令(+:前進, -:後進) */
		      (float)command.mY,/* 旋回命令(+:右旋回, -:左旋回) */
		      (float)ecrobot_get_gyro_sensor(NXT_PORT_S1),/* ジャイロセンサ値 */
		      (float)USER_GYRO_OFFSET,/* ジャイロセンサオフセット値 */
		      (float)nxt_motor_get_count(NXT_PORT_C),/* 左モータ回転角度[deg] */
		      (float)nxt_motor_get_count(NXT_PORT_B),/* 右モータ回転角度[deg] */
		      (float)ecrobot_get_battery_voltage(),/* バッテリ電圧[mV] */
		      &pwm_L,/* 左モータPWM出力値 */
		      &pwm_R);/* 右モータPWM出力値 */
      mLeftMotor.setPWM(pwm_L);
      mRightMotor.setPWM(pwm_R);
      mStableCount++;
      if(mStableCount > 500){
	mSubState = DONE;
      }
      break;
    case DONE:
      break;
    }
}

bool
StandUpSkill::isDone() const {
  static int counter = 0;
  counter++;
  return isStable() && counter > 500;
}

bool
StandUpSkill::isStandUp() const {
  // モータの角度が一定に達したら立ち上がったと判定する。
  return mTailMotor.getCount() > STANDING_ANGLE_2 - 2;
}

bool
StandUpSkill::isStable() const {
  return mStableCount > 500;
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
  // mTailMotor.setPWM(78);
}
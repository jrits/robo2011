#include "SitDownSkill.h"

extern "C" {
#include "balancer.h"
}

#include "Motor.h"
#include "Nxt.h"
#include "GyroSensor.h"
#include "constants.h"
#include "Vector.h"

using namespace ecrobot;

extern Motor mLeftMotor;
extern Motor mRightMotor;
extern Motor mTailMotor;
extern Nxt mNxt;
extern GyroSensor mGyroSensor;

SitDownSkill::SitDownSkill()
    : mSitDownAngle(100),
      mCurrentState(INIT)
{
}

SitDownSkill::~SitDownSkill(){
}

signed int
SitDownSkill::getAngle() const {
  return mSitDownAngle;
}

void
SitDownSkill::setAngle(signed int angle){
  mSitDownAngle = angle;
}

void
SitDownSkill::execute(){
  S8 pwmL = 0, pwmR=0;
  VectorT<float> command(0.0,0.0);

  switch(mCurrentState){
    case INIT:
      mCurrentState = STOP;
      break;
    case STOP:
      balance_control((F32)command.mX, // 前後進命令
                      (F32)command.mY,
                      (F32)mGyroSensor.get(),
                      (F32)USER_GYRO_OFFSET,
                      (F32)mLeftMotor.getCount(),
                      (F32)mRightMotor.getCount(),
                      (F32)mNxt.getBattMv(),
                      &pwmL,
                      &pwmR);
      if(isStable()){
        mCurrentState = INCLINED_BACKWARD;
      }
      break;
    case INCLINED_BACKWARD:
      balance_control((F32)command.mX, // 前後進命令
                      (F32)command.mY,
                      (F32)mGyroSensor.get(),
                      (F32)USER_GYRO_OFFSET-20,
                      (F32)mLeftMotor.getCount(),
                      (F32)mRightMotor.getCount(),
                      (F32)mNxt.getBattMv(),
                      &pwmL,
                      &pwmR);
      if(isInclined()){
        mCurrentState = SIT_DOWN;
      }
      break;
    case SIT_DOWN:
      pwmL = 0;
      pwmR = 0;
      if(isSeated()) {
        mCurrentState = DONE;
      }
      break;
    case DONE:
      pwmL = 0;
      pwmR = 0;
      break;
    default:
      break;
  }

  tail_control(mSitDownAngle);
  mLeftMotor.setPWM(pwmL);
  mRightMotor.setPWM(pwmR);
}

void
SitDownSkill::tail_control(signed int angle) const{
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

bool
SitDownSkill::isStable() const {
  static int count = 0;
  count++;
  return count >= 250;
}

bool
SitDownSkill::isInclined() const {
  static int count = 0;
  count++;
  return count >= 50;
}

bool
SitDownSkill::isSeated() const {
  static int count = 0;
  count++;
  return count >=250;
}

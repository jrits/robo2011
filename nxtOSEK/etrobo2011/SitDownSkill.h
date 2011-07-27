#ifndef SITDOWNSKILL_H
#define SITDOWNSKILL_H


extern "C" {
#include "balancer.h"
}

#include "Motor.h"
#include "Nxt.h"
#include "GyroSensor.h"
#include "constants.cpp"
#include "Vector.h"

using namespace ecrobot;

extern Motor mLeftMotor;
extern Motor mRightMotor;
extern Motor mTailMotor;
extern Nxt mNxt;
extern GyroSensor mGyroSensor;

class SitDownSkill {
 public:
  SitDownSkill(){}
  
  ~SitDownSkill(){}

  void execute(){
    S8 pwmL = 0, pwmR=0;

    VectorT<float> command(0.0,0.0);
    balance_control((F32)command.mX, // 前後進命令
		    (F32)command.mY,
		    (F32)mGyroSensor.get(),
		    (F32)USER_GYRO_OFFSET,
		    (F32)mLeftMotor.getCount(),
		    (F32)mRightMotor.getCount(),
		    (F32)mNxt.getBattMv(),
		    &pwmL,
		    &pwmR);
    tail_control(100);
    mLeftMotor.setPWM(pwmL + 10);
    mRightMotor.setPWM(pwmR + 10);
  }

 private:
  void tail_control(signed int angle) const{
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
};

#endif // SITDOWNSKILL_H

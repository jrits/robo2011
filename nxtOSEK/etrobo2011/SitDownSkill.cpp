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

void SitDownSkill::execute(){
  S8 pwmL = 0, pwmR=0;

  VectorT<float> command(0.0,0.0);
  balance_control((F32)command.mX, // Oãi½ß
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

void
SitDownSkill::tail_control(signed int angle) const{
  static const float P_GAIN = 2.5F;
  static const float PWM_ABS_MAX = 60; /* ®Sâ~p[^§äPWMâÎÅål */        
  float pwm = (float)(angle - mTailMotor.getCount())*P_GAIN; /* äá§ä */

  /* PWMoÍOa */
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

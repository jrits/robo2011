#include "Posture.h"

#include "Motor.h"

Posture::Posture(Motor& leftMotor,
                 Motor& rightMotor,
                 Motor& tailMotor)
    : mLeftMotor(leftMotor),
      mRightMotor(rightMotor),
      mTailMotor(tailMotor) {
  
}

Posture::~Posture(){
  
}

void Posture::inclineBackward(int angle){

  S8 PWM_ABS_MAX = 100;
  float P_GAIN = 2.5F;
  /* PWMo—Í–O˜aˆ— */
  float pwm = (float)(angle - mTailMotor.getCount())*P_GAIN; /* ”ä—á§Œä */
  if (pwm > PWM_ABS_MAX) {
    pwm = 100;
  } else if (pwm < -PWM_ABS_MAX) {
    pwm = -PWM_ABS_MAX;
  }
  mTailMotor.setPWM((S8)pwm);
  mTailMotor.setBrake(true);
  mLeftMotor.setPWM(10);
  mRightMotor.setPWM(10);    
}

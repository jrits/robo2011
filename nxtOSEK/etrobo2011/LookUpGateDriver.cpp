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
      // ���荞�ށB
      if(sitDown()){
        mLcd.clear();
        mLcd.putf("sn","IN_FRONT_OF_GATE");
        mLcd.disp();
        mCurrentSubSection = UNDER_GATE;
      }
      break;
    case UNDER_GATE:
      // 3�_�X�����s�B
      mTripodLineTrace.setForward(10);
      mTripodLineTrace.execute();
      tail_control(90);
      if(passedGate()){
        mLcd.clear();
        mLcd.putf("sn","UNDER_GATE");
        mLcd.disp();
        mCurrentSubSection = BEHIND_GATE;
      }
      break;
    case BEHIND_GATE:
      /*�����オ��B*/
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
      // assert����ׂ��B
      break;
  }

  return isDone();
}

bool
LookUpGateDriver::foundGate() const{
  static int count = 0;
  
  // �Ƃ肠�����A1�b�o�߂�����Q�[�g�����������Ƃɂ���B
  // TODO �Q�[�g�̌��m���@����������K�v����B
  count++;
  return count >= 250;
}

bool
LookUpGateDriver::passedGate() const{
  static int count = 0;
  // �Ƃ肠�����A1�b�o�߂�����Q�[�g��ʉ߂������Ƃɂ���B
  // TODO �Q�[�g�ʉ߂̌��m���@����������K�v����B
  count++;
  return count >= 250;
}

bool
LookUpGateDriver::sitDown() {
  static int count = 0;
  mSitDownSkill.setAngle(80);
  mSitDownSkill.execute();
  count++;

  // �Ƃ肠�����A2�b�o�߂���������Ă��邱�Ƃɂ���B
  // TODO �����Ă��邩�A�ۂ��̌��o���@���������K�v����B
  return count >= 1000;
}

bool
LookUpGateDriver::standUp() {
  static int count = 0;
  mStandUpSkill.execute();
  count++;
  
  // �Ƃ肠�����A2�b�o�߂����痧���Ă��邱�Ƃɂ���B
  // TODO ������������A�ۂ��̌��o���@���������K�v����B
  return count >= 500;
}

bool
LookUpGateDriver::isDone() const{
  return mCurrentSubSection == DONE;
}

void
LookUpGateDriver::tail_control(signed int angle) const {
  static const float P_GAIN = 2.5F;
  static const float PWM_ABS_MAX = 60; /* ���S��~�p���[�^����PWM��΍ő�l */        
  float pwm = (float)(angle - mTailMotor.getCount())*P_GAIN; /* ��ᐧ�� */

  /* PWM�o�͖O�a���� */
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

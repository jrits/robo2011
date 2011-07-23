//
// TripodLineTrace.h
//

#ifndef TripodLineTrace_H_
#define TripodLineTrace_H_

#include "TripodSkill.h"

/**
 * ���C���g���[�X�X�L��
 */
class TripodLineTrace : public TripodSkill
{
private:
  float mBlack;         //!< ��
  float mWhite;         //!< ��
  float mLineThreshold; //!< ����臒l
  int mInitForward;     //!< �����t�H���[�h�l
  int mInitDuration;    //!< �����t�H���[�h�l�p���J�E���^����
  int mTimeCounter;     //!< ���ԃJ�E���^
  bool mUseOnoff;       //!< ON/OFF����(true) or PID����(false)�t���O
  //Pid mLightPid(LIGHT_PID_KP, LIGHT_PID_KI, LIGHT_PID_KD);
  //float LIGHT_ONOFF_K;
public:
  TripodLineTrace(float black, float white, float threshold);
  ~TripodLineTrace(){}
  void reset();
  void setInitForward(int initForward);
  void setInitDuration(int initDuration);
  void setUseOnoff(bool useOnoff);
public:
  VectorT<float> calcCommand();
public:
  float calcCommandTurnByOnOff();
  float calcCommandTurn();
  float lightValueNormalization();
};
#endif

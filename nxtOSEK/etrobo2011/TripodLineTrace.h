//
// TripodLineTrace.h
//

#ifndef TripodLineTrace_H_
#define TripodLineTrace_H_

#include "TripodSkill.h"

/**
 * ライントレーススキル
 */
class TripodLineTrace : public TripodSkill
{
private:
  float mBlack;         //!< 黒
  float mWhite;         //!< 白
  float mLineThreshold; //!< 白黒閾値
  int mInitForward;     //!< 初期フォワード値
  int mInitDuration;    //!< 初期フォワード値継続カウンタ期間
  int mTimeCounter;     //!< 時間カウンタ
  bool mUseOnoff;       //!< ON/OFF制御(true) or PID制御(false)フラグ
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

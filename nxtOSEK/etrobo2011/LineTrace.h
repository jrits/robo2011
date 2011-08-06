//
// LineTrace.h
//

#ifndef LINETRACE_H_
#define LINETRACE_H_

#include "Skill.h"

/**
 * ライントレーススキル
 */
class LineTrace : public Skill
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
	LineTrace(float black, float white, float threshold);
	~LineTrace(){}
    void reset();
    void setInitForward(int initForward);
    void setInitDuration(int initDuration);
public:
    void execute();
    VectorT<float> calcCommand();
    float calcCommandTurn();
	float lightValueNormalization();
public:
    void setUseOnoff(bool useOnoff);
    float calcCommandTurnByOnOff();
};
#endif

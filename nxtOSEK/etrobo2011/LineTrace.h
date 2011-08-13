//
// LineTrace.h
//

#ifndef LINETRACE_H_
#define LINETRACE_H_

#include "Skill.h"
#include "History.h"

#define TURN_HISTORY_BUFF_SIZE 10

/**
 * ライントレーススキル
 */
class LineTrace : public Skill
{
protected:
    float mBlack;         //!< 黒
    float mWhite;         //!< 白
    float mLineThreshold; //!< 白黒閾値
    int mInitForward;     //!< 初期フォワード値
    int mInitDuration;    //!< 初期フォワード値継続カウンタ期間
    int mTimeCounter;     //!< 時間カウンタ
    //Pid mLightPid(LIGHT_PID_KP, LIGHT_PID_KI, LIGHT_PID_KD);
    //float LIGHT_ONOFF_K;
    bool mDoOnOffTrace;   //!< あえてON/OFF制御ライントレースをする
public:
    LineTrace(float black, float white, float threshold);
    ~LineTrace(){}
    void reset();
    void setInitForward(int initForward);
    void setInitDuration(int initDuration);
public:
    void setDoOnOffTrace(bool doOnOffTrace);
public:
    VectorT<float> calcCommand();
    float calcCommandTurn();
    float lightValueNormalization();
    float maimaiValueNormalization();
    float calcOnOffCommandTurn();
};
#endif

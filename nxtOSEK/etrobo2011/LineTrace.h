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
    float mMaimaiBlack;         //!< 黒(MAIMAI)
    float mMaimaiWhite;         //!< 白(MAIMAI)
    float mMaimaiLineThreshold; //!< 白黒閾値(MAIMAI)
    int mInitForward;     //!< 初期フォワード値
    int mInitDuration;    //!< 初期フォワード値継続カウンタ期間
    int mTimeCounter;     //!< 時間カウンタ
    //Pid mLightPid(LIGHT_PID_KP, LIGHT_PID_KI, LIGHT_PID_KD);
    //float LIGHT_ONOFF_K;
    bool mDoOnOffTrace;   //!< あえてON/OFF制御ライントレースをする
    float mTemp_K_THETADOT;
    float mTemp_K_PHIDOT;
public:
    LineTrace(float black, float white, float threshold, 
        float maimaiBlack, float maimaiWhite, float maimaiThreshold);
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
    float calcOnOffCommandTurn();
};
#endif

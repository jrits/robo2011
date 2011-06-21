#ifndef _PIDCONTROLLER_H_
#define _PIDCONTROLLER_H_

#include "mytypes.h"
#include "ecrobot_types.h"
#include "Vector.h"
#include "parameters.hpp"
#include "Color.hpp"

//extern Color color;

/** 
 * PID制御。
 */
class PidController
{

protected:
    float kp; ///< PID制御の比例ゲイン。
    float ki; ///< PID制御の積分ゲイン。
    float kd; ///< PID制御の微分ゲイン。
    static const int MAX_SIZE = 50; ///< 微分ゲインを取得するためのバッファサイズ
    int size;
public:
    /**
     * コンストラクタ.
     *
     * @param kp PID制御の比例ゲイン。
     * @param ki PID制御の積分ゲイン。
     * @param kd PID制御の微分ゲイン。
     */
    PidController(float pKp, float pKi, float pKd, int pSize = 10)
        :
        kp(pKp),
        ki(pKi),
        kd(pKd),
        size(pSize)
    {
    };
    /**
     * デストラクタ
     */
    virtual ~PidController(){};
public:
    inline void setKp(float pKP) {this->kp = pKP;};
    inline void setKi(float pKI) {this->ki = pKI;};
    inline void setKd(float pKD) {this->kd = pKD;};
    inline void setSize(int pSize) {this->size = pSize;};
    inline float getKp() const {return this->kp;};
    inline float getKi() const {return this->ki;};
    inline float getKd() const {return this->kd;};
    inline int   getSize() const {return this->size;};
public:
    /**
     * PID制御
     *
     * @param current 現在の観測値(出力値)
     * @param reference 目標観測値
     * @return フィードバック入力値
     */
    virtual float control(float current, float reference);
};

#endif

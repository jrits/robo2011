//
// Skill.h
//
#ifndef SKILL_H_
#define SKILL_H_

#include "constants.h"
#include "Activator.h"
extern Activator mActivator;
extern bool gDoForwardPid;

/**
 * スキル抽象クラス
 *
 * このクラスのオブジェクトは生成されない
 */
class Skill
{
protected:
    float mForward; //!< フォワード値
public:
    /**
     * コンストラクタ
     */
    Skill(){}
    /**
     * デストラクタ
     */
    virtual ~Skill(){}
    /**
     * フォワード値をセットする
     *
     * @param[in] forward フォワード値
     */
    void setForward(float forward)
    {
        mForward = forward;
    }
    /**
     * スキルを発揮してロボットを動かす。
     *
     * calcCommand を用いて走行ベクトルを決定し、制御機器(Activator)を操作する。
     */
    virtual void execute()
    {
        VectorT<float> command = calcCommand();
        mActivator.run(command);//制御機器にセット
    }
    /**
     * 走行ベクトルを計算
     *
     * Override Me!!
     */
    virtual VectorT<float> calcCommand() { return VectorT<float>(0,0); }
    // 純粋仮想関数にするとプログラム容量が肥大化する。とりあえず純粋仮想関数を避ける。
};

#endif

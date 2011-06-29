//
// Driver.h
//
#ifndef DRIVER_H_
#define DRIVER_H_

#include "constants.h"
#include "Skill.h"

/**
 * ドライバの抽象クラス、このクラスのオブジェクトは生成されない
 */
class Driver
{
protected:
    int mState; //!< 現在状態
    Skill* mSkill; //!< 現在ドライバ
public:
    /**
     * コンストラクタ
     */
	Driver(){}
    /**
     * デストラクタ
     */
	virtual ~Driver(){}
    /**
     * 運転する。
     * 
     * 現在の状況にあわせて適切なスキルを選択し、スキルを発揮することでロボットを運転する。
     *
     * @retval true 最終状態。これ以降の状態遷移なし。
     * @retval false まだ役目が完了していません。
     *
     * Override Me!
     */
	virtual bool drive() { return true; }
};
#endif

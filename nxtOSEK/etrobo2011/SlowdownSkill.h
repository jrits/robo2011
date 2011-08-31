//
// SlowdownSkill.h
//

#ifndef SlowdownSkill_H_
#define SlowdownSkill_H_

#include "Skill.h"
#include "struct.h"

/**
 * 減速スキル
 *
 * スキルに減速機構を追加する Decorator クラス。
 */
class SlowdownSkill : public Skill
{
    Skill *mSkill;         //!< 減速機構を追加するスキル。
    float mMinimumForward; //!< 減速最低速度(デフォルト10)
    float mTargetDistance; //!< 停止目標距離(mm)
    float mAllowableError; //!< 許容エラー値(mm)
public:
	SlowdownSkill();
	~SlowdownSkill(){}
public:
    void setSkill(Skill *skill);
    void setTargetDistance(float targetDistance);
    void setMinimumForward(float minimumForward);
	void setAllowableError(float allowableError);
    bool isArrived();
    bool isPassed();
public:
	VectorT<float> calcCommand();
};
#endif

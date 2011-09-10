//
// StopSkill.h
//

#ifndef StopSkill_H_
#define StopSkill_H_

#include "Skill.h"
#include "struct.h"

/**
 * 急停止スキル。目標地点で速度を急に0に落とす。
 *
 * スキルに急停止機構を追加する Decorator クラス。
 */
class StopSkill : public Skill
{
    Skill *mSkill;         //!< 急停止機構を追加するスキル。
    float mTargetDistance; //!< 停止目標距離(mm)
    float mAllowableError; //!< 許容エラー値(mm)。何mm手前で速度を0に落とすか。
public:
    StopSkill();
    ~StopSkill(){}
public:
    void setSkill(Skill *skill);
    void setTargetDistance(float targetDistance);
    void setAllowableError(float allowableError);
    bool isArrived();
public:
    VectorT<float> calcCommand();
};
#endif

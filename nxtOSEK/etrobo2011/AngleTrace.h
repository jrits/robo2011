//
// AngleTrace.h
//

#ifndef ANGLETRACE_H_
#define ANGLETRACE_H_

#include "Skill.h"

/**
 * 角度トレーススキル
 */
class AngleTrace : public Skill
{
private:
	float mTargetAngle; //!< 目標角度
    float mAllowableError; //!< 許容誤差角度。方向転換に使う。 [0, 180]
public:
	AngleTrace();
	~AngleTrace(){}
	void setTargetAngle(float targetAangle);
    void setAllowableError(float allowableError);
public:
	VectorT<float> calcCommand();
    bool isArrived();
};
#endif

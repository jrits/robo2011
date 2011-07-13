//
// CoordinateTrace.h
//

#ifndef COORDINATETRACE_H_
#define COORDINATETRACE_H_

#include "Skill.h"
#include "struct.h"

/**
 * 座標指定走行スキル
 */
class CoordinateTrace : public Skill
{
private:
	Point mTargetCoordinate; //!< 目標座標
    float mAllowableError;   //!< 許容エラー値(mm)
public:
	CoordinateTrace();
	~CoordinateTrace(){}
	void setTargetCoordinate(Point targetCoordinate);
	void setAllowableError(float allowableError);
public:
	VectorT<float> calcCommand();
public:
    bool isArrived();
};
#endif

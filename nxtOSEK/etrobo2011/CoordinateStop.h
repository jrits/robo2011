//
// CoordinateStop.h
//

#ifndef COORDINATESTOP_H_
#define COORDINATESTOP_H_

#include "Skill.h"
#include "struct.h"

/**
 * 座標指定着地
 */
class CoordinateStop : public Skill
{
private:
    Point mTargetCoordinate; //!< 目標座標
    float mSlowdownDistance; //!< 目標座標の何mm手前から着地動作を始めるか
private:
    float mTargetDistance;   //!< 停止目標距離
public:
    CoordinateStop();
    ~CoordinateStop(){}
    void setTargetCoordinate(Point targetCoordinate);
    void setSlowdownDistance(float slowdownDistance);
public:
    void setTargetDistance(float targetDistance);
protected:
    VectorT<float> calcCommand();
};
#endif

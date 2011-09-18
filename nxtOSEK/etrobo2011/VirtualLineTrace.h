//
// VirtualLineTrace.h
//

#ifndef VirtualLineTrace_H_
#define VirtualLineTrace_H_

#include "Skill.h"
#include "struct.h"

/**
 * 仮想ライントレーススキル
 */
class VirtualLineTrace : public Skill
{
private:
    ControlPoint *mControlPoints; //!< 制御点セット
    int mNumOfControlPoints; //!< 制御点の数
    int mCurrentIndex; //!< 現在制御点インデックス
    float mXoffset; //!< 大会のための調整用変数
    float mYoffset; //!< 大会のための調整用変数
public:
    VirtualLineTrace();
    ~VirtualLineTrace(){}
    void setControlPoints(ControlPoint *controlPoints, int numOfControlPoints);
    void reset();
public:
    VectorT<float> calcCommand();
    bool isLast();
};
#endif

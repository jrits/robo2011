//
// Course.h
//
#ifndef COURSE_H_
#define COURSE_H_

#include "constants.h"
#include "Driver.h"

/**
 * 抽象コース(ナビゲータ)クラス
 */
class Course
{
protected:
    int mNameId; //!< インコースかアウトコースか
    int mState; //!< 現在状態
    Driver *mDriver; //!< 現在ドライバ
public:
    /**
     * コンストラクタ
     */
    Course(){}
    /**
     * デストラクタ
     */
    virtual ~Course(){}
    /**
     * 現在の状況に適切なドライバを選択し、ドライバに運転させる
     *
     * Override Me!
     */
    virtual void drive(){}
    int getNameId() { return mNameId; }
    int getState() { return mState; }
};

#endif /*COURSE_H_*/

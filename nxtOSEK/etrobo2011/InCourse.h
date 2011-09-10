//
// InCourse.h
//
#ifndef INCOURSE_H_
#define INCOURSE_H_

#include "Course.h"

/**
 * インコース
 */
class InCourse : public Course
{
public:
    /**
     * インコースの区間
     */
    enum eSection {
        TESTDRIVE, //!< テストドライバ起動
        START,     //!< スタート区間
    	SEESAW,    //!< シーソー区間
        STAIRWAY,  //!< 階段区間
        GARAGEIN   //!< ガレージ・イン区間
    };
public:
    InCourse(InCourse::eSection aSection = InCourse::START);
    ~InCourse(){}
    void drive();
};
    
#endif /*INCOURSE_H_*/

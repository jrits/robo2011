//
// OutCourse.h
//
#ifndef OUTCOURSE_H_
#define OUTCOURSE_H_

#include "Course.h"

/**
 * アウトコース
 */
class OutCourse : public Course
{
public:
    /**
     * アウトコースの区間
     */
    enum eSection {
        TESTDRIVE, //!< テストドライバ起動
        START,     //!< スタート区間
        SEESAW,    //!< シーソー区間
        STAIRWAY,  //!< 階段区間
        ETSUMO,    //!< ET相撲区間 ETロボコン2011 追記
        GARAGEIN   //!< ガレージ・イン区間
    };
public:
 	OutCourse(OutCourse::eSection aState = OutCourse::START);
	~OutCourse(){}
    void drive();
};
	
#endif /*OUTCOURSE_H_*/

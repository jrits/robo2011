#ifndef INDOLPHIN_COURSE_H_
#define INDOLPHIN_COURSE_H_

#include "Course.hpp"
#include "TwinloopSection.hpp"
#include "DolphinSection.hpp"

extern DolphinSection dolphinSection;
extern TwinloopSection twinloopSection;

class InDolphinCourse : public Course
{
public:
    enum SectionId {
        START,
        DOLPHIN,
        AFTER_DOLPHIN,
        BEFORE_TWINLOOP,
        TWINLOOP,
        AFTER_TWINLOOP,
        AFTER_FIRST360,
        END
    };
public:
    InDolphinCourse(Direction &g, Distance &d, Time &t,
              SectionId si = InDolphinCourse::START,
              CourseId ci = Course::IN
        ) : Course(g, d, t, ci) {
        this->sectionId = si;
    };
    ~InDolphinCourse() {};

public:
    Section* detect();

};

#endif // INDOLPHIN_COURSE_H_


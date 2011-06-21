#ifndef COURSE_H_
#define COURSE_H_

#include "Direction.hpp"
#include "Time.hpp"
#include "Distance.hpp"
#include "parameters.hpp"
#include <cmath>

#include "NormalLineSection.hpp"
#include "GoalStopSection.hpp"
#include "NewGoalStopSection.hpp"
#include "DashedLineSection.hpp"
#include "NewDashedLineSection.hpp"
#include "TreasureHuntSection.hpp"
#include "NewTreasureHuntSection.hpp"
#include "TestSection.hpp"

extern NormalLineSection normalLineSection; // ToDo: no more global
extern GoalStopSection goalStopSection;
extern DashedLineSection dashedLineSection;
extern TreasureHuntSection treasureHuntSection;
extern NewDashedLineSection newDashedLineSection;
extern NewTreasureHuntSection newTreasureHuntSection;
extern NewGoalStopSection newGoalStopSection;

/**
 * Section (Marker Section) Detector
 */
class Course
{
public:
    enum CourseId {
        IN,
        OUT
    };
    
protected:
    CourseId  courseId;
    int       sectionId;
    Direction &direction;
    Distance  &distance;
    Time      &time;
    bool      isDashedLineSectionFinished;
    bool      isTreasureHuntSectionFinished;
    Lcd lcd;

public:
    Course(Direction &g, Distance &d, Time &t,
           CourseId courseId
        ):
        direction(g),
        distance(d),
        time(t)
        {
            this->courseId = courseId;
            this->isDashedLineSectionFinished = false;
            this->isTreasureHuntSectionFinished = false;
        };

    ~Course() {};

public:
    inline CourseId getCourseId() { return this->courseId; } 
    inline int getSectionId() { return this->sectionId; }
    inline void finishDashedLineSection(){isDashedLineSectionFinished = true;}
    inline void finishTreasureHuntSection(){isTreasureHuntSectionFinished = true;}
    Section* detect();
    
protected:
    void countFlag(int &flag, bool cond)
    {
        if ( cond ) {
            flag++;
        } else {
            flag--;
        }
        if (flag < 0) flag = 0;
    };
};

#endif // COURSE_H_


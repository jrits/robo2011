#ifndef IN_COURSE_H_
#define IN_COURSE_H_

#include "Course.hpp"

/**
 * Course Manager
 *
 * @todo In Course would be an object of a course. 
 */
class InCourse : public Course
{
public:
    enum SectionId {
        START,
        FIRST360,
        AFTER_FIRST360,
        END
    };

public:
    InCourse(Direction &g, Distance &d, Time &t,
              SectionId si = InCourse::START,
              CourseId ci = Course::IN
        ) : Course(g, d, t, ci) {
        this->sectionId = si;
    };
    ~InCourse() {};

public:
    Section* detect()
    {
        int nextSectionId = this->sectionId; ///<今回決定する状態。とりあえず前回と同じ状態に
        Section* nextSection = 0;
        static int flagCount = 0;

        static int globalInit = false;
        static int endInit   = false;

        /// 車体向き認識の調整
        if (this->sectionId == InCourse::START) {
            direction.calcDirectionOffset90();
        }
        if (this->sectionId == InCourse::START) {
            direction.calcDirectionOffset180();
        }

        F32 newAvgDirectionOff = direction.getNewAvgDirectionOff();
        F32 derivativeOfDirection = direction.getDerivativeOfDirection();

#if PRINT_STATE
        F32 directionOff = direction.getDirectionOff();
        lcd.clear();
        lcd.putf("sdn", "Raw", (S32)directionOff);
        lcd.putf("sdn", "Avg", (S32)newAvgDirectionOff);
        lcd.putf("sdn", "SID", this->sectionId);
        lcd.putf("sdn", "Cnt", flagCount);
        lcd.disp();
#endif

        if (globalInit) {
            normalLineSection.setEdge(RIGHT);
            normalLineSection.setSpeed(SPEED);
            globalInit = false;
        }

        if (this->sectionId == InCourse::START) {
            nextSection = &normalLineSection;

            // ２度目の360度検出でゴール前直線となるため、１度目は別の状態へ
            countFlag(flagCount,newAvgDirectionOff > GOAL_STOP_STRATEGY_TRIGER_DEGREE);
            if (flagCount >= 20) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InCourse::FIRST360;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InCourse::FIRST360) {
            nextSection = &normalLineSection;

            // 一度目の360度状態を抜ける
            countFlag(flagCount,newAvgDirectionOff < 210);
            if (flagCount >= 20) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InCourse::AFTER_FIRST360;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InCourse::AFTER_FIRST360) {
            nextSection = &normalLineSection;

            // ２度目の360度直線検出でゴール前直線へ
            countFlag(flagCount, newAvgDirectionOff > GOAL_STOP_STRATEGY_TRIGER_DEGREE
                      && std::abs(derivativeOfDirection) < GOAL_STOP_STRATEGY_TRIGER_DEGREE_DERIVATIVE_BOUNDS);
            if (flagCount >= GOAL_STOP_STRATEGY_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
                beep();
#endif
                nextSectionId = InCourse::END;
                distance.reset();
                flagCount = 0;
            }
        } else if (this->sectionId == InCourse::END) {
            if (endInit == false) {
                goalStopSection.initialize();
                goalStopSection.setEdge(RIGHT);
                goalStopSection.setDistanceToGoal(IN_DISTANCE_TO_GOAL);
                goalStopSection.setSpeed(GOAL_STOP_STRATEGY_SPEED);
                endInit = true;
            }
            nextSection = &goalStopSection;
        }

        this->sectionId = nextSectionId;
        return nextSection;
    };
    
};

#endif // IN_COURSE_H_


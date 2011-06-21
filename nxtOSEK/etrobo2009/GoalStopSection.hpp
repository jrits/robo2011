#ifndef _GOAL_STOP_STRATEGY_H_
#define _GOAL_STOP_STRATEGY_H_

#include "NormalLineSection.hpp"
#include "Driver.hpp"
#include "SensorHistory.hpp"
#include "Direction.hpp"

class GoalStopSection  : public Section
{
private:
    F32 distanceToGoal;///< 最後のマーカからゴールまでの走行距離。
    
public:
    GoalStopSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
    };
    ~GoalStopSection(){};
    void inline initialize(){};
private:
    VectorT<S8> computeRunningVelocity();
    VectorT<S8> computeStopVelocity(F32 refDirection);
    VectorT<S8> computeRebootVelocity(F32 refDirection);
public:
    bool execute();
    void inline setDistanceToGoal(F32 dist) {
        this->distanceToGoal = dist;
    }
    // obsolete
    void inline setEdge(EDGE edge) {
        lineTraceDriver.setEdge(edge);
    };
    void inline setSpeed(S8 speed) {
        lineTraceDriver.setSpeed(speed);
    }
};

#endif /* _GOAL_STOP_STRATEGY_H_ */

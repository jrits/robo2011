#ifndef _NEW_DASHED_LINE_STRATEGY_H_
#define _NEW_DASHED_LINE_STRATEGY_H_

#include "NormalLineSection.hpp"
#include "Distance.hpp"
#include "Direction.hpp"
#include "parameters.hpp"


class NewDashedLineSection : public Section
{
private:
public:
    NewDashedLineSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
    };
    ~NewDashedLineSection(){};
    void inline initialize(){};
public:
    bool execute();
};

#endif /* _NEW_DASHED_LINE_STRATEGY_H_ */

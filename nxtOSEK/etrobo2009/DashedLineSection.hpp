#ifndef _DASHED_LINE_STRATEGY_H_
#define _DASHED_LINE_STRATEGY_H_

#include "NormalLineSection.hpp"
#include "Distance.hpp"
#include "Direction.hpp"
#include "parameters.hpp"


class DashedLineSection : public Section
{
private:
public:
    DashedLineSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
    };
    ~DashedLineSection(){};
    void inline initialize(){};
public:
    bool execute();
};

#endif /* _DASHED_LINE_STRATEGY_H_ */

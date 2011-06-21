#ifndef _TWINLOOP_STRATEGY_H_
#define _TWINLOOP_STRATEGY_H_

#include "Section.hpp"
#include "parameters.hpp"

class TwinloopSection : public Section
{
private:
public:
    TwinloopSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
    };
    virtual ~TwinloopSection(){};
    void inline initialize(){};
public:
    bool execute();
};

#endif /* _TWINLOOP_STRATEGY_H_ */

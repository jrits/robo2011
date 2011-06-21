#ifndef _DOLPHIN_STRATEGY_H_
#define _DOLPHIN_STRATEGY_H_

#include "NormalLineSection.hpp"
#include "Distance.hpp"
#include "Direction.hpp"
#include "parameters.hpp"


class DolphinSection : public Section
{
private:
public:
    DolphinSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
    };
    ~DolphinSection(){};
    void inline initialize(){};
public:
    bool execute();
};

#endif /* _DOLPHIN_STRATEGY_H_ */

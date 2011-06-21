#ifndef _TREASURE_HUNT_STRATEGY_H_
#define _TREASURE_HUNT_STRATEGY_H_

#include "Section.hpp"
#include "parameters.hpp"

class TreasureHuntSection : public Section
{
private:
public:
    TreasureHuntSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
    };
    virtual ~TreasureHuntSection(){};
    void inline initialize(){};
public:
    bool execute();
};

#endif /* _TREASURE_HUNT_STRATEGY_H_ */

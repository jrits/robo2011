#ifndef _NEWTREASUREHUNTSECTION_H_
#define _NEWTREASUREHUNTSECTION_H_

#include "Section.hpp"
#include "parameters.hpp"

class NewTreasureHuntSection : public Section
{
private:
public:
    NewTreasureHuntSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
    };
    virtual ~NewTreasureHuntSection(){};
    void inline initialize(){};
public:
    bool execute();
};


#endif /* _NEWTREASUREHUNTSECTION_H_ */

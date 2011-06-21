#ifndef NORMAL_LINE_STRATEGY_H_
#define NORMAL_LINE_STRATEGY_H_

#include "Section.hpp"
#include "Driver.hpp"
#include "parameters.hpp"

class NormalLineSection : public Section
{
private:
    int state;
    S8  bootSpeed;
    S32 bootTime;
    EDGE bootEdge;
    S8  speed;
    EDGE edge;
public:
    NormalLineSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh) {
        initialize();
    };
    ~NormalLineSection(){};
    void inline initialize(){
        state = -1;
        bootSpeed = START_BOOT_SPEED;
        bootTime  = START_BOOT_TIME;
        speed = SPEED;
        bootEdge = TRACED_EDGE;
        edge = TRACED_EDGE;
    };
public:
    bool execute();
    void inline setBootEdge(EDGE edge) {
        this->bootEdge = edge;
    }
    void inline setEdge(EDGE edge) {
        this->edge = edge;
    };
    void inline setBootSpeed(S8 bootSpeed) {
        this->bootSpeed = bootSpeed;
    }
    void inline setBootTime(S32 bootTime) {
        this->bootTime = bootTime;
    }
    void inline setSpeed(S8 speed) {
        this->speed = speed;
        lineTraceDriver.setSpeed(speed);
    }
    void inline switchEdge() {
        lineTraceDriver.switchEdge();
    }
};

#endif /* NORMAL_LINE_STRATEGY_H_ */

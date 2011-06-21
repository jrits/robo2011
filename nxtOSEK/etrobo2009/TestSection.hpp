#ifndef _TEST_STRATEGY_H_
#define _TEST_STRATEGY_H_

#include "Section.hpp"
#include "Distance.hpp"
#include "Direction.hpp"
#include "parameters.hpp"

#define TEST_DIRECTION 1
#define TEST_CURVATURE 2
#define TEST_LINETRACE 3
#define TEST_DDIRECTION 4
#define TEST_FIXED 5
#define TEST TEST_CURVATURE

extern LightSensor light;
extern PidController lightPid;
extern PidController anglePid;
extern PidController curvPid;

class TestSection : public Section
{
public:
    TestSection(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, 
                 DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
        Section(b,d,g,lp,ap,dap,c,sh)
        {};
    virtual ~TestSection(){};

    bool execute() {
        VectorT<S8> velocity;
#if TEST == TEST_DIRECTION
        light.setLamp(false);
        velocity = directionPidDriver.computeVelocity(0.0);
        velocity.mX = 60;
#elif TEST == TEST_CURVATURE
        light.setLamp(false);
        velocity = curvaturePidDriver.computeVelocity(-1.0/30.0);
        velocity.mX = 50;
#elif TEST == TEST_LINETRACE
        velocity = lineTraceDriver.computeVelocity(WHITE_GRAY_THRESH);
        velocity.mX = 60;
#elif TEST == TEST_DDIRECTION
        light.setLamp(false);
        velocity = curvaturePidDriver.computeVelocity(-0.1);
        velocity.mX = 50;
#elif TEST == TEST_FIXED
        light.setLamp(false);
        velocity.mY = 35;
        velocity.mX = 50;
#endif
#if TORQUE_MOTOR
        bRun.run(velocity);
#endif
        return false;
    }
};

#endif /* _TEST_STRATEGY_H_ */

#ifndef _BALANCINGRUN_H_
#define _BALANCINGRUN_H_

#include "Balancer.hpp"

class BalancingRun
{
private:

    Balancer &balancer;
    Motor &motorL, &motorR;

public:
    BalancingRun(Balancer &b, Motor &mL, Motor &mR):
        balancer(b),
        motorL(mL),
        motorR(mR)
        {};
    
    ~BalancingRun(){};

    void run(VectorT<S8> &direction);

    void brake(){motorL.setBrake(true); motorR.setBrake(true);};
};

#endif /* _BALANCINGRUN_H_ */

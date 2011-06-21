#include "BalancingRun.hpp"

void BalancingRun::run(VectorT<S8> &direction){
    S8 pwmL, pwmR;
    
    balancer.balance(direction,pwmL,pwmR);

    motorL.setPWM(pwmL);
    motorR.setPWM(pwmR);
};

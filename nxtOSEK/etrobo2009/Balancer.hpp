#ifndef _BALANCE_H_
#define _BALANCE_H_

#include "Vector.h"
#include "SensorHistory.hpp"
#include "parameters.hpp"

extern "C"
{
    #include "balancer.h"
}

class Balancer
{
private:
    SensorHistory &history;

public:
    Balancer(SensorHistory &sh):
        history(sh){};

    ~Balancer(){};

    void initialize();
    void balance(VectorT<S8> &direction, S8 &pwmL, S8 &pwmR);
};

#endif /* _BALANCE_H_ */

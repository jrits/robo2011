#ifndef LOOKUPGATEDRIVER_H
#define LOOKUPGATEDRIVER_H

#include "Driver.h"

class LookUpGateDriver : public Driver {
public:
    LookUpGateDriver();
    ~LookUpGateDriver();
    bool drive();
    
private:
    bool isDoFindGate();
    bool isGatePassed();
    bool isGateFound();
    bool isSitDowned();
    bool isStandUped();
    bool isDone();

    enum eSubSection {
        INIT = -1,
        BEFORELINETRACE,
        IN_FRONT_OF_GATE,
        UNDER_GATE,
        BEHIND_GATE,
        DONE
    };

    eSubSection mCurrentSubSection;
};

#endif

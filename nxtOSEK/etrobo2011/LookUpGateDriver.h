#ifndef LOOKUPGATEDRIVER_H
#define LOOKUPGATEDRIVER_H

#include "Driver.h"

class LookUpGateDriver : public Driver {
 public:
  LookUpGateDriver();
  ~LookUpGateDriver();
  bool drive();

 private:

  bool foundGate() const;
  bool passedGate() const;
  bool sitDown();
  bool standUp();
  bool isDone() const;

  void tail_control(signed int angle) const;
  
  enum eSubSection {
    INIT = 0,
    IN_FRONT_OF_GATE,
    UNDER_GATE,
    BEHIND_GATE,
    DONE
  };

  eSubSection mCurrentSubSection;
};

#endif

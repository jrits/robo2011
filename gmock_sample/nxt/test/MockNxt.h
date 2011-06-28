#ifndef _MOCKNXT_H_
#define _MOCKNXT_H_

#include "NxtIF.h"

typedef short S16;

class MockNxt : public NxtIF {
 public:
  MOCK_METHOD0(getButtons, eButton());
  MOCK_METHOD0(getBattMv, S16());
};

#endif /* _MOCKNXT_H_ */

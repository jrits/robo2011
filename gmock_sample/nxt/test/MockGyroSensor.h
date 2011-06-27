#ifndef _MOCKGYROSENSOR_H_
#define _MOCKGYROSENSOR_H_

#include "GyroSensorIF.h"

typedef short S16;

class MockGyroSensor : public GyroSensorIF {
 public:
  MOCK_METHOD0(get, S16());
  MOCK_METHOD0(getOffset, S16());
  MOCK_METHOD1(setOffset, void(S16 offset));
};

#endif /* _MOCKGYROSENSOR_H_ */

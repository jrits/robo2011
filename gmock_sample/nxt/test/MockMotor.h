#ifndef _MOCKMOTOR_H_
#define _MOCKMOTOR_H_

#include "MotorIF.h"

typedef char S8;
typedef int S32;

class MockMotor : public MotorIF {
 public:
  MOCK_METHOD1(setCount, void(S32 count));
  MOCK_METHOD0(getCount, S32());
  MOCK_METHOD1(setPWM, void(S8 pwm));
  MOCK_METHOD1(setBrake, void(bool brake));
  MOCK_METHOD0(reset, void());
};

#endif /* _MOCKMOTOR_H_ */

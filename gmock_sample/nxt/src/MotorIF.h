#ifndef MOTORIF_H
#define MOTORIF_H

typedef char S8;
typedef int S32;

class MotorIF {
 public:
  virtual ~MotorIF(){};
  virtual void setCount(S32 count) = 0;
  virtual S32 getCount() = 0;
  virtual void setPWM(S8 pwm) = 0;
  virtual void setBrake(bool brake) = 0;
  virtual void reset() = 0;
};

#endif

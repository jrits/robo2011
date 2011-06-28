#ifndef GYROSENSORIF_H
#define GYROSENSORIF_H

typedef short S16;

class GyroSensorIF
{
 public:
  virtual ~GyroSensorIF(){};
  virtual S16 get() = 0;
  virtual S16 getOffset() = 0;
  virtual void setOffset(S16 offset) = 0;
};

#endif

#ifndef ACTIVATOR_H
#define ACTIVATOR_H

#include "Vector.h"

class MotorIF;
class GyroSensorIF;
class NxtIF;

typedef float F32;

class Activator {
 public:
  Activator(MotorIF &leftMotor, 
            MotorIF &rightMotor, 
            GyroSensorIF &gyroSensor, 
            NxtIF &nxt);

  ~Activator(){}

  // 実装したいメソッド。
  void run(VectorT<F32> command);

 private:
  MotorIF &mLeftMotor; //!< 左モータ
  MotorIF &mRightMotor; //!< 右モータ
  GyroSensorIF &mGyroSensor; //!< ジャイロセンサ
  NxtIF &mNxt; //!< NXTオブジェクト
  int mGyroOffset; //!< ジャイロセンサオフセット値
};

#endif

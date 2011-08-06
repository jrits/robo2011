#ifndef POSTURE_H
#define POSTURE_H

#include "Motor.h"
using namespace ecrobot;


class Posture {
 public:
  Posture(Motor& leftMotor,
          Motor& rightMotor,
          Motor& tailMotor);

  ~Posture();

  /*! 走行体の筐体を後ろに指定した角度傾ける。

    走行体を後ろに傾け、指定した角度に体制を保つ。
    
    @param angle 後ろに筐体を傾ける角度。
  */
  void inclineBackward(int angle);

 private:
  Motor& mLeftMotor;
  Motor& mRightMotor;
  Motor& mTailMotor;
};

#endif

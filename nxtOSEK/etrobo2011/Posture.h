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

  /*! ���s�̂�➑̂����Ɏw�肵���p�x�X����B

    ���s�̂����ɌX���A�w�肵���p�x�ɑ̐���ۂB
    
    @param angle ����➑̂��X����p�x�B
  */
  void inclineBackward(int angle);

 private:
  Motor& mLeftMotor;
  Motor& mRightMotor;
  Motor& mTailMotor;
};

#endif

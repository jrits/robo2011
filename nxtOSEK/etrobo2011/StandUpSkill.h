#ifndef STANDUPSKILL_H
#define STANDUPSKILL_H

// #include "Activator.h"
// #include "TripodActivator.h"
#include "Motor.h"

using namespace ecrobot;
class Activator;
class TripodActivator;

class StandUpSkill {
 public:
  StandUpSkill(Activator& activator,
               TripodActivator& tripodActivator,
               Motor& leftMotor,
               Motor& rightMotor,
               Motor& tailMotor);
  
  ~StandUpSkill(){}

  void execute();
  //! Returns true when this skill finished.
  bool isDone() const;

 private:
  bool isStable() const;
  bool isStandUp() const;
  
  void tail_control_with_PID(signed int target_angle);
  //! 
  enum SubState{
    INIT = 0,
    TO_FIRST_ANGLE,
    TO_SECOND_ANGLE,
    TO_STABLE,
    STANDING_UP,
    DONE
  };

  Activator& mActivator;
  TripodActivator& mTripodActivator;
  Motor& mLeftMotor;
  Motor& mRightMotor;
  Motor& mTailMotor;

  SubState mSubState;
  bool mIsStandUp;
  int mStableCount;
  bool mOnlyFirst;

  // 微分動作を計算するための、前回の誤差を保持する。
  float mLastDiff;
  // 積分動作を保持するためのフィールド。
  float mIterm;
};

#endif

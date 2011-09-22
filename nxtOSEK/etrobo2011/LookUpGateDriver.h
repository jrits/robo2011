#ifndef LOOKUPGATEDRIVER_H
#define LOOKUPGATEDRIVER_H

#include "Driver.h"

class LookUpGateDriver : public Driver {
public:
    LookUpGateDriver();
  ~LookUpGateDriver();
  bool drive();

 private:

  //! ゲートを見つける。
  /**
     @return ゲートを見つけたらtrueを返す。
   */
  bool foundGate() const;

  //! ゲートを超えたか判定する。
  /**
     Check whether nxt passed the look-up gate.


     @return return true if Nxt passed the look-up gate.
   */
  bool passedGate();
  //! ゲート手前で座り込む。
  bool sitDown();
  //! ゲート通過後に立ち上がる。
  bool standUp();
  //! LookUpGateDriverの処理が終了したか判定する。
  /**
     @return 終了している場合はtrue。
  */
  bool isDone() const;

  //! ゲートの真下にいるか判定する。
  bool isUnderGate() const;
  //! サブ区間を表現するenum。
  enum eSubSection {
    INIT = 0,//!< 初期区間。
    IN_FRONT_OF_GATE, //!< ゲート手前区間。
    UNDER_GATE, //!< ゲート下。
    BEHIND_GATE,//!<　ゲートを
    DONE
  };

  //! サブ区間を表現するフィールド。
  eSubSection mCurrentSubSection;

  //! スタート地点からゲートまでの距離。
  float mDistanceAtGate;
  //! ゲート通貨後に、オーバーランする距離。
  float mOverrunDistance;
  //! ゲートで、座標の修正を行ったかどうか判定するフラグ。
  bool mAdjustCoordinatesFlag;
};

#endif


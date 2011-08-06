//
// TripodSkill.h
//
#ifndef TripodSkill_H_
#define TripodSkill_H_

#include "constants.h"
#include "TripodActivator.h"
extern TripodActivator mTripodActivator;

/**
 * スキル抽象クラス
 *
 * このクラスのオブジェクトは生成されない
 */
class TripodSkill
{
protected:
	float mForward; //!< フォワード値
public:
  /**
   * コンストラクタ
   */
	TripodSkill(){}
  /**
   * デストラクタ
   */
	virtual ~TripodSkill(){}
  /**
   * フォワード値をセットする
   *
   * @param[in] forward フォワード値
   */
	void setForward(float forward)
    {
      mForward = forward;
    }
  /**
   * スキルを発揮してロボットを動かす。
   *
   * calcCommand を用いて走行ベクトルを決定し、制御機器(TripodActivator)を操作する。
   */
  void execute()
    {
      VectorT<float> command = calcCommand();
      mTripodActivator.run(command);//制御機器にセット
    }
  /**
   * 走行ベクトルを計算
   *
   * Override Me!!
   */
	virtual VectorT<float> calcCommand() { return VectorT<float>(0,0); }
  // 純粋仮想関数にするとプログラム容量が肥大化する。とりあえず純粋仮想関数を避ける。
};

#endif

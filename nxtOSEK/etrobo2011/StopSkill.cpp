#include "StopSkill.h"
#include "factory.h"

/**
 * コンストラクタ
 */
StopSkill::StopSkill() :
    Skill()
{
    mTargetDistance = 0.0; // mm
    mAllowableError = 0.0;
    mSkill = &mLineTrace; // とりあえず
}

/**
 * 急停止機構を追加するスキルを設定する
 *
 * @param[in] skill 急停止機構を追加されるスキル
 */
void StopSkill::setSkill(Skill *skill)
{
    if (skill == 0) skill = &mLineTrace; // とりあえず
    mSkill = skill;
}

/**
 * 停止目標距離を設定する。
 *
 * @param[in] targetDistance 停止目標距離(現在の位置からの相対距離)
 */
void StopSkill::setTargetDistance(float targetDistance)
{
    mTargetDistance = mGps.getDistance() + targetDistance;
};

/**
 * 許容エラー値を設定する。
 *
 * @param[in] allowableError 許容エラー値
 */
void StopSkill::setAllowableError(float allowableError)
{
    mAllowableError = allowableError;
};

/**
 * 目標距離と現在距離から、急停止動作を行うのに適切な走行ベクトルを計算する
 *
 * @return 走行ベクトル
 */
VectorT<float> StopSkill::calcCommand()
{
    // スキルの利用。フォワード値を(必要があれば)上書きする。
    VectorT<float> command = mSkill->calcCommand();

    // フォワード値の制御。目標地点にたどり着いたらフォワード値0
    if (isArrived()) command.mX = 0;

#if 0
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // 今だけ static
    float diff = mTargetDistance -  mGps.getDistance();
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "StopSkill");
        lcd.putf("dn", (int)mTargetDistance);
        lcd.putf("dn", (int)mAllowableError);
        lcd.putf("dn", (int)mGps.getDistance());
        lcd.putf("dn", (int)diff);
        lcd.putf("dn", (int)command.mX);
        lcd.disp();
    }
#endif

    return command;
}

/**
 * 目標距離に到着したか
 *
 * @retval true 到着した
 * @retval false まだ
 */
bool StopSkill::isArrived()
{
    // 目標距離と現在距離との差
    float diff = mTargetDistance -  mGps.getDistance();

    // 許容範囲内かをを閾値にて判定
    return (diff < mAllowableError);
}

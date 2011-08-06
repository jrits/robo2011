#include "SlowdownSkill.h"
#include "factory.h"

/**
 * コンストラクタ
 */
SlowdownSkill::SlowdownSkill() :
    Skill()
{
    mTargetDistance = 0.0; // mm
    mSkill = &mLineTrace; // とりあえず
    mMinimumForward = 10.0;
    mAllowableError = 0.0;
}

/**
 * 減速機構を追加するスキルを設定する
 *
 * @param[in] skill 減速機構を追加されるスキル
 */
void SlowdownSkill::setSkill(Skill *skill)
{
    if (skill == 0) skill = &mLineTrace; // とりあえず
    mSkill = skill;
}

/**
 * 停止目標距離を設定する。
 *
 * @param[in] targetDistance 停止目標距離(現在の位置からの相対距離)
 */
void SlowdownSkill::setTargetDistance(float targetDistance)
{
    mTargetDistance = mGps.getDistance() + targetDistance;
};

/**
 * 減速最低速度を設定する。
 *
 * @param[in] minimumForward 減速最低速度
 */
void SlowdownSkill::setMinimumForward(float minimumForward)
{
    mMinimumForward = minimumForward;
};

/**
 * 許容エラー値を設定する。
 *
 * @param[in] allowableError 許容エラー値
 */
void SlowdownSkill::setAllowableError(float allowableError)
{
    mAllowableError = allowableError;
};

/**
 * 目標距離と現在距離から、減速動作を行うのに適切な走行ベクトルを計算する
 *
 * @return 走行ベクトル
 */
VectorT<float> SlowdownSkill::calcCommand()
{
    // スキルの利用。フォワード値を(必要があれば)上書きする。
    VectorT<float> command = mSkill->calcCommand();

    // フォワード値のPID制御
    float P = mTargetDistance -  mGps.getDistance();
    float X = mSlowdownPid.control(P);
    if (X > command.mX) X = command.mX;
    if (X < mMinimumForward) X = mMinimumForward;
    command.mX = X;

#if 0
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // 今だけ static
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "SlowdownSkill");
        lcd.putf("dn", (int)mTargetDistance);
        lcd.putf("dn", (int)mAllowableError);
        lcd.putf("dn", (int)mGps.getDistance());
        lcd.putf("dn", (int)P);
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
bool SlowdownSkill::isArrived()
{
    // 目標距離と現在距離との差
    float diff = mTargetDistance - mGps.getDistance();

    // 許容範囲内かをを閾値にて判定
    return (diff < mAllowableError);
}

/**
 * 目標地点を通り過ぎたかの判定を返す。
 *
 * @return
 */
bool SlowdownSkill::isPassed()
{
    float diff = mTargetDistance -  mGps.getDistance();
    return (diff < 0);
};

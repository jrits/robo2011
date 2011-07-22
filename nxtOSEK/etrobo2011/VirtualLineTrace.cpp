#include "VirtualLineTrace.h"
#include "factory.h"
#include <math.h>

/**
 * コンストラクタ
 */
VirtualLineTrace::VirtualLineTrace() :
    Skill()
{
    mCurrentIndex = 0; // 現在制御点インデックス
    mControlPoints = (ControlPoint*)null; //制御点リスト(なし)
    mNumOfControlPoints = 0; //制御点の個数(0)

    mCoordinateTrace.setForward(100); // デフォルトフォワード値
    mCoordinateTrace.setAllowableError(10.0); // デフォルト許容誤差(mm)
    mAngleTrace.setAllowableError(2.0); // デフォルト許容誤差(degree)
	
	mXoffset = -170; // mXoffset = 0; // @todo: 大会のために絶妙な調整が必要
	mYoffset = 0;
}

/**
 * 状態のリセット
 *
 * 現在制御点インデックスをリセットする
 *
 * @return -
 */
void VirtualLineTrace::reset()
{
    mCurrentIndex = 0; // 現在制御点インデックス
}

/**
 * 制御点セットを設定する。
 *
 * @param[in] controlPoints 制御点セット
 * @param[in] numOfControlPoints 制御点の数
 */
void VirtualLineTrace::setControlPoints(ControlPoint *controlPoints, int numOfControlPoints)
{
    mControlPoints = controlPoints;
    mNumOfControlPoints = numOfControlPoints;
};

/**
 * 仮想ライントレースを行うのに適切な走行ベクトルを計算する
 *
 * @return 走行ベクトル
 */
VectorT<float> VirtualLineTrace::calcCommand()
{
	gLineTrace = false;
    VectorT<float> command;

    // インデックスが最大値を超えて落ちたりしないように保険
    mCurrentIndex = MIN(mCurrentIndex, mNumOfControlPoints - 1);

    // 変数名が長いので一旦別名に保存
    Point targetCoordinate = {mControlPoints[mCurrentIndex].X, mControlPoints[mCurrentIndex].Y};
    float direction = mControlPoints[mCurrentIndex].direction;
    float forward = mControlPoints[mCurrentIndex].forward;
    float allowableError = mControlPoints[mCurrentIndex].allowableError;
    //bool  slowdown = mControlPoints[mCurrentIndex].slowdown;
	bool slowdown = false; // 大会直前調整。slowdown フラグをライン復帰許可フラグとして使うのでキャンセル。

    if (! isnan(targetCoordinate.X) && ! isnan(targetCoordinate.Y)) { 
        // 座標指定走行
    	Point targetCoordinateOff = MakePoint(mControlPoints[mCurrentIndex].X + mXoffset, mControlPoints[mCurrentIndex].Y + mYoffset);
        mCoordinateTrace.setTargetCoordinate(targetCoordinateOff);
        if (! isnan(forward)) mCoordinateTrace.setForward(forward);
        if (! isnan(allowableError)) mCoordinateTrace.setAllowableError(allowableError);
        // 通常座標指定走行
        if (! slowdown) { 
            command = mCoordinateTrace.calcCommand();
        }
        // スローダウン座標指定走行
        else { 
            mSlowdownSkill.setSkill(&mCoordinateTrace);
            mSlowdownSkill.setTargetDistance(mGps.calcDistanceTo(targetCoordinateOff));
            command = mSlowdownSkill.calcCommand();
        }
        // 次の点へ
        if (mCoordinateTrace.isArrived()) mCurrentIndex++;
    }
    else if (! isnan(direction)) {
        // 方向転換
        mAngleTrace.setTargetAngle(direction);
        command = mAngleTrace.calcCommand();
        command.mX = 0.0; // 方向転換なのでフォワード値0
        if (! isnan(allowableError)) mAngleTrace.setAllowableError(allowableError);
        // 次の点へ
        if (mAngleTrace.isArrived()) mCurrentIndex++;
    }
    else { // おかしな場合はとりあえずライントレースさせておく？？
        command = mLineTrace.calcCommand();
    }

#if 0 // DEBUG
    static int count = 0;
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "VirtualLineTrace");
        lcd.putf("dn", mCurrentIndex);
        lcd.putf("dn", (int)targetCoordinate.X);
        lcd.putf("dn", (int)targetCoordinate.Y);
        lcd.putf("dn", (int)mGps.getXCoordinate());
        lcd.putf("dn", (int)mGps.getYCoordinate());
        lcd.putf("dn", (int)mGps.getDirection());
        lcd.disp();
    }
#endif

    return command;
};

/**
 * 最終制御点状態かどうかを判定する
 *
 * @retval true 最終制御点状態である
 * @retval false まだまだ
 */
bool VirtualLineTrace::isLast()
{
    //return (mCurrentIndex >= mNumOfControlPoints - 1);
	return (mCurrentIndex >= mNumOfControlPoints - 1) || mControlPoints[mCurrentIndex].slowdown; // 大会直前調整。slowdown フラグをライン復帰許可フラグとして使う。
}

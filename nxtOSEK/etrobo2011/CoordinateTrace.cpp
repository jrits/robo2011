#include "CoordinateTrace.h"
#include "factory.h"
#include <math.h>

/**
 * コンストラクタ
 */
CoordinateTrace::CoordinateTrace() :
    Skill()
{
    mTargetCoordinate = MakePoint(0, 0);
    setForward(100);
    mAllowableError = 100.0; // mm
}

/**
 * 目標位置を設定する。
 *
 * @param[in] targetCoordinate 目標座標
 */
void CoordinateTrace::setTargetCoordinate(Point targetCoordinate)
{
    mTargetCoordinate = targetCoordinate;
};


/**
 * 許容エラー値を設定する。
 *
 * @param[in] allowableError 許容エラー値
 */
void CoordinateTrace::setAllowableError(float allowableError)
{
    mAllowableError = allowableError;
};

/**
 * 目標座標と現在座標から、座標指定走行を行うのに適切な走行ベクトルを計算する
 *
 * @return 走行ベクトル
 */
VectorT<float> CoordinateTrace::calcCommand()
{
    // 現在座標取得
    Point current;
    current.X = mGps.getXCoordinate();
    current.Y = mGps.getYCoordinate();

    // 現在座標と目標座標との差
    float diffX = mTargetCoordinate.X - current.X;
    float diffY = mTargetCoordinate.Y - current.Y;
 
#if 0
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // 今だけ static
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "CoordinateTrace");
        lcd.putf("dn", (int)mTargetCoordinate.X);
        lcd.putf("dn", (int)mTargetCoordinate.Y);
        lcd.putf("dn", (int)current.X);
        lcd.putf("dn", (int)current.Y);
        lcd.disp();
    }
#endif
    
    // 目標への角度を算出
    float targetDirection;
    if (diffX == 0.0 && diffY == 0.0) {
        targetDirection = mGps.getDirection();
    }
    else {
        double radian = Gps::atan2(diffY, diffX);
        if (isinf(radian)) targetDirection = mGps.getDirection();
        else targetDirection = Gps::radianToDegree(radian);
    }

    // アングルトレースの利用
    mAngleTrace.setTargetAngle(targetDirection);
    VectorT<float> command = mAngleTrace.calcCommand();
    command.mX = mForward; // フォワード値上書き

    return command;
};

/**
 * 目標位置に到着したか
 *
 * @retval true 到着した
 * @retval false まだ
 */
bool CoordinateTrace::isArrived()
{
    // 現在座標取得
    Point current;
    current.X = mGps.getXCoordinate();
    current.Y = mGps.getYCoordinate();

    // 現在座標と目標座標との距離
    double diffX = mTargetCoordinate.X - current.X;
    double diffY = mTargetCoordinate.Y - current.Y;
    double distance = sqrt(pow(diffX, 2) + pow(diffY, 2));

    // 許容範囲内かをを閾値にて判定
    return (distance < mAllowableError);
}

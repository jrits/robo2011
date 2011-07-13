#include "CoordinateStop.h"
#include "factory.h"

/**
 * コンストラクタ
 */
CoordinateStop::CoordinateStop() : 
    Skill()
{
    setTargetCoordinate(MakePoint(0, 0));
    setSlowdownDistance(500); // mm
    setForward(100);
}

/**
 * 目標位置を設定する。
 *
 * @param[in] targetCoordinate 目標座標
 */
void CoordinateStop::setTargetCoordinate(Point targetCoordinate)
{
    mTargetCoordinate = targetCoordinate;
    // 現在座標
    float currentX = mGps.getXCoordinate();
    float currentY = mGps.getYCoordinate();
    // 目標座標と現在座標との距離
    float diffX = mTargetCoordinate.X - currentX;
    float diffY = mTargetCoordinate.Y - currentY;
    float diffDistance = sqrt(pow(diffX, 2) + pow(diffY, 2));
    // 目標距離を算出
    mTargetDistance = mGps.getDistance() + diffDistance;
};

/**
 * 目標座標の何mm手前から着地動作を始めるか設定する
 *
 * @param[in] slowdownDistance 目標座標の何mm手前から着地動作を始めるか
 */
void CoordinateStop::setSlowdownDistance(float slowdownDistance)
{
	mSlowdownDistance = slowdownDistance;
}

/**
 * 目標距離を自前で設定する。
 *
 * setTargetCoordinate() が目標距離を勝手に計算してくれるが、微調整したい場合はこちらを使う。
 *
 * @param[in] targetDistance 目標距離
 */
void CoordinateStop::setTargetDistance(float targetDistance)
{
    mTargetDistance = targetDistance;
};

/**
 * 目標距離と現在距離から、座標指定着地を行うのに適切な走行ベクトルを計算する
 *
 * @return 走行ベクトル
 */
 
/* 
VectorT<float> CoordinateStop::calcCommand()
{
    // 座標指定走行の利用。フォワード値を上書きする。
    VectorT<float> command = mCoordinateTrace.calcCommand();

    // 現在距離
    float distance = mGps.getDistance();

    // 距離が近づいたら徐々に速度を下げる。通り過ぎたらバックする。
    // @todo: 通り過ぎないのが一番良い。
    if (mTargetDistance - distance < mSlowdownDistance) { // 着地動作開始
        float P = mTargetDistance - distance;
        float X = mStopPid.control(P);
        if(X > mForward) X = mForward;
        if(X < -mForward) X = -mForward;
        command.mX = X;

#if 0
        {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "CoordinateStop");
            lcd.putf("dn", (int)mTargetDistance);
            lcd.putf("dn", (int)distance);
            lcd.putf("dn", (int)command.mX);         
            lcd.disp();
        }
#endif
    }
    else { // まだ遠い場合
        command.mX = mForward;
    }

	return command;
}

*/

// SORA
//calcCommand2 目標座標付近で急停止パターン

VectorT<float> CoordinateStop::calcCommand()
{
    
    // 座標指定走行の利用。フォワード値を上書きする。
    VectorT<float> command = mCoordinateTrace.calcCommand();

    // 現在距離
    float distance = mGps.getDistance();

    // 距離が近づいたら急停止。SlowdownDistanceをひとまず拝借している。適当な変数を準備して適当な値に変更すべき？
    if ((mTargetDistance - distance) < mSlowdownDistance) { // 着地動作開始
        
        command.mX = 0;
        //SORA test
        command.mY = 0;

#if 0
        {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "CoordinateStop");
            lcd.putf("dn", (int)mTargetDistance);
            lcd.putf("dn", (int)distance);
            lcd.putf("dn", (int)command.mX);         
            lcd.disp();
        }
#endif
    }
    else { // まだ遠い場合
        //command.mX = mForward;
    }
    
//SORA [log]    
#if 2
    // ログ送信
    LOGGER_SEND = 2;
//	LOGGER_DATAS08[0] = (S8)(linePattern);
	LOGGER_DATAS16[0] = (S16)(mTargetDistance - distance);
	LOGGER_DATAS16[1] = (S16)(command.mX);
//	LOGGER_DATAS16[2] = (S16)(mTargetDistance - distance);
//	LOGGER_DATAS16[3] = (S16)(lightDiff);
#endif
    
    
        
	return command;
}

//  2010/11/25  失敗：急停止は転ぶ
//SORA */

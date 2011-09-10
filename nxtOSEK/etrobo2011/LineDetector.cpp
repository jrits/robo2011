//
// LineDetector.cpp
//

#include "LineDetector.h"
#include "constants.h"
#include "factory.h"

#define LINEDETECT_THRESHOLD 20 // ご検知しない程度に小さければ良い

/**
 * ラインを検知する
 *
 * @return ライン検知の結果
 */
LineDetector::ePattern LineDetector::detect()
{
    LineDetector::ePattern linePattern;

    // 光変化量
    float lightDiff = mLightHistory.calcDifference();
    //lightDiff *= (100/FORWARD); // 速度を考慮に入れる

    if (lightDiff >= LINEDETECT_THRESHOLD) { // 白=>黒パターン
        linePattern =  WHITE_TO_BLACK;
    }
    else if (lightDiff <= (-1)*LINEDETECT_THRESHOLD) { // 黒⇒白パターン
        linePattern = BLACK_TO_WHITE;
    }
    // これだけだと灰色検知が難しい。ことがある。
    else if (mLightHistory.get(LATEST) > LINE_THRESHOLD) { // 黒⇒黒
        linePattern = ON_BLACK;
    }
    else { // 白⇒白
        linePattern = ON_WHITE;
    }

#if 0
    // ログ送信
    LOGGER_SEND = 2;
    LOGGER_DATAS08[0] = (S8)(linePattern);
    LOGGER_DATAS16[0] = (S16)(mLightHistory.get(LATEST));
    LOGGER_DATAS16[1] = (S16)(mLightHistory.get(OLDEST));
    LOGGER_DATAS16[2] = (S16)(mLightHistory.calcDifference());
    LOGGER_DATAS16[3] = (S16)(lightDiff);
#endif

    return linePattern;
}

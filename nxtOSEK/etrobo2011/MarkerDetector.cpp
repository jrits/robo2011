//
// MarkerDetector.cpp
//

#include "MarkerDetector.h"
#include "constants.h"

#include "Speaker.h"

/**
 * 灰色マーカー区間かどうかを判定する。
 *
 * NOTE:１度の灰色マーカー区間で一度だけ true になります。
 *
 * @retval true 灰色マーカー区間
 * @retval false そうじゃない
 */
bool MarkerDetector::detect()
{
    double L = mLightSensor.get();
    // 灰色のサンプルをカウントする。黒を１度でも見つけたら0にリセットする。
    // 灰(または白)
    if (L < MARKER_THRESHOLD) { // 黒灰閾値 = (灰平均(610) + 黒平均(700)) / 2.0 = 655
        mGrayCount++;
        if (mGrayCount > mNsample) { mGrayCount = mNsample; } // 安全のため
    }
    // 黒
    else { 
        //Speaker speaker;
        //speaker.playTone(50, 1, 100); // Hz:33-1976 , 10ms, volume:0-100
        mGrayCount = 0;
    }
    // mNsample 期間中、黒が現れなければマーカと判断する。
    if (mGrayCount >= mNsample) {
//        Speaker speaker;
//        speaker.playTone(1976, 10, 100); // Hz:33-1976 , 10ms, volume:0-100
        return true;
    }
    return false;
}

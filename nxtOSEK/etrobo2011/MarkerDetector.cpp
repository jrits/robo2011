//
// MarkerDetector.cpp
//

#include "MarkerDetector.h"
#include "constants.h"
#include "Speaker.h"
#include "factory.h"

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
    // ３点傾立走行中？ @todo: gDoTripod なんかがないのでとりあえず。ちゃんとしたい。
    bool doTripod = mTailMotor.getCount() > 30;

    // マーカー検出閾値
    float markerThresh = gDoMaimai ?
        (doTripod ? MAIMAI_TRIPOD_MARKER_THRESHOLD : MAIMAI_MARKER_THRESHOLD) :
        (doTripod ? TRIPOD_MARKER_THRESHOLD : MARKER_THRESHOLD);

    // 黒線の上かどうか // まいまいの場合は黒いほうが値が小さいが、光センサの場合は黒いほうが大きい
    bool isOnBlack = (gDoMaimai ? gMaimaiValue < markerThresh : mLightSensor.get() > markerThresh);

    // 灰(または白)のサンプルをカウントする。黒を１度でも見つけたら0にリセットする。
    if (isOnBlack) {
        Speaker speaker;
        speaker.playTone(50, 1, 100); // Hz:33-1976 , 10ms, volume:0-100
        mGrayCount = 0;
    }
    else {
        mGrayCount = (mGrayCount > mNsample ? mNsample : mGrayCount + 1);
    }
    // mNsample 期間中、黒が現れなければマーカと判断する。
    if (mGrayCount >= mNsample) {
        Speaker speaker;
        speaker.playTone(1976, 10, 100); // Hz:33-1976 , 10ms, volume:0-100
        return true;
    }
    return false;
}

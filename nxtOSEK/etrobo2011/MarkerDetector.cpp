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
 * @retval true 灰色マーカー区間
 * @retval false そうじゃない
 */
bool MarkerDetector::detect()
{
    // マーカー検出閾値
    float markerThresh = gDoMaimai ?
        (gDoTripod ? MAIMAI_TRIPOD_MARKER_THRESHOLD : MAIMAI_MARKER_THRESHOLD) :
        (gDoTripod ? TRIPOD_MARKER_THRESHOLD : MARKER_THRESHOLD);

    // 黒線の上かどうか // まいまいの場合は黒いほうが値が小さいが、光センサの場合は黒いほうが大きい
    float L = (gDoMaimai ? gMaimaiValue : mLightSensor.get());
    bool isOnBlack = (gDoMaimai ? L < markerThresh : L > markerThresh);

#if 0 // DEBUG
    //DESK_DEBUG = true; // モータを回さないデバグ
    static int count = 0; // staticは原則禁止だが今だけ
    if (count++ % 25 == 0) {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "MarkerDetector");
        lcd.putf("dn", gDoMaimai);
        lcd.putf("dn", doTripod);
        lcd.putf("dn", (S32)(markerThresh * 100));
        lcd.putf("dn", (S32)(L * 100));
        lcd.putf("dn", isOnBlack);
        lcd.disp();
    }
#endif

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

//
// StraightDetector.cpp
//

#include "StraightDetector.h"
#include "constants.h"
#include "factory.h"
#include "macro.h"

/**
 * 直線を検知する
 *
 * @return 直線検知の結果
 */
bool StraightDetector::detect()
{
    // 移動平均向きの変化量が3度以下なら直線
    float diffDirection = mDirectionAverageHistory.calcDifference();
    bool isStraight = (ABS(diffDirection) < 3.0);

#if 0
    // ログ送信
    LOGGER_SEND = 1;
    LOGGER_DATAS08[0] = (S8)(isStraight);
    LOGGER_DATAS32[0] = (S32)(diffDirection);
#endif
    
    return isStraight;
}

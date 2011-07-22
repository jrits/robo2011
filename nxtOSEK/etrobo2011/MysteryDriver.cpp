//
// MysteryDriver.cpp
//

#include "MysteryDriver.h"
#include "factory.h"
#include "constants.h"

//ミステリーサークルのルート(4パターン)
//#include "MysteryLineLL.h"
//#include "MysteryLineLR.h"
//#include "MysteryLineRL.h"
//#include "MysteryLineRR.h"

/**
 * コンストラクタ
 */
MysteryDriver::MysteryDriver()
{
    mState = MysteryDriver::INIT;
    mInitState = true;
}

/**
 * ミステリーサークル区間を攻略する
 *
 * サークル前直線ライントレースから、ライン復帰後ライントレースまでを担当する。
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool MysteryDriver::drive()
{
    return 0;
}

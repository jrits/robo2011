///////////////////////////////////////////////////////////
//  NormalDriver.cpp
//  Implementation of the Class NormalDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#include "NormalDriver.h"
#include "factory.h"

/**
 * コンストラクタ
 */
NormalDriver::NormalDriver()
{
}

/**
 * ノーマル走行アルゴリズム
 *
 * ただのライントレース
 *
 * @retval true 最終状態。これ以降の状態遷移なし。
 * @retval false まだ役目が完了していません。
 */
bool NormalDriver::drive()
{
    mLineTrace.execute();
    return true;
}

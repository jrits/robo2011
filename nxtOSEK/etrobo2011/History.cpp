///////////////////////////////////////////////////////////
//  Histrory.cpp
//  Implementation of the Class Histrory
//  Created on:      06-10-2010
//  Original author: yhayama
///////////////////////////////////////////////////////////

//#include<iostream>
#include "History.h"

/**
 * コンストラクタ
 *
 * @param[in] buffer バッファとして使う配列を指すポインタ
 * @param[in] size バッファサイズ
 */
History::History(float* buffer, int size):
    mBuffer(buffer),
    mSize(size),
    mIndex(0)
{
}

/**
 * デストラクタ
 */
History::~History()
{
}

/**
 * バッファサイズの取得
 *
 * @return バッファサイズ
 */
int History::getSize()
{
    return mSize;
}

/**
 * 移動平均計算関数
 *
 * バッファ全体の平均値を計算する
 *
 * @return 平均値
 */
float History::calcAverage()
{
    float sum = 0.0;
    float ave = 0.0;
    for (int i = 0; i < mSize ; i++) {
        sum += mBuffer[i];
    }
    ave = sum / (float)mSize;
    return ave;
}

/**
 * 時間差分計算関数
 *
 * バッファ最新値と最古値の差を計算する
 *
 * @return 差分値
 */
float History::calcDifference()
{
    return get(LATEST) - get(OLDEST);
}

/**
 * 値の取得
 *
 * @param[in] time どの時間の値を取得するか指定。
 * - 0 が最新。-1 が直前。負の方向に進むにつれて古い値になる。
 * - 簡易的に 1 を指定することで最古値を取得できる。2 が２番目に古い値。3 が…
 */
float History::get(int time)
{
    unsigned int index = (time + mIndex + mSize) % mSize;
    // std::cout << "time: " << time << " index: " << index <<  " value: " << mBuffer[index] << std::endl;
    return mBuffer[index];
}

/**
 * ヒストリの更新
 *
 * @return -
 */
void History::update(float data)
{
    mIndex = (mIndex + 1) % mSize;
    //float oldestValue = mBuffer[mIndex];
    mBuffer[mIndex] = data;
}

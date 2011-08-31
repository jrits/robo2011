#ifndef __MYSTERY_LINE_LL_H__
#define __MYSTERY_LINE_LL_H__

#include "struct.h"

int NUM_MYSTERY_LINE_LL = 12;   //要素数

ControlPoint MYSTERY_LINE_LL[]={    //GpsVisualizerからのデータ（左-左周りのコース上のポイント）
	{3330, -3114, FP_NAN, 50, 60, FALSE}, 
	{3450, -3057, FP_NAN, 50, 60, FALSE}, 
	{3504, -2952, FP_NAN, 50, 60, FALSE}, 
	{3435, -2628, FP_NAN, 50, 60, FALSE}, 
	{3129, -2604, FP_NAN, 50, 60, FALSE}, 
	{2805, -2583, FP_NAN, 50, 60, FALSE}, 
	{2739, -2331, FP_NAN, 50, 60, FALSE}, 
	{2904, -2151, FP_NAN, 50, 60, FALSE}, 
	{3018, -1833, FP_NAN, 50, 60, TRUE}, 
	{3279, -1620, FP_NAN, 50, 60, TRUE}, 
	{3897, -1608, FP_NAN, 50, 60, TRUE}, 
	{5223, -1794, FP_NAN, 50, 60, TRUE}
};

#endif // __MYSTERY_LINE_LL_H__

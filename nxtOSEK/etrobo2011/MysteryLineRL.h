#ifndef __MYSTERY_LINE_RL_H__
#define __MYSTERY_LINE_RL_H__

#include "struct.h"

int NUM_MYSTERY_LINE_RL = 10;   //要素数

ControlPoint MYSTERY_LINE_RL[]={    //GpsVisualizerからのデータ（右-左周りのコース上のポイント）
	{3330, -3123, FP_NAN, 50, 60, FALSE}, 
	{3666, -3024, FP_NAN, 50, 60, FALSE}, 
	{3849, -2880, FP_NAN, 50, 60, FALSE}, 
	{3765, -2580, FP_NAN, 50, 60, FALSE}, 
	{2814, -2586, FP_NAN, 50, 60, FALSE}, 
	{2727, -2358, FP_NAN, 50, 60, FALSE}, 
	{3108, -1740, FP_NAN, 50, 60, TRUE}, 
	{3462, -1617, FP_NAN, 50, 60, TRUE}, 
	{3987, -1617, FP_NAN, 50, 60, TRUE}, 
	{5238, -1782, FP_NAN, 50, 60, TRUE}
};

#endif // __MYSTERY_LINE_RL_H__

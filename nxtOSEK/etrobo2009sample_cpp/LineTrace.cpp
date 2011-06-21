//
// LineTrace.cpp
//

#include "LineTrace.h"


#define WHITE		  500 // 白色の光センサ値
#define BLACK		  700 // 黒色の光センサ値

#define FORWARD       50 // 前進命令

//=============================================================================
// ライントレース命令
VectorT<S8> LineTrace::getCommand()
{
	if (mLight.get() <= (WHITE + BLACK)/2)
		return VectorT<S8>(FORWARD, 50); // 右折
	else
		return VectorT<S8>(FORWARD,-50); // 左折
}

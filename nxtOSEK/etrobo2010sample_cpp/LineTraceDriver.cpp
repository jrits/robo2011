//
// LineTraceDriver.cpp
//

#include "LineTraceDriver.h"

//=============================================================================
// Simple line tracing
void LineTraceDriver::update(void)
{
	if (mrLight.get() <= (mBlack + mWhite)/2)
	{
		mDriverCmd = VectorT<S16>(50, 50); // Turn right
	}
	else
	{
		mDriverCmd = VectorT<S16>(50,-50); // Turn left
	}
}

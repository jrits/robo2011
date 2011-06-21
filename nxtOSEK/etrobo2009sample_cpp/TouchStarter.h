//
// TouchStarter.h
//

#ifndef TOUCHSTARTER_H_
#define TOUCHSTARTER_H_

#include "TouchSensor.h"
using namespace ecrobot;

class TouchStarter
{
private:
	const TouchSensor& mrTouch;

public:
	TouchStarter(const TouchSensor& touch): mrTouch(touch) {}

	~TouchStarter(){}
	
	// タッチセンサ押下で開始
	void waitForReadyGo(){ while(!mrTouch.isPressed()); }
};

#endif /*TOUCHSTARTER_H_*/

//
// TouchStarter.h
//
// タッチスタータスタブ

#ifndef TOUCHSTARTER_H_
#define TOUCHSTARTER_H_

#include "TouchSensor.h"

class TouchStarter
{
private:
	TouchSensor& mrTouch;

public:
	TouchStarter(TouchSensor& touch): mrTouch(touch) {}

	~TouchStarter(){}
	
	// タッチセンサ押下で開始
	void waitForReadyGo(){ while(!mrTouch.isPressed()); }
};

#endif /*TOUCHSTARTER_H_*/

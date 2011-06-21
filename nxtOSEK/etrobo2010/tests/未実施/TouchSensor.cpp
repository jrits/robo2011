//
// TouchSensor.cpp
//
// タッチセンサスタブ
//

#include "TouchSensor.h"

//=============================================================================
// Constructor
TouchSensor::TouchSensor(){
	pressed = "";
}

//=============================================================================
// get Touch Sensor status
bool TouchSensor::isPressed()
{
	bool val = false;
	pressed = " ";
	
	/* 1が入力されるまでループ */
	cout << "press 1 to start " << endl;
	cin >> pressed;
	
	if(pressed == "1")
		val = true;
	return val;
}

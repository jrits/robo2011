//
// TouchSensor.h
//
// タッチセンサスタブ
//

#ifndef TOUCHSENSORSTUB_H_
#define TOUCHSENSORSTUB_H_

#include <iostream>
#include <string>
using namespace std;

/**
 * NXT Touch sensor class.
 */
class TouchSensor
{
private:
	string pressed;
public:
	/**
	 * Constructor.
	 * Note:<BR>
	 * This class must be constructed as a global object. Otherwise, a device assertion will be displayed<BR>
	 * in the LCD when the object is constructed as a non global object.<BR>
	 * When the object is destructed while the system is shut down, the device is de-activated automatically.
	 * @param port Touch sensor connected port
	 * @return -
	 */
	TouchSensor();

	bool isPressed();
};

#endif

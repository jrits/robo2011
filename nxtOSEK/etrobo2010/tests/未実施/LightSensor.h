//
// LightSensor.h
//
// Copyright 2009 by Takashi Chikamasa, Jon C. Martin and Robert W. Kramer
//

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

//#include "Sensor.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

#define TSIZE 100
typedef unsigned int S16;
//namespace ecrobot
//{
/**
 * NXT Light sensor class.
 */
class LightSensor
{
public:
	/**
	 * Constructor (turn on the lamp by default).
	 * Note:<BR>
	 * This class must be constructed as a global object. Otherwise, a device assertion will be displayed<BR>
	 * in the LCD when the object is constructed as a non global object.<BR>
	 * When the object is destructed while the system is shut down, the device is de-activated automatically.
	 * @param port Light sensor connected port
	 * @param lamp Turn on/off the lamp (true:on/false:off)
	 * @return -
	 */
	LightSensor(char* fileName);

	/**
	 * Destructor (turn off the lamp if it was on).
	 * @param -
	 * @return -
	 */
	~LightSensor(void);

	/**
	 * Get brightness.
	 * @param -
	 * @return Brightness value (greater value means brighter)
	 */
	S16 getBrightness(void) const;
	bool setBrightness(void);

	/**
	 * Turn on/off the lamp.
	 * @param lamp true:on/false:off
	 * @return -
	 */
	void setLamp(bool lamp);
	private:
		int mTable[TSIZE];
		int index;
};
//}

#endif

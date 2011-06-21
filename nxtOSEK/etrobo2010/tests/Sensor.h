//
// Sensor.h
//
// Sensor Stub
//
// Copyright 2009 by nseo
//

#ifndef SENSOR_H_
#define SENSOR_H_

namespace ecrobot
{
class Sensor
{
protected:
	float mValue;
public:	
	virtual float get(void) const { return mValue; }
	virtual void set(float value) { mValue = value; }
};
}

#endif

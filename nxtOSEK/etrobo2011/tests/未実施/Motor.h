//
// Motor.h
//
// Motor Stub
//
// Copyright 2010 by nseo
//

#ifndef MOTOR_H_
#define MOTOR_H_

#include "Sensor.h"

namespace ecrobot
{
class Motor : public Sensor
{
public:
    float getCount() const { return get(); }
};
}

#endif

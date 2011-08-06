//
// TripodActivator.h
//
#ifndef TripodActivator_H_
#define TripodActivator_H_

#include "Activator.h"
using namespace ecrobot;

/**
 * ３点制御機器(ハンドル、アクセル、ブレーキ)
 *
 * Skill によって操作される。
 */
class TripodActivator : public Activator
{
public:
	TripodActivator(Motor &leftMotor, 
              Motor &rightMotor, 
              GyroSensor &gyroSensor, 
              Nxt &nxt);
	~TripodActivator(){}
    void run(VectorT<F32> command);
};
#endif

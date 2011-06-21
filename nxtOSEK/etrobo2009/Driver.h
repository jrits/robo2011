//
// Driver.h
//

#ifndef DRIVER_H_
#define DRIVER_H_

#include "Vector.h"
using namespace ecrobot;

extern "C"
{
	#include "ecrobot_interface.h"
};

class Driver
{
public:
	Driver(){}

	virtual ~Driver(){}

	// A drive request
	virtual bool wannaDrive(void) { return true; } // default driver

	// Drive command: forward(100 to -100), turn(100 to -100)
	virtual VectorT<S8> getCommand(){ return VectorT<S8>(0,0); } // just standing up
};
	
#endif /*DRIVER_H_*/

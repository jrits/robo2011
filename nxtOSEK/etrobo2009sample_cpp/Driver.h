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
	
	// ëñçsñΩóﬂ: ëOå„êiñΩóﬂ(100Å`-100), ê˘âÒñΩóﬂ(100Å`-100)	
	virtual VectorT<S8> getCommand(){ return VectorT<S8>(0,0); }
};
	
#endif /*DRIVER_H_*/

//
// Driver.h
//

#ifndef DRIVER_H_
#define DRIVER_H_

#include "Vector.h"
using namespace ecrobot;

#include "NXTway_GS_data.h"

extern "C"
{
	#include "ecrobot_interface.h"
};

/**
 * Driver base class for NXTway-GS.
 */
class Driver
{
public:
	/**
	 * Robot driver request enum.
	 */
	enum eDriverRequest
	{
		NO_REQ, /**< No request */
		STOP,	/**< Request to stop the robot */
		START,	/**< Request to start controlling the robot */
		DRIVE	/**< Request to drive the robot according to the command to be acquired by using getCommand API */
	};

	/**
	 * Constructor.
	 */
	Driver(): mDriverReq(Driver::NO_REQ), mDriverCmd(VectorT<S16>(0,0)) {}

	/**
	 * Destructor.
	 */
	virtual ~Driver(){}

	/**
	 * Update driver status
	 */
	virtual void update(void) {} // By default, do nothing

	/**
	 * Reset the driver to re-start it.
	 */
	virtual void reset(void){} // By default, do nothing

	/**
	 * Notify NXTway-GS internal data (read only)
	 * @param data NXTway-GS internal data
	 */
	virtual void notify(const NXTway_GS_data* data) {} // By default, do nothing

	/**
	 * Get driver request. The Driver base class always returns DRIVE request.
	 * @return Robot driver request enum
	 */
	inline eDriverRequest getRequest(void) { return mDriverReq; }

	/**
	 * Get driver requested command. The Driver base class always returns (0,0) command.
	 * @return forward and turn command
	 */
	inline VectorT<S16> getCommand(void){ return mDriverCmd; }

protected:
	/** Driver request */
	Driver::eDriverRequest mDriverReq;
	/** Driver command vector (mX:forward, mY:turn) */
	VectorT<S16> mDriverCmd;
};

#endif /*DRIVER_H_*/

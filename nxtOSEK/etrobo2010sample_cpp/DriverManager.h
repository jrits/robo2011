//
// DriverManager.h
//

#ifndef DRIVERMANAGER_H_
#define DRIVERMANAGER_H_

#include "Driver.h"
#include "Nxtway_GS_data.h"

extern "C"
{
	#include "ecrobot_interface.h"
};

/**
 * Robot driver manager class.
 */
class DriverManager: public Driver
{
public:
	/**
	 * Constructor.
	 */
	DriverManager();

	/**
	 * Destructor.
	 */
	~DriverManager();

	/**
	 * Create the driver registration table.
	 * @param numOfDrivers Number of drivers to be registered.
	 * @return true:succeeded/false:failed
	 */
	bool createDriverTable(UINT numOfDrivers);

	/**
	 * Delete the driver registration table.
	 */
	void deleteDriverTable(void);

	/**
	 * Add a driver to the driver registration table.
	 * Note that the order of the driver registrations is equal to the priority of the drivers.
	 * (e.g. The first registered driver is the highest priority)
	 * @param driver Driver to be registered in the table.
	 * @return true:succeeded/false:failed
	 */
	bool add(Driver* driver);

	/**
	 * Update the drivers in the table.
	 * Note that this function updates all drivers status, so it should be called before getting the latest status
	 * of the drivers.
	 */
	virtual void update(void);

	/**
	 * Reset registered drivers.
	 */
	virtual void reset(void);

	/**
	 * Notify NXTway-GS internal data (read only) to the registered drivers.
	 * @param data NXTway-GS internal data
	 */
	virtual void notify(const NXTway_GS_data* data);

	/**
	 * Get the driver table index id of the driver which has a request.
	 * @return Driver table index id (-1 means that no driver had a request)
	 */
	inline SINT getDriverId(void) const { return mDriverId; }

private:
	SINT mSizeOfTable;
	SINT mNumOfDrivers;
	Driver** mDriverAddressTable;
	SINT mDriverId;
};

#endif /* DRIVERMANAGER_H_ */

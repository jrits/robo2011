//
// DriverManager.cpp
//

#include "DriverManager.h"

//=============================================================================
// Constructor
DriverManager::DriverManager():
mSizeOfTable(0),
mNumOfDrivers(0),
mDriverAddressTable(0),
mDriverId(-1)
{}

//=============================================================================
// Destructor
DriverManager::~DriverManager(void)
{
	this->deleteDriverTable();
}

//=============================================================================
// Create drivers registration table
bool DriverManager::createDriverTable(UINT numOfDrivers)
{
	bool ret = false;
	if (mSizeOfTable == 0)
	{
		mDriverAddressTable = new Driver*[numOfDrivers];
		if (mDriverAddressTable != 0)
		{
			mSizeOfTable = numOfDrivers;
			mNumOfDrivers = 0;
			ret = true;
		}
	}
	return ret;
}

//=============================================================================
// Delete drivers registration table
void DriverManager::deleteDriverTable(void)
{
	mSizeOfTable = 0;
	mDriverAddressTable = 0;
	mDriverId = -1;
	mDriverReq = Driver::NO_REQ;
	mDriverCmd = VectorT<S16>(0,0);
	if (mDriverAddressTable != 0)
	{
		delete [] mDriverAddressTable;
	}
}

//=============================================================================
// Register a driver
bool DriverManager::add(Driver* driver)
{
	bool ret = false;
	if (mNumOfDrivers < mSizeOfTable)
	{
		mDriverAddressTable[mNumOfDrivers++] = driver;
		ret = true;
	}
	return ret;
}

//=============================================================================
// Update status of the registered drivers
void DriverManager::update(void)
{
	Driver::eDriverRequest driverReq;
	mDriverReq = Driver::NO_REQ;
	mDriverCmd = VectorT<S16>(0,0);
	mDriverId = -1;

	bool driverReqFlag = false;
	for (SINT driverId=0; driverId<mNumOfDrivers; driverId++)
	{
		mDriverAddressTable[driverId]->update(); // update status of all drivers
		driverReq = mDriverAddressTable[driverId]->getRequest();
		if (driverReq != Driver::NO_REQ && !driverReqFlag)
		{
			mDriverReq = driverReq;
			mDriverCmd = mDriverAddressTable[driverId]->getCommand();
			mDriverId = driverId;
			driverReqFlag = true;
		}
	}
}

//=============================================================================
// Rest all drivers
void DriverManager::reset(void)
{
	for (SINT i=0; i<mNumOfDrivers; i++)
	{
		mDriverAddressTable[i]->reset();
	}
}

//=============================================================================
// Notify robot internal data to all drivers
void DriverManager::notify(const NXTway_GS_data* data)
{
	for (SINT i=0; i<mNumOfDrivers; i++)
	{
		mDriverAddressTable[i]->notify(data);
	}
}




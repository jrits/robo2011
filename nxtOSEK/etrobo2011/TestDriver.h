///////////////////////////////////////////////////////////
//  TestDriver.h
//  Definition of the Class TestDriver
//  Created on:      10-9-2010 19:43:43
///////////////////////////////////////////////////////////
#ifndef TESTDRIVER_H_
#define TESTDRIVER_H_

#include "Driver.h"

/**
 * テストドライバ
 */
class TestDriver : public Driver
{
public:
	TestDriver();
	~TestDriver(){}
    bool drive();
};

#endif

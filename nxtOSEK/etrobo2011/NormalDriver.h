///////////////////////////////////////////////////////////
//  NormalDriver.h
//  Definition of the Class NormalDriver
//  Created on:      10-9-2010 19:43:43
//  Original author: takahashi
///////////////////////////////////////////////////////////
#ifndef NORMALDRIVER_H_
#define NORMALDRIVER_H_

#include "Driver.h"

/**
 * í èÌãÊä‘ÉhÉâÉCÉo
 */
class NormalDriver : public Driver
{
public:
	NormalDriver();
	~NormalDriver(){}
    bool drive();
};

#endif

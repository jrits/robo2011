//
// TripodCoordinateTrace.h
//

#ifndef TripodCoordinateTrace_H_
#define TripodCoordinateTrace_H_

#include "CoordinateTrace.h"

/**
 * 
 */
class TripodCoordinateTrace : public CoordinateTrace
{
public:
    TripodCoordinateTrace();
    ~TripodCoordinateTrace(){}
    void execute();
};
#endif

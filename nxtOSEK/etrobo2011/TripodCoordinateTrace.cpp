//
// TripodCoordinateTrace.cpp
//
#include "TripodCoordinateTrace.h"
#include "factory.h"
extern TripodActivator mTripodActivator;
extern bool gDoForwardPid;

/**
 * �R���X�g���N�^
 */
TripodCoordinateTrace::TripodCoordinateTrace()
{
	//mTargetCoordinate = MakePoint(0, 0);
    //setForward(100);
    //mAllowableError = 100.0; // mm
}

/**
 * �X�L���𔭊����ă��{�b�g�𓮂����B
 *
 * calcCommand ��p���đ��s�x�N�g�������肵�A����@��(TripodActivator)�𑀍삷��B
 */
void TripodCoordinateTrace::execute()
{
    VectorT<float> command = calcCommand();
    mTripodActivator.run(command);//����@��ɃZ�b�g
}

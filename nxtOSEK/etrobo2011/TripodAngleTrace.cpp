//
// TripodAngleTrace.cpp
//
#include "TripodAngleTrace.h"
#include "factory.h"
extern TripodActivator mTripodActivator;
extern bool gDoForwardPid;

/**
 * �R���X�g���N�^
 */
TripodAngleTrace::TripodAngleTrace()
{
	//mTargetAngle = 0;
}

/**
 * �X�L���𔭊����ă��{�b�g�𓮂����B
 *
 * calcCommand ��p���đ��s�x�N�g�������肵�A����@��(TripodActivator)�𑀍삷��B
 */
void TripodAngleTrace::execute()
{
    VectorT<float> command = calcCommand();
    mTripodActivator.run(command);//����@��ɃZ�b�g
}

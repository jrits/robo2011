//
// NXTway_GS.cpp
//

#include "NXTway_GS.h"


//=============================================================================
// Constructor
NXTway_GS::NXTway_GS(const Nxt& nxt, const GyroSensor& gyro, Motor& motorL, Motor& motorR)
:
  mrNxt(nxt),
  mrGyro(gyro),
  mrMotorL(motorL),
  mrMotorR(motorR)
{
  this->reset();
}

//=============================================================================
// Reset NXTway-GS
void NXTway_GS::reset(S16 offset)
{
  mrMotorL.reset();
  mrMotorR.reset();

  balance_init();

  this->setGyroOffset(offset);
}

//=============================================================================
// Set gyro sensor offset value
void NXTway_GS::setGyroOffset(S16 offset)
{
  mGyroOffset = offset;
}

void NXTway_GS::drive(VectorT<S8> direction)
{
	
  balance_control(
  		  (F32)direction.mX, // forward command
  		  (F32)direction.mY, // turn command
  		  (F32)mrGyro.get(),
  		  (F32)mGyroOffset,
  		  (F32)mrMotorL.getCount(),
  		  (F32)mrMotorR.getCount(),
  		  (F32)mrNxt.getBattMv(),
  		  &pwmL,
  		  &pwmR);
  // absorb the difference between 2 motors.
  // pwmR = (S8) (((7.775f*pwmR-49.513f)+43.663f)/7.7109f);
  // pwmR = (S8) (6.3156f*pwmR/6.5981f);
//  pwmR = (S8) (((7.8821f*pwmR-8.8633f)+8.9700f)/7.9931f);
  mrMotorL.setPWM(pwmL);
  mrMotorR.setPWM(pwmR);
  
}

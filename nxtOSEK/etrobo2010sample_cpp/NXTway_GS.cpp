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
	mNxtwayGsData.command = VectorT<S16>(0,0);
	mNxtwayGsData.gyroData = 0;
	mNxtwayGsData.gyroOffset = offset;
	mNxtwayGsData.battMv = 0;
	this->stop();
	balance_init();
	this->setGyroOffset(offset);
}

//=============================================================================
// Set gyro sensor offset value
void NXTway_GS::setGyroOffset(S16 offset)
{
	mNxtwayGsData.gyroOffset = offset;
}

//=============================================================================
// Calibrate gyro sensor offset value dynamically
S16 NXTway_GS::calGyroOffset(void)
{
	U32 calGyroOffset = 0;
	SINT numOfAvg = 0;
	while (numOfAvg < 100) // it takes approximately 4*100 = 400msec
	{
		calGyroOffset += mrGyro.get();
		numOfAvg++;
		U32 tic = systick_get_ms();
		while (systick_get_ms() < 4 + tic); // wait for 4msec
	}

	return static_cast<S16>(calGyroOffset/numOfAvg);
}

//=============================================================================
// Get calculated motor PWM values
VectorT<S8> NXTway_GS::calcPWM(VectorT<S16> cmd)
{
	// save internal data
	mNxtwayGsData.gyroData = mrGyro.get();
	mNxtwayGsData.battMv = mrNxt.getBattMv();
	mNxtwayGsData.motorCount.mX = mrMotorL.getCount();
	mNxtwayGsData.motorCount.mY = mrMotorR.getCount();

	VectorT<S8> pwm;
	balance_control(
		static_cast<F32>(mNxtwayGsData.command.mX),
		static_cast<F32>(mNxtwayGsData.command.mY),
		static_cast<F32>(mNxtwayGsData.gyroData),
		static_cast<F32>(mNxtwayGsData.gyroOffset),
		static_cast<F32>(mNxtwayGsData.motorCount.mX),
		static_cast<F32>(mNxtwayGsData.motorCount.mY),
		static_cast<F32>(mNxtwayGsData.battMv),
		&pwm.mX,
		&pwm.mY);

	return pwm;
}

//=============================================================================
// Drive NXTway-GS by driver command
void NXTway_GS::drive(VectorT<S16> cmd)
{
	mNxtwayGsData.command = cmd;
	mNxtwayGsData.motorPwm = this->calcPWM(mNxtwayGsData.command);

	mrMotorL.setPWM(mNxtwayGsData.motorPwm.mX);
	mrMotorR.setPWM(mNxtwayGsData.motorPwm.mY);
}

//=============================================================================
// Stop the wheel motors and reset the motor encoders to zero
void NXTway_GS::stop(void)
{
	mNxtwayGsData.motorPwm = VectorT<S8>(0,0);
	mNxtwayGsData.motorCount = VectorT<S32>(0,0);
	mrMotorL.reset();
	mrMotorR.reset();
}


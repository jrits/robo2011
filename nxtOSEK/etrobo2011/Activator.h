//
// Activator.h
//
#ifndef Activator_H_
#define Activator_H_

#include "constants.h"
#include "Motor.h"
#include "GyroSensor.h"
#include "Vector.h"
#include "Nxt.h"

using namespace ecrobot;

extern "C"
{
	#include "balancer.h"
};

/**
 * �|���U�q����@��(�n���h���A�A�N�Z���A�u���[�L)
 *
 * Skill �ɂ���đ��삳���B
 */
class Activator
{
protected:
	Motor &mLeftMotor; //!< �����[�^
	Motor &mRightMotor; //!< �E���[�^
	GyroSensor &mGyroSensor; //!< �W���C���Z���T
	Nxt &mNxt; //!< NXT�I�u�W�F�N�g
    int mGyroOffset; //!< �W���C���Z���T�I�t�Z�b�g�l
public:
	Activator(Motor &leftMotor, 
              Motor &rightMotor, 
              GyroSensor &gyroSensor, 
              Nxt &nxt);
	~Activator(){}
    // �p�����^�[�̏�����
    void reset(int gyroOffset);
    // ���s�B�n���h���A�A�N�Z�X�̑���B
	void run(VectorT<F32> command);
    // �t�H���[�hPID�A�^�[��PID(@todo)�𗘗p�������s
	void runWithPid(VectorT<F32> speed);
    // ��~
	void stop();
protected:
    float mTargetSpeed;
    float mCurrentForward;
    float forwardPid(float targetSpeed);
};
#endif

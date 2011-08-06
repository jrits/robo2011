//
// Activator.cpp
//
#include "Activator.h"
#include "Pid.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 *
 * @param[in] leftMotor �����[�^
 * @param[in] rightMotor �E���[�^
 * @param[in] gyroSensor �W���C���Z���T
 * @param[in] nxt NXT�I�u�W�F�N�g
 */
Activator::Activator(Motor &leftMotor, 
                     Motor &rightMotor, 
                     GyroSensor &gyroSensor, 
                     Nxt &nxt) :
    mLeftMotor(leftMotor), 
    mRightMotor(rightMotor), 
    mGyroSensor(gyroSensor), 
    mNxt(nxt)
{
	mGyroOffset = USER_GYRO_OFFSET; //�I�t�Z�b�g�l��������
    mTargetSpeed = 0.0;
    mCurrentForward = 0.0;
}

/**
 * �p�����[�^�̏�����
 *
 * @param[in] gyroOffset �W���C���I�t�Z�b�g
 */
void Activator::reset(int gyroOffset)
{
    mGyroOffset = gyroOffset;
}

/**
 * ���s�B�n���h���A�A�N�Z���̑���B
 *
 * @param[in] command ���s�x�N�g��(forward, turn)
 */
void Activator::run(VectorT<F32> command)
{
	S8 pwm_L, pwm_R;

    // C++ �o�[�W�������ƂȂ��� mActivator.run() �œ����Ȃ��̂łƂ肠�����B
     balance_control(
         (float)command.mX,							 /* �O��i����(+:�O�i, -:��i) */
         (float)command.mY,							 /* ���񖽗�(+:�E����, -:������) */
         (float)ecrobot_get_gyro_sensor(NXT_PORT_S1), /* �W���C���Z���T�l */
         (float)USER_GYRO_OFFSET,                     /* �W���C���Z���T�I�t�Z�b�g�l */
         (float)nxt_motor_get_count(NXT_PORT_C),		 /* �����[�^��]�p�x[deg] */
         (float)nxt_motor_get_count(NXT_PORT_B),		 /* �E���[�^��]�p�x[deg] */
         (float)ecrobot_get_battery_voltage(),		 /* �o�b�e���d��[mV] */
         &pwm_L,										 /* �����[�^PWM�o�͒l */
         &pwm_R);									 /* �E���[�^PWM�o�͒l */

     if (! DESK_DEBUG) {
         nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); /* �����[�^PWM�o�̓Z�b�g(-100�`100) */
         nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); /* �E���[�^PWM�o�̓Z�b�g(-100�`100) */
     }

	// balance_control(
    //     (F32)command.mY, // �O��i����
    //     (F32)command.mY, // ���񖽗�
    //     (F32)mGyroSensor.get(),
    //     (F32)mGyroOffset,
    //     (F32)mLeftMotor.getCount(),
    //     (F32)mRightMotor.getCount(),
    //     (F32)mNxt.getBattMv(),
    //     &pwm_L,
    //     &pwm_R);
	
    // if (! DESK_DEBUG) {
    //     mLeftMotor.setPWM(pwm_L);
    //     mRightMotor.setPWM(pwm_R);
    // }

#if 0// DEBUG
    {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "Activator::run");
        lcd.putf("dn", (int)command.mX);
        lcd.putf("dn", (int)command.mY);
        lcd.putf("dn", (int)pwm_L);
        lcd.putf("dn", (int)pwm_R);
        lcd.disp();
    }
#endif

}

/**
 * �t�H���[�hPID�A�^�[��PID(@todo)�𗘗p�������s
 *
 * @param[in] speed �ڕW���s�X�s�[�h(encode/sec)
 */
void Activator::runWithPid(VectorT<F32> speed)
{
    VectorT<F32> command;
    command.mX = forwardPid(speed.mX);
    command.mY = speed.mY; // turnPid(speed.mY); // @todo
    run(command);
}

/**
 * �u���[�L�̑���B
 *
 * �o�����X�𐧌�͍s��Ȃ��B���[�^���~�߂邾���B
 *
 * @return -
 */
void Activator::stop()
{
	mLeftMotor.setPWM(0);
	mRightMotor.setPWM(0);
	mLeftMotor.setBrake(true);
	mRightMotor.setBrake(true);
}

// ���ߕ��@: ���ۂɑ��点�Ē��߁B
Pid mForwardPid(0.0001, 0.0, 0.0);

/**
 * �t�H���[�hPID
 *
 * ���s�X�s�[�h��PID���䂷��B
 *
 * @param speed ���҂���X�s�[�h(encode/sec)�B���720(�Q��])�Ń}�b�N�X�X�s�[�h�H
 * @return �t�H���[�h�l
 * @todo �X�s�[�h�̒P�ʂ�cm/sec�ɂ���
 */
float Activator::forwardPid(float targetSpeed)
{
    // ������(�������֐������̂��ʓ|�������̂ł�����)
    if (targetSpeed != mTargetSpeed) {
        mTargetSpeed = targetSpeed;
        // ���̊�l����PID�Ŕ���������B�W���� forward 100 �� 720(�Q��])/sec ���炢�Ɨ\�z���Č��߂����B
        mCurrentForward = (1/7.2) * mTargetSpeed;
    }
    
    // �ω���(encode/sec)�B���O���ƃX�s�[�h0�̉\�������邽�߁A������x���ԊԊu(5count)���������Ă���B
    float leftSpeed    = (mLeftMotorHistory.get()  - mLeftMotorHistory.get(-5))  / (0.004 * 5);
    float rightSpeed   = (mRightMotorHistory.get() - mRightMotorHistory.get(-5)) / (0.004 * 5);
    float currentSpeed = (leftSpeed + rightSpeed)/2.0;

    // PID����
    float P = targetSpeed - currentSpeed;
    mCurrentForward += mForwardPid.control(P);
    mCurrentForward = MAX(MIN(100, mCurrentForward), -100);

#if 1 // DEBUG
    {
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "ForwardPid");
        lcd.putf("dn", (int)targetSpeed);
        lcd.putf("dn", (int)currentSpeed);
        lcd.putf("dn", (int)mCurrentForward);
        lcd.putf("dn", (int)((1/7.2) * mTargetSpeed));
        lcd.disp();
    }
#endif

    return mCurrentForward;
}




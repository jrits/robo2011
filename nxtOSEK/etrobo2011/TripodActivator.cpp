//
// TripodActivator.cpp
//
#include "TripodActivator.h"
#include "constants.h"
#include "factory.h"

/**
 * �R���X�g���N�^
 *
 * @param[in] leftMotor �����[�^
 * @param[in] rightMotor �E���[�^
 * @param[in] gyroSensor �W���C���Z���T
 * @param[in] nxt NXT�I�u�W�F�N�g
 */
TripodActivator::TripodActivator(Motor &leftMotor, 
                                 Motor &rightMotor, 
                                 GyroSensor &gyroSensor, 
                                 Nxt &nxt) :
  mLeftMotor(leftMotor), 
  mRightMotor(rightMotor), 
  mGyroSensor(gyroSensor), 
  mNxt(nxt)
{
  mGyroOffset = USER_GYRO_OFFSET; //�I�t�Z�b�g�l��������
}

/**
 * �p�����[�^�̏�����
 *
 * @param[in] gyroOffset �W���C���I�t�Z�b�g
 */
void TripodActivator::reset(int gyroOffset)
{
  mGyroOffset = gyroOffset;
}
/**
 * �n���h���A�A�N�Z���̑���B
 *
 * �o�����X����͐���@��(TripodActivator)�������I�ɍs���Ă����B
 *
 * @param[in] command ���s�x�N�g��
 */
void TripodActivator::run(VectorT<F32> command)
{
  float pwmL, pwmR;

//   balance_control(
//     (F32)command.mX, // �O��i����
//     (F32)command.mY, // ���񖽗�
//     (F32)mGyroSensor.get(),
//     (F32)mGyroOffset,
//     (F32)mLeftMotor.getCount(),
//     (F32)mRightMotor.getCount(),
//     (F32)mNxt.getBattMv(),
//     &pwmL,
//     &pwmR);

  // @todo: balance_control �Ɠ������͒l�Ȃ瓯�����炢�̏o�͒l�ɂȂ�悤�ɂ�����
  pwmL = command.mX + (command.mY > 0 ? command.mY : 0) * 0.3;
  pwmR = command.mX + (-command.mY > 0 ? -command.mY : 0) * 0.3;

  if (! DESK_DEBUG) {
    mLeftMotor.setPWM((S8)(MIN(MAX(pwmL, -128), 127)));
    mRightMotor.setPWM((S8)(MIN(MAX(pwmR, -128), 127)));
  }

#if 1 // DEBUG
  static int count = 0;
  if (count++ > 5) {
    Lcd lcd;
    lcd.clear();
    lcd.putf("sn", "TripodActivator");
    lcd.putf("dn", (int)command.mX);
    lcd.putf("dn", (int)command.mY);
    lcd.putf("dn", (int)pwmL);
    lcd.putf("dn", (int)pwmR);
    lcd.disp();
  }
#endif
}

/**
 * �u���[�L�̑���B
 *
 * �o�����X�𐧌�͍s��Ȃ��B���[�^���~�߂邾���B
 *
 * @return -
 */
void TripodActivator::stop()
{
  mLeftMotor.setPWM(0);
  mRightMotor.setPWM(0);
  mLeftMotor.setBrake(true);
  mRightMotor.setBrake(true);
}


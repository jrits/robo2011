/* helloworld.c for TOPPERS/ATK(OSEK) */ 
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* �֐��v���g�^�C�v�錾 */
static void tail_control(signed int angle);
#define P_GAIN             2.5F /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60 /* ���S��~�p���[�^����PWM��΍ő�l */

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

TASK(OSEK_Task_Background)
{
  ecrobot_set_light_sensor_active(NXT_PORT_S3);
	while(1)
	{
    tail_control(70);
    ecrobot_status_monitor("OSEK HelloWorld!");
    systick_wait_ms(10); /* 10msec wait */
	}
}

//*****************************************************************************
// �֐��� : tail_control
// ����  : angle (���[�^�ڕW�p�x[�x])
// �Ԃ�l : ����
// �T�v : ���s�̊��S��~�p���[�^�̊p�x����
//*****************************************************************************
static void tail_control(signed int angle)
{
  float pwm = (float)(angle - nxt_motor_get_count(NXT_PORT_A))*P_GAIN; /* ��ᐧ�� */
  /* PWM�o�͖O�a���� */
  if (pwm > PWM_ABS_MAX)
  {
    pwm = PWM_ABS_MAX;
  }
  else if (pwm < -PWM_ABS_MAX)
  {
    pwm = -PWM_ABS_MAX;
  }

  nxt_motor_set_speed(NXT_PORT_A, (signed char)pwm, 1);
}

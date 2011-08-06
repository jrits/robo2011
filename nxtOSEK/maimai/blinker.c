//
// �t�@�C���� : blinker.c
//
//	�T�v       : �܂��܂��i���Łj�����C���g���[�X �T���v���v���O����
//

/* ITRON(TOPPERS/JSP) API */
#include "kernel.h"
#include "kernel_id.h"

/* Mindstorms NXT API */
#include "ecrobot_interface.h"

/* �|���U�q����p�����[�^ */
#include "balancer.h"


/* ���L�̃|�[�g��`�̓Z���T�[�ƃ��[�^�[�̔z���ɍ��킹�ĕύX����K�v������܂� */
#define MOTOR_RIGHT		NXT_PORT_B
#define MOTOR_LEFT		NXT_PORT_C
#define TOUCH_SENS		NXT_PORT_S2
#define LIGHT_SENS		NXT_PORT_S3
#define GYRO_SENS		NXT_PORT_S4


/* ���L�̃p�����[�^�̓Z���T��/���ɍ��킹�ă`���[�j���O����K�v������܂� */
#define GYRO_OFFSET  604 /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��)	*/
#define THRESHOLD    .7F /* ���C���G�b�W臒l								*/


static float brightness;	/* �R�[�X���x			*/
static U16 lightDowned;		/* �������̌��Z���T�[�l	*/
static U16 lightUpped;		/* �_�����̌��Z���T�[�l	*/


//*****************************************************************************
// FUNCTION		: jsp_systick_low_priority
// ARGUMENT		: none
// RETURN		: none
// DESCRIPTION 	: 1msec periodical ISR
//*****************************************************************************
void jsp_systick_low_priority(void)
{
	if (get_OS_flag()) /* check whether JSP already started or not */
	{
		isig_tim();          /* cyclic task dispatcher */
		check_NXT_buttons(); /* this must be called here */
	}
}



//*****************************************************************************
// FUNCTION		: ecrobot_device_initialize
// ARGUMENT		: none
// RETURN		: none
// DESCRIPTION 	: ECRobot device init hook function
//*****************************************************************************
void ecrobot_device_initialize(void)
{
	// Bluetooth������
	ecrobot_init_bt_slave("1234");	/* �p�X�L�[ "1234" */

	// ���[�^��~ �i�u���[�L���[�h�j
	nxt_motor_set_speed(MOTOR_LEFT,  0, 1);	/* �����[�^ */
	nxt_motor_set_speed(MOTOR_RIGHT, 0, 1);	/* �E���[�^ */
}



//*****************************************************************************
// FUNCTION		: ecrobot_device_terminate
// ARGUMENT		: none
// RETURN		: none
// DESCRIPTION 	: ECRobot device term hook function
//*****************************************************************************
void ecrobot_device_terminate(void)
{
	// Bluetooth�ؒf
	ecrobot_term_bt_connection();

	// ���[�^��~ �i�u���[�L���[�h�j
	nxt_motor_set_speed(MOTOR_LEFT,  0, 1);	/* �����[�^ */
	nxt_motor_set_speed(MOTOR_RIGHT, 0, 1);	/* �E���[�^ */

	// �u���[�L���������邽�߂̑҂�����
	systick_wait_ms(500);
}



//=============================================================================
// �^�X�N��	: TSK_STA
// �T�v		: �X�^�[�^�[�^�X�N
//
void tsk_starter(VP_INT exinf)
{
	balance_init();							/* �|���U�q���䏉����			*/
	nxt_motor_set_count(MOTOR_LEFT, 0);		/* �����[�^�G���R�[�_���Z�b�g	*/
	nxt_motor_set_count(MOTOR_RIGHT, 0);	/* �E���[�^�G���R�[�_���Z�b�g	*/

	/* �^�b�`�Z���T�����ҋ@ */
	while (ecrobot_get_touch_sensor(TOUCH_SENS) == 0) {
		;	/* DO NOTHING */
	}

	sta_cyc(CYC);	/* �����N���n���h���[�N��	*/

	ext_tsk();		/* ���^�X�N�̏I��			*/
}



//=============================================================================
// �n���h����	: CYC
// �T�v		: �����N���n���h��
//

void cyc(VP_INT exinf)
{
	U32 systime;

	systime = systick_get_ms();	// �V�X�e�������擾

	//
	// 4�~���b��������
	//
	switch (systime % 4) {
		case 0 :
			;					// DO NOTHING
			break;
		case 1 :
			iact_tsk(TSK_BLN);	// �|������TASK�N��
			iact_tsk(TSK_LOG);	// ���K�[TASK�N��
			break;
		case 2 :
		case 3 :
		default:
			;					// DO NOTHING
	}

	//
	// 40�~���b��������
	//
	switch (systime % 40) {
		case 0 :
			iact_tsk(TSK_LON);	// LED�_��TASK�N��
			iact_tsk(TSK_LMN);	// ���x�vTASK�N��
			break;
		case 20 :
			iact_tsk(TSK_LOF);	// LED����TASK�N��
			iact_tsk(TSK_LMN);	// ���x�vTASK�N��
			break;
		default :
			;					// DO NOTHING
	}
}



//=============================================================================
// �^�X�N��	: TSK_BLN
// �T�v		: �|���i�o�����T�[�j�^�X�N
//
void tsk_balancer(VP_INT exinf)
{
	signed char forward;		/* �O��i����: -100(��i)�`100(�O�i)		*/
	signed char turn;			/* ���񖽗�: -100(������)�`100(�E����)	*/
	signed char pwm_L, pwm_R;	/* ���E���[�^PWM�o��						*/

	forward = 0;	/* �O�i����(��~) */

	if (brightness < THRESHOLD) {
		turn = 50;  /* �E�� */
	} else {
		turn = -50; /* ���� */
	}

	/* �|���U�q���� */
	balance_control(
		(float)forward,                              /* �O��i����			*/
		(float)turn,                                 /* ���񖽗�				*/
		(float)ecrobot_get_gyro_sensor(GYRO_SENS),   /* �W���C���Z���T�l			*/
		(float)GYRO_OFFSET,                          /* �W���C���Z���T�I�t�Z�b�g�l	*/
		(float)nxt_motor_get_count(MOTOR_LEFT),      /* �����[�^��]�p�x[deg]	*/
		(float)nxt_motor_get_count(MOTOR_RIGHT),     /* �E���[�^��]�p�x[deg]	*/
		(float)ecrobot_get_battery_voltage(),        /* �o�b�e���d��[mV]		*/
		&pwm_L,  /* �����[�^PWM�o�͒l */
		&pwm_R); /* �E���[�^PWM�o�͒l */

	nxt_motor_set_speed(MOTOR_LEFT,  pwm_L, 1); /* �����[�^PWM�o�̓Z�b�g */
	nxt_motor_set_speed(MOTOR_RIGHT, pwm_R, 1); /* �E���[�^PWM�o�̓Z�b�g */

	/* ���^�X�N�̏I�� */
	ext_tsk();
}



//=============================================================================
// �^�X�N��	: TSK_LON
// �T�v		: LED�_���^�X�N
//
void tsk_lightOn(VP_INT exinf)
{
	/*�ԐFLED�������̎���ʌv�� */
	lightDowned = ecrobot_get_light_sensor(LIGHT_SENS);
	
	/* �ԐFLED��_������ */
	ecrobot_set_light_sensor_active(LIGHT_SENS);
	
	/* ���^�X�N�̏I�� */
	ext_tsk();
}



//=============================================================================
// �^�X�N��	: TSK_LOF
// �T�v		: LED�����^�X�N
//
void tsk_lightOff(VP_INT exinf)
{
	/* �ԐFLED�_�����̎���ʌv�� */
	lightUpped = ecrobot_get_light_sensor(LIGHT_SENS);
	
	/* �ԐFLED���������� */
	ecrobot_set_light_sensor_inactive(LIGHT_SENS);
	
	/* ���^�X�N�̏I�� */
	ext_tsk();
}



//=============================================================================
// �^�X�N��	: TSK_LMN
// �T�v		: ���x�v�i���~�m���[�^�[�j�^�X�N
//
void tsk_luminometer(VP_INT exinf)
{
	U16 lightDiff;				/* �_�����Ə������̕ω���	*/
	float k;					/* ���Z���T�[����`�␳�l	*/
	
	/* ���Z���T�[�̕ω��ʂ��v�Z */
	if (lightDowned - lightUpped > 0) {
		lightDiff = lightDowned - lightUpped;
	} else {
		lightDiff = 0U;
	}
	
	/* ���Z���T�[����`�␳�W�����v�Z �i�����f�[�^���j */
	k = (1.0382E-3 * lightDowned - 6.3295E-1) * lightDowned + 1.1024E+2;
	
	/* �R�[�X���x���v�Z */
	brightness = (float) lightDiff / k;
	
	/* ���^�X�N�̏I�� */
	ext_tsk();
}



//=============================================================================
// �^�X�N��	: TSK_LOG
// �T�v		: ���K�[�^�X�N
//
void tsk_logger(VP_INT exinf)
{
	S8 dat1;
	S8 dat2;
	
	dat1 = (S8)(brightness * 100);
	dat2 = 0;
	
	/* Bluetooth data logger */
	ecrobot_bt_data_logger(dat1, dat2);

	/* ���^�X�N�̏I�� */
	ext_tsk();
}

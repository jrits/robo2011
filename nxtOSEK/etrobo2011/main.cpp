//
// �t�@�C���� : main.cpp
//
//	�T�v       : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/ATK(OSEK)�p���C���v���O����
//

#include "constants.h"
#include "factory.h"
#include "Vector.h"
using namespace ecrobot;

extern "C"
{
#if 0
} // �G�f�B�^�̎����C���f���g�@�\�����܂����n�b�N
#endif

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/**
 * Bluetooth �ڑ�
 *
 * ���̊֐��Ŏw�肵��Bluetooth�f�o�C�X�����ڑ��惍�{�b�g�̃f�o�C�X���ɐݒ肳��܂��B
 *
 * @param[in] lcd Lcd�N���X�I�u�W�F�N�g
 * @param[in] bt_name Bluetooth�f�o�C�X��
 */
static void connect_bt(Lcd &lcd, char BT_NAME[16]);

/* ���L�̃}�N���͌�/���ɍ��킹�ĕύX����K�v������܂� */
/* sample_c1�}�N�� */
#define GYRO_OFFSET  605 /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��) */
#define LIGHT_WHITE	 500 /* ���F�̌��Z���T�l */
#define LIGHT_BLACK	 700 /* ���F�̌��Z���T�l */
/* sample_c2�}�N�� */
#define SONAR_ALERT_DISTANCE 30 /* �����g�Z���T�ɂ���Q�����m����[cm] */
/* sample_c3�}�N�� */
#define TAIL_ANGLE_STAND_UP 95 /* ���S��~���̊p�x[�x] */
#define TAIL_ANGLE_DRIVE      3 /* �o�����X���s���̊p�x[�x] */
#define P_GAIN             2.5F /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60 /* ���S��~�p���[�^����PWM��΍ő�l */
/* sample_c4�}�N�� */
#define DEVICE_NAME       "ET0"  /* Bluetooth�ʐM�p�f�o�C�X�� */
#define PASS_KEY          "1234" /* Bluetooth�ʐM�p�p�X�L�[ */
#define CMD_START         '1'    /* �����[�g�X�^�[�g�R�}���h(�ύX�֎~) */
/* Bluetooth�ʐM�p�f�[�^��M�o�b�t�@ */
char rx_buf[BT_MAX_RX_BUF_SIZE];

/* �֐��v���g�^�C�v�錾 */
static int sonar_alert(void);
static void tail_control(signed int angle);
static int remote_start(void);

//=============================================================================
// TOPPERS/ATK declarations
DeclareCounter(SysTimerCnt);
DeclareTask(TaskDrive);
DeclareEvent(EventDrive);
DeclareAlarm(AlarmDrive);
DeclareTask(TaskGps);
DeclareEvent(EventGps);
DeclareAlarm(AlarmGps);
DeclareTask(TaskHistory);
DeclareEvent(EventHistory);
DeclareAlarm(AlarmHistory);
DeclareTask(TaskSonar);
DeclareEvent(EventSonar);
DeclareAlarm(AlarmSonar);
DeclareTask(TaskLogger);
DeclareEvent(EventLogger);
DeclareAlarm(AlarmLogger);


//=============================================================================
// �֐���	: user_1ms_isr_type2
// ����		: �Ȃ�
// �߂�l	: �Ȃ�
// �T�v		: 1msec�������荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
//
void user_1ms_isr_type2(void)
{
  (void)SignalCounter(SysTimerCnt); // Increment System Timer Count
  SleeperMonitor(); // Clock�N���X�܂���I2C�f�o�C�X�N���X�g�p���ɕK�v
  // ENTER �{�^���� bluetooth �ڑ����L�����Z��
  if (mNxt.getButtons() == Nxt::ENTR_ON && !mBluetooth.isConnected()) {
    mBluetooth.cancelWaitForConnection();
  }
}

// ECRobot C API �f�o�C�X�̏�����
void ecrobot_device_initialize(void)
{
  ecrobot_set_light_sensor_active(NXT_PORT_S3); /* ���Z���T�ԐFLED��ON */
  ecrobot_init_sonar_sensor(NXT_PORT_S2); /* �����g�Z���T(I2C�ʐM)�������� */
  nxt_motor_set_count(NXT_PORT_A, 0); /* ���S��~�p���[�^�G���R�[�_���Z�b�g */
  ecrobot_init_bt_slave(PASS_KEY);
}

// ECRobot C API �f�o�C�X�̏I��
void ecrobot_device_terminate(void)
{
  ecrobot_set_light_sensor_inactive(NXT_PORT_S3); /* ���Z���T�ԐFLED��OFF */
  ecrobot_term_sonar_sensor(NXT_PORT_S2); /* �����g�Z���T(I2C�ʐM)���I�� */
  ecrobot_term_bt_connection(); /* Bluetooth�ʐM���I�� */
}

// �^�X�N�ԋ��L������
bool gDoSonar = false; //!< �\�i�[�Z���T�����t���O
int gSonarDistance = 255; //!< �\�i�[�Z���T�̌���
bool gSonarIsDetected = false; //!< �՗����m�̌���
bool gTouchStarter = false; //!< �^�b�`�Z���T�����t���O

/*
 * Sonar�^�X�N
 */
TASK(TaskSonar)
{
  //     // 48msec ���ɃC�x���g�ʒm����ݒ�
  //     SetRelAlarm(AlarmSonar, 1, 48); 
  //     WaitEvent(EventSonar);

  //     int gSonarDetectCount = 0;
  //     int gSonarTotalCount = 0;
  //     float gSonarDetectRatio = 0.0;
  //     while (1) {
  //         if (! gDoSonar) {
  //             gSonarDetectCount = 0;
  //             gSonarTotalCount = 0;
  //         }
  //     	if (gDoSonar) {
  //             gSonarDistance = mSonarSensor.getDistance();
  //             gSonarDetectCount += (gSonarDistance < 60);
  //             gSonarTotalCount++;
  //             gSonarDetectRatio = (gSonarDetectCount / (float)gSonarTotalCount);
  //             gSonarIsDetected = (gSonarDetectRatio > 0.1);
  //         }

  // #if 0 // DEBUG
  //         gDoSonar = true;
  //         static int count = 0;
  //         if (count++ > 5) {
  //             Lcd lcd;
  //             lcd.clear();
  //             lcd.putf("sn", "TaskSonar");
  //             lcd.putf("dn", gDoSonar);
  //             lcd.putf("dn", gSonarDistance);
  //             lcd.putf("dn", gSonarDetectCount);
  //             lcd.putf("dn", gSonarTotalCount);
  //             lcd.putf("dn", gSonarIsDetected);
  //             lcd.disp();
  //         }
  // #endif
  //         // �C�x���g�ʒm��҂�
  //         ClearEvent(EventSonar);
  //         WaitEvent(EventSonar);
  //    }
}

/**
 * ���s�^�X�N
 */
TASK(TaskDrive)
{
  signed char forward;      /* �O��i���� */
  signed char turn;         /* ���񖽗� */
  float pwm_L, pwm_R; /* ���E���[�^PWM�o�� */

  while(1)
  {
    tail_control(TAIL_ANGLE_STAND_UP); /* ���S��~�p�p�x�ɐ��� */
    if (ecrobot_get_touch_sensor(NXT_PORT_S4) == 1)
    {
      break; /* �^�b�`�Z���T�������ꂽ */
    }

    systick_wait_ms(10); /* 10msec�E�F�C�g */
  }

  balance_init(); /* �|���U�q���䏉���� */
  nxt_motor_set_count(NXT_PORT_C, 0); /* �����[�^�G���R�[�_���Z�b�g */
  nxt_motor_set_count(NXT_PORT_B, 0); /* �E���[�^�G���R�[�_���Z�b�g */

  static int count = 0;
  static VectorT<F32> command(30,0);
  while(1)
  {
    count++;
    if(count < 1000){
      tail_control(3); /* �o�����X���s�p�p�x�ɐ��� */
      mActivator.run(command);
    } else {
      mLookUpGateDriver.drive();
    }
    
    systick_wait_ms(4); /* 4msec�E�F�C�g */
  }
}

/*
 * GPS�X�V�^�X�N
 */
TASK(TaskGps)
{
  // 4msec ���ɃC�x���g�ʒm����ݒ�
  SetRelAlarm(AlarmGps, 1, 4); 
  WaitEvent(EventGps);

  while (1) {
    mGps.update();
    // �C�x���g�ʒm��҂�
    ClearEvent(EventGps);
    WaitEvent(EventGps);
  }
}

/*
 * History�X�V�^�X�N
 */
TASK(TaskHistory)
{
  // 4msec ���ɃC�x���g�ʒm����ݒ�
  SetRelAlarm(AlarmHistory, 1, 4); 
  WaitEvent(EventHistory);

  while (1) {
    mLightHistory.update(mLightSensor.get());
    mRightMotorHistory.update(mRightMotor.getCount());
    mLeftMotorHistory.update(mLeftMotor.getCount());
    mRightMotorSpeedHistory.update(mRightMotorHistory.calcDifference());
    mLeftMotorSpeedHistory.update(mLeftMotorHistory.calcDifference());
    mDirectionHistory.update(mGps.getDirection());
    mDirectionAverageHistory.update(mDirectionHistory.calcAverage());
    mGyroHistory.update(mGyroSensor.get());
    // �C�x���g�ʒm��҂�
    ClearEvent(EventHistory);
    WaitEvent(EventHistory);
  }
}

/*
 * Logger(bluetooth)�^�X�N
 */
TASK(TaskLogger)
{
  // 100msec ���ɃC�x���g�ʒm����ݒ�
  SetRelAlarm(AlarmLogger, 1, 100); 
  WaitEvent(EventLogger);

  while(!gTouchStarter);
  while (1) {
    if (LOGGER_SEND == 1) {
      mDaq.send(LOGGER_DATAS08, LOGGER_DATAS32[0]);
    } 
    else if (LOGGER_SEND == 2) {
      mDaq.send(LOGGER_DATAS08, LOGGER_DATAU16, LOGGER_DATAS16, LOGGER_DATAS32);
    }

    // �C�x���g�ʒm��҂�
    ClearEvent(EventLogger);
    WaitEvent(EventLogger);
  }
}

/**
 * Bluetooth �ڑ�
 *
 * ���̊֐��Ŏw�肵��Bluetooth�f�o�C�X�����ڑ��惍�{�b�g�̃f�o�C�X���ɐݒ肳��܂��B
 *
 * @param[in] lcd Lcd�N���X�I�u�W�F�N�g
 * @param[in] bt_name Bluetooth�f�o�C�X��
 */
static void connect_bt(Lcd &lcd, char bt_name[16])
{
  //CHAR  name[16]; 
  U8 address[7];

  lcd.clear();

  if (mBluetooth.getDeviceAddress(address)) // get the device address
  {
    lcd.putf("sn", "BD_ADDR:");
    for (SINT i=0; i<7; i++) lcd.putf("x", address[i],2);

    mBluetooth.setFriendlyName(bt_name); // set the friendly device name
    if (mBluetooth.getFriendlyName(bt_name)) // display the friendly device name
    {
      lcd.putf("nssn", "BT_NAME: ", bt_name);
    }

    lcd.putf("nsn", "Connecting BT...");
    lcd.putf("sn",  "ENTR to cancel.");
    lcd.disp();

    if (mBluetooth.waitForConnection("1234", 0)) // wait forever
    {
      lcd.putf("s", "Connected.");
    }
  }
  else
  {
    lcd.putf("s", "BT Failed.");
  }

  lcd.putf("ns", "Press Touch.");
  lcd.disp();
}

//*****************************************************************************
// �֐��� : sonar_alert
// ���� : ����
// �Ԃ�l : 1(��Q������)/0(��Q������)
// �T�v : �����g�Z���T�ɂ���Q�����m
//*****************************************************************************
static int sonar_alert(void)
{
  static unsigned int counter = 0;
  static int alert = 0;

  signed int distance;

  if (++counter == 40/4) /* ��40msec�������ɏ�Q�����m  */
  {
    /*
     * �����g�Z���T�ɂ�鋗����������́A�����g�̌��������Ɉˑ����܂��B
     * NXT�̏ꍇ�́A40msec�������x���o����̍ŒZ��������ł��B
     */
    distance = ecrobot_get_sonar_sensor(NXT_PORT_S2);
    if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
    {
      alert = 1; /* ��Q�������m */
    }
    else
    {
      alert = 0; /* ��Q������ */
    }
    counter = 0;
  }

  return alert;
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

//*****************************************************************************
// �֐��� : remote_start
// ���� : ����
// �Ԃ�l : 1(�X�^�[�g)/0(�ҋ@)
// �T�v : Bluetooth�ʐM�ɂ�郊���[�g�X�^�[�g�B Tera Term�Ȃǂ̃^�[�~�i���\�t�g����A
//       ASCII�R�[�h��1�𑗐M����ƁA�����[�g�X�^�[�g����B
//*****************************************************************************
static int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; /* ��M�o�b�t�@���N���A */
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		/* ��M�f�[�^���� */
		if (rx_buf[0] == CMD_START)
		{
			start = 1; /* ���s�J�n */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}

	return start;
}

};

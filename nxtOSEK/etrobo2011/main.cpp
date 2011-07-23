//
// �t�@�C���� : main.cpp
//
//    �T�v       : 2�֓|���U�q���C���g���[�X���{�b�g��TOPPERS/ATK(OSEK)�p���C���v���O����
//

#include "constants.h"
#include "factory.h"
using namespace ecrobot;

extern "C"
{
#if 0
} // �G�f�B�^�̎����C���f���g�@�\�����܂����n�b�N
#endif

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* sample_c3�}�N�� */
#define TAIL_ANGLE_STAND_UP 108 /* ���S��~���̊p�x[�x] */
#define TAIL_ANGLE_DRIVE      3 /* �o�����X���s���̊p�x[�x] */
#define P_GAIN             2.5F /* ���S��~�p���[�^������W�� */
#define PWM_ABS_MAX          60 /* ���S��~�p���[�^����PWM��΍ő�l */
/* sample_c4�}�N�� */
//#define DEVICE_NAME       "ET0"  /* Bluetooth�ʐM�p�f�o�C�X�� */
//#define PASS_KEY          "1234" /* Bluetooth�ʐM�p�p�X�L�[ */
#define CMD_START         '1'    /* �����[�g�X�^�[�g�R�}���h(�ύX�֎~) */

/**
 * Bluetooth �ڑ�
 *
 * ���̊֐��Ŏw�肵��Bluetooth�f�o�C�X�����ڑ��惍�{�b�g�̃f�o�C�X���ɐݒ肳��܂��B
 *
 * @param[in] lcd Lcd�N���X�I�u�W�F�N�g
 * @param[in] bt_name Bluetooth�f�o�C�X��
 */
static void connect_bt(Lcd &lcd, char BT_NAME[16]);
static void tail_control(signed int angle);
static int remote_start(void);

/* Bluetooth�ʐM�p�f�[�^��M�o�b�t�@ */
char rx_buf[BT_MAX_RX_BUF_SIZE];

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
// �֐���    : user_1ms_isr_type2
// ����        : �Ȃ�
// �߂�l    : �Ȃ�
// �T�v        : 1msec�������荞�݃t�b�N�֐�(OSEK ISR type2�J�e�S��)
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
    // ecrobot_init_sonar_sensor(NXT_PORT_S2);
}

// ECRobot C API �f�o�C�X�̏I��
void ecrobot_device_terminate(void)
{
    // ecrobot_term_sonar_sensor(NXT_PORT_S2);
}

// �^�X�N�ԋ��L������
bool gDoSonar = false; //!< �\�i�[�Z���T�����t���O
bool gSonarIsDetected = false; //!< �՗����m�̌���
int  gSonarTagetDistance = 0;
float gSonarTagetAngle = 0;

bool gTouchStarter = false; //!< �^�b�`�Z���T�����t���O

/*
 * Sonar�^�X�N
 */
/* ET���{�R��2011 �ǋL*/
// �O���^�X�N�ɂ��gDoSonar��true�ɐݒ肳�ꂽ�ہA�ȉ���3�̋��L�������̒l���X�V����
//
// bool  gSonarIsDetected        �^�[�Q�b�g���m�t���O�A��������true������ ��5cm�`60cm�݂̂����m����悤�ݒ肵�Ă���
// float gSonarTagetDistance    ���m�����^�[�Q�b�g�ƃ��{�̋����i�P�ʂ�GPS�ɂ��킹�ă~�����[�g���Ƃ����j
// float gSonarTagetAngle        ���m�����^�[�Q�b�g�̃��{���猩���p�x�i-180�`180�x�j
//
// �ۑ聗todo
// �T���v�����O����(80msec��)�y�сA���m�G���A(5cm�`60cm)�̑Ó����̌���
//  �������Ӂ� ��~��Ԃ̌��؂͈Ӗ����Ȃ��B���M�n���񒆂̌��؂��K�v
//   �����{���u���Ȃ�����񂵂Ă���ہA�ǂ̒��x�^�[�Q�b�g�����m���Ă���邩�H���ꂪET���o�̊́i�u�����ɐ���o����΂Ȃ��悢�j
TASK(TaskSonar)
{
    // 4msec ���ɃC�x���g�ʒm����ݒ�
    SetRelAlarm(AlarmSonar, 1, 80); 
    WaitEvent(EventSonar);

    int distance = 0;
    int timecounter = 0;
    
    while (1) {
        if(!gDoSonar){
            gSonarIsDetected = false;
        }
        
        if(gDoSonar){
            if(timecounter % (4/4) == 0) //80msec��
            {
                distance = mSonarSensor.getDistance();
                
                //if((5 < distance) && (distance < 60)){
                if((5 < distance) && (distance < 80)){
                    gSonarIsDetected = true;
                    gSonarTagetDistance = distance * 10;//�\�i�[��distance��cm�P�ʂȂ̂ŁAGPS�ɂ��킹�ďC��
                    gSonarTagetAngle = Gps::marge180(mGps.getDirection());
                    mSpeaker.playTone(1000, 1, 10);
                }
                else{
                    gSonarIsDetected = false;
                }
            }
            timecounter++;
        }
#if 1 // ���O���M(0�F�����A1�F���{)
        LOGGER_SEND = 2;
        LOGGER_DATAS08[0] = (S8)(gDoSonar); 
        LOGGER_DATAS08[1] = (S8)(gSonarIsDetected); 
        LOGGER_DATAU16    = (U16)(distance);
        LOGGER_DATAS16[0] = (S16)(mGps.getXCoordinate());
        LOGGER_DATAS16[1] = (S16)(mGps.getYCoordinate());
        LOGGER_DATAS16[2] = (S16)(mGps.getDirection());
        LOGGER_DATAS16[3] = (S16)(distance);
        LOGGER_DATAS32[0] = (S32)(distance);
        LOGGER_DATAS32[1] = (S32)(distance);
        LOGGER_DATAS32[2] = (S32)(gSonarTagetDistance);
        LOGGER_DATAS32[3] = (S32)(gSonarTagetAngle);
        
        mLcd.clear();
        //mLcd.putf("nsnn", "Get Ready?");
        //mLcd.putf("sdn",  "Light = ", (int)mLightSensor.get(), 5);//LightSensor�̒l��int�^5���ŕ\��
        //mLcd.putf("sdn",  "Gyro  = ", (int)mGyroSensor.get() , 5);//GyroSensor�̒l��int�^5���ŕ\��
        mLcd.putf("sd" ,  "Sonar = ",  distance, 5);//���܂������Ȃ��̂ŃR�����g�A�E�g
        mLcd.disp();
#endif
        // �C�x���g�ʒm��҂�
        ClearEvent(EventSonar);
        WaitEvent(EventSonar);
    }
}
/**
 * ���s�^�X�N
 */
TASK(TaskDrive)
{
    // 4msec ���ɃC�x���g�ʒm����ݒ�
    SetRelAlarm(AlarmDrive, 1, 4); 
    WaitEvent(EventDrive);
//     K_THETADOT = 10.5F;


    //connect_bt(mLcd, BT_NAME); // bluetooth�ڑ�
    mActivator.reset(USER_GYRO_OFFSET);

    while(1){
        tail_control(TAIL_ANGLE_STAND_UP); /* ���S��~�p�p�x�ɐ��� */
        if (remote_start() == 1)
        {
            break; // �����[�g�X�^�[�g
        }
        if (gTouchStarter = mTouchSensor.isPressed())
        {
            break; /* �^�b�`�Z���T�������ꂽ */
        }
#if 1 // �L�����u���[�V�����p�f�B�X�v���\��(0�F�����A1�F���{)
        //gDoSonar = true;//���܂������Ȃ��̂ŃR�����g�A�E�g
        mLcd.clear();
        mLcd.putf("nsnn", "Get Ready?");
        mLcd.putf("sdn",  "Light = ", (int)mLightSensor.get(), 5);//LightSensor�̒l��int�^5���ŕ\��
        mLcd.putf("sdn",  "Gyro  = ", (int)mGyroSensor.get() , 5);//GyroSensor�̒l��int�^5���ŕ\��
        //mLcd.putf("sd" ,  "Sonar = ",  gSonarTagetDistance, 5);//���܂������Ȃ��̂ŃR�����g�A�E�g
        mLcd.disp();
#endif
        
        systick_wait_ms(10); /* 10msec�E�F�C�g */
    }
#if 1 // �L�����u���[�V�����p�f�B�X�v���\��(0�F�����A1�F���{)
        mLcd.clear();
        //gDoSonar = true;//�L�����u���[�V����
#endif
    bool doDrive = true;
    while (1) {
        tail_control(TAIL_ANGLE_DRIVE); /* �o�����X���s�p�p�x�ɐ��� */
        if (mFailDetector.detect()) doDrive = false;
        if (doDrive) mCourse->drive();
        else mActivator.stop();

        // �C�x���g�ʒm��҂�
        ClearEvent(EventDrive);
        WaitEvent(EventDrive);
        
    }
    TerminateTask();
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
// �֐��� : tail_control
// ���� : angle (���[�^�ڕW�p�x[�x])
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

    rx_len = mBluetooth.receive(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
    if (rx_len > 0)
    {
        /* ��M�f�[�^���� */
        if (rx_buf[0] == CMD_START)
        {
            start = 1; /* ���s�J�n */
        }
        //mBluetooth.send(rx_buf, 0, rx_len); //��M�f�[�^���G�R�[�o�b�N ���K�[�ɃS�~�����肻���Ȃ̂ŃR�����g�A�E�g
    }

    return start;
}

};

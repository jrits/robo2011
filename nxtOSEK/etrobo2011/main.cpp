//
// ファイル名 : main.cpp
//
//    概要       : 2輪倒立振子ライントレースロボットのTOPPERS/ATK(OSEK)用メインプログラム
//

#include "constants.h"
#include "factory.h"
using namespace ecrobot;

extern "C"
{
#if 0
} // エディタの自動インデント機能をごまかすハック
#endif

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* sample_c3マクロ */
#define TAIL_ANGLE_STAND_UP 108 /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */
/* sample_c4マクロ */
//#define DEVICE_NAME       "ET0"  /* Bluetooth通信用デバイス名 */
//#define PASS_KEY          "1234" /* Bluetooth通信用パスキー */
#define CMD_START         '1'    /* リモートスタートコマンド(変更禁止) */

/**
 * Bluetooth 接続
 *
 * この関数で指定したBluetoothデバイス名が接続先ロボットのデバイス名に設定されます。
 *
 * @param[in] lcd Lcdクラスオブジェクト
 * @param[in] bt_name Bluetoothデバイス名
 */
static void connect_bt(Lcd &lcd, char BT_NAME[16]);
static void tail_control(signed int angle);
static int remote_start(void);

/* Bluetooth通信用データ受信バッファ */
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
// 関数名    : user_1ms_isr_type2
// 引数        : なし
// 戻り値    : なし
// 概要        : 1msec周期割り込みフック関数(OSEK ISR type2カテゴリ)
//
void user_1ms_isr_type2(void)
{
    (void)SignalCounter(SysTimerCnt); // Increment System Timer Count
    SleeperMonitor(); // ClockクラスまたはI2Cデバイスクラス使用時に必要
    // ENTER ボタンで bluetooth 接続をキャンセル
    if (mNxt.getButtons() == Nxt::ENTR_ON && !mBluetooth.isConnected()) {
        mBluetooth.cancelWaitForConnection();
    }
}

// ECRobot C API デバイスの初期化
void ecrobot_device_initialize(void)
{
    // ecrobot_init_sonar_sensor(NXT_PORT_S2);
}

// ECRobot C API デバイスの終了
void ecrobot_device_terminate(void)
{
    // ecrobot_term_sonar_sensor(NXT_PORT_S2);
}

// タスク間共有メモリ
bool gDoSonar = false; //!< ソナーセンサ発動フラグ
bool gSonarIsDetected = false; //!< 衝立検知の結果
int  gSonarTagetDistance = 0;
float gSonarTagetAngle = 0;

bool gTouchStarter = false; //!< タッチセンサ押下フラグ

/*
 * Sonarタスク
 */
/* ETロボコン2011 追記*/
// 外部タスクによりgDoSonarがtrueに設定された際、以下の3つの共有メモリの値を更新する
//
// bool  gSonarIsDetected        ターゲット検知フラグ、見つけたらtrueが入る ※5cm〜60cmのみを検知するよう設定している
// float gSonarTagetDistance    検知したターゲットとロボの距離（単位はGPSにあわせてミリメートルとした）
// float gSonarTagetAngle        検知したターゲットのロボから見た角度（-180〜180度）
//
// 課題＠todo
// サンプリング周期(80msec毎)及び、検知エリア(5cm〜60cm)の妥当性の検証
//  →☆注意☆ 停止状態の検証は意味がない。超信地旋回中の検証が必要
//   →ロボがブレながら旋回している際、どの程度ターゲットを検知してくれるか？これがET相撲の肝（ブレずに旋回出来ればなおよい）
TASK(TaskSonar)
{
    // 80msec 毎にイベント通知する設定
    SetRelAlarm(AlarmSonar, 1, 60); 
    WaitEvent(EventSonar);

    int distance = 0;
    int timecounter = 0;
    
    while (1) {
        if(!gDoSonar){
            gSonarIsDetected = false;
        }
        
        if(gDoSonar){
            distance = mSonarSensor.getDistance();
            if((5 < distance) && (distance < 60)){
                gSonarIsDetected = true;
                gSonarTagetDistance = distance * 10;//ソナーのdistanceはcm単位なので、GPSにあわせて修正
                gSonarTagetAngle = Gps::marge180(mGps.getDirection());
                mSpeaker.playTone(1000, 1, 10);
            }
            else{
                gSonarIsDetected = false;
            }
            timecounter++;
        }
#if 0 // ログ送信(0：解除、1：実施)
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
        //mLcd.putf("sdn",  "Light = ", (int)mLightSensor.get(), 5);//LightSensorの値をint型5桁で表示
        //mLcd.putf("sdn",  "Gyro  = ", (int)mGyroSensor.get() , 5);//GyroSensorの値をint型5桁で表示
        mLcd.putf("sd" ,  "Sonar = ",  distance, 5);//うまくいかないのでコメントアウト
        mLcd.disp();
#endif
        // イベント通知を待つ
        ClearEvent(EventSonar);
        WaitEvent(EventSonar);
    }
}
/**
 * 走行タスク
 */
TASK(TaskDrive)
{
    // 4msec 毎にイベント通知する設定
    SetRelAlarm(AlarmDrive, 1, 4); 
    WaitEvent(EventDrive);
//     K_THETADOT = 10.5F;


    //connect_bt(mLcd, BT_NAME); // bluetooth接続
    mActivator.reset(USER_GYRO_OFFSET);

    while(1){
        tail_control(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */
        if (remote_start() == 1)
        {
            break; // リモートスタート
        }
        if (gTouchStarter = mTouchSensor.isPressed())
        {
            break; /* タッチセンサが押された */
        }
#if 1 // キャリブレーション用ディスプレ表示(0：解除、1：実施)
        //gDoSonar = true;//うまくいかないのでコメントアウト
        mLcd.clear();
        mLcd.putf("nsnn", "Get Ready?");
        mLcd.putf("sdn",  "Light = ", (int)mLightSensor.get(), 5);//LightSensorの値をint型5桁で表示
        mLcd.putf("sdn",  "Gyro  = ", (int)mGyroSensor.get() , 5);//GyroSensorの値をint型5桁で表示
        //mLcd.putf("sd" ,  "Sonar = ",  gSonarTagetDistance, 5);//うまくいかないのでコメントアウト
        mLcd.disp();
#endif
        systick_wait_ms(10); /* 10msecウェイト */
    }
#if 1 // キャリブレーション用ディスプレ表示(0：解除、1：実施)
        mLcd.clear();
        //gDoSonar = true;//キャリブレーション
#endif
    bool doDrive = true;
    while (1) {
        tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
        if (mFailDetector.detect()) doDrive = false;
        if (doDrive) mCourse->drive();
        else mActivator.stop();

        // イベント通知を待つ
        ClearEvent(EventDrive);
        WaitEvent(EventDrive);
        
    }
    TerminateTask();
}

/*
 * GPS更新タスク
 */
TASK(TaskGps)
{
    // 4msec 毎にイベント通知する設定
    SetRelAlarm(AlarmGps, 1, 4); 
    WaitEvent(EventGps);

    while (1) {
        mGps.update();
        // イベント通知を待つ
        ClearEvent(EventGps);
        WaitEvent(EventGps);
    }
}

/*
 * History更新タスク
 */
TASK(TaskHistory)
{
    // 4msec 毎にイベント通知する設定
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
        // イベント通知を待つ
        ClearEvent(EventHistory);
        WaitEvent(EventHistory);
    }
}

/*
 * Logger(bluetooth)タスク
 */
TASK(TaskLogger)
{
    // 100msec 毎にイベント通知する設定
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

        // イベント通知を待つ
        ClearEvent(EventLogger);
        WaitEvent(EventLogger);
    }
}

/**
 * Bluetooth 接続
 *
 * この関数で指定したBluetoothデバイス名が接続先ロボットのデバイス名に設定されます。
 *
 * @param[in] lcd Lcdクラスオブジェクト
 * @param[in] bt_name Bluetoothデバイス名
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
// 関数名 : tail_control
// 引数 : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
static void tail_control(signed int angle)
{
    float pwm = (float)(angle - nxt_motor_get_count(NXT_PORT_A))*P_GAIN; /* 比例制御 */
    /* PWM出力飽和処理 */
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
// 関数名 : remote_start
// 引数 : 無し
// 返り値 : 1(スタート)/0(待機)
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
static int remote_start(void)
{
    int i;
    unsigned int rx_len;
    unsigned char start = 0;

    for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
    {
        rx_buf[i] = 0; /* 受信バッファをクリア */
    }

    rx_len = mBluetooth.receive(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
    if (rx_len > 0)
    {
        /* 受信データあり */
        if (rx_buf[0] == CMD_START)
        {
            start = 1; /* 走行開始 */
        }
        //mBluetooth.send(rx_buf, 0, rx_len); //受信データをエコーバック ロガーにゴミが入りそうなのでコメントアウト
    }

    return start;
}

};

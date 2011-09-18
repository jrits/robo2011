//
// ファイル名 : main.cpp
//
//	概要		: 2輪倒立振子ライントレースロボットのTOPPERS/ATK(OSEK)用メインプログラム
//

#include "define.h"
#include "constants.h"
#include "factory.h"
#include "Vector.h"
using namespace ecrobot;

extern "C"
{
#if 0
} // エディタの自動インデント機能をごまかすハック
#endif

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/**
 * Bluetooth 接続
 *
 * この関数で指定したBluetoothデバイス名が接続先ロボットのデバイス名に設定されます。
 *
 * @param[in] lcd Lcdクラスオブジェクト
 * @param[in] bt_name Bluetoothデバイス名
 */
static void connect_bt(Lcd &lcd, char BT_NAME[16]);

/* Bluetooth スタート */
#define PASS_KEY          "1234" /* Bluetooth通信用パスキー */
#define CMD_START         '1'    /* リモートスタートコマンド(変更禁止) */
char rx_buf[BT_MAX_RX_BUF_SIZE]; /* Bluetooth通信用データ受信バッファ */
static int remote_start(void);

/* まいまい式差分計算 */
static float calc_maimai(U16 light_off_value, U16 light_on_value);

// タスク間共有メモリ
bool gDoSonar = false; //!< ソナーセンサ発動フラグ
int gSonarDistance = 255; //!< ソナーセンサの結果
bool gSonarIsDetected = false; //!< 衝立検知の結果
bool gTouchStarter = false; //!< タッチセンサ押下フラグ
bool gDoMaimai = false; //!< まいまい式発動フラグ
float gMaimaiValue = 0.0;  //!< まいまい式の結果
bool gDoForwardPid = false; //!< フォワードPID発動フラグ(暫定)
bool gDoProgressiveTurn = false; //!< 過去のturn値をベースにしたトレース(暫定)
int  gSonarTagetDistance = 0; // ETsumo
float gSonarTagetAngle = 0; // ETsumo

//=============================================================================
// TOPPERS/ATK declarations
DeclareCounter(SysTimerCnt);
DeclareTask(TaskDrive);
DeclareEvent(EventDrive);
DeclareAlarm(AlarmDrive);
DeclareTask(TaskMaimai);
DeclareEvent(EventMaimai);
DeclareAlarm(AlarmMaimai);
DeclareTask(TaskGps);
DeclareEvent(EventGps);
DeclareAlarm(AlarmGps);
DeclareTask(TaskSonar);
DeclareEvent(EventSonar);
DeclareAlarm(AlarmSonar);
DeclareTask(TaskLogger);
DeclareEvent(EventLogger);
DeclareAlarm(AlarmLogger);


//=============================================================================
// 関数名	: user_1ms_isr_type2
// 引数		: なし
// 戻り値	: なし
// 概要		: 1msec周期割り込みフック関数(OSEK ISR type2カテゴリ)
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
  ecrobot_set_light_sensor_active(NXT_PORT_S3); /* 光センサ赤色LEDをON */
  //ecrobot_init_sonar_sensor(NXT_PORT_S2); /* 超音波センサ(I2C通信)を初期化 */
  nxt_motor_set_count(NXT_PORT_A, 0); /* 完全停止用モータエンコーダリセット */
  ecrobot_init_bt_slave(PASS_KEY);
}

// ECRobot C API デバイスの終了
void ecrobot_device_terminate(void)
{
  ecrobot_set_light_sensor_inactive(NXT_PORT_S3); /* 光センサ赤色LEDをOFF */
  //ecrobot_term_sonar_sensor(NXT_PORT_S2); /* 超音波センサ(I2C通信)を終了 */
  ecrobot_term_bt_connection(); /* Bluetooth通信を終了 */
}

/*
 * Sonarタスク
 */
/* ETロボコン2011 追記*/
// 外部タスクによりgDoSonarがtrueに設定された際、以下の3つの共有メモリの値を更新する
//
// bool  gSonarIsDetected        ターゲット検知フラグ、見つけたらtrueが入る ※5cm-60cmのみを検知するよう設定している
// float gSonarTagetDistance    検知したターゲットとロボの距離（単位はGPSにあわせてミリメートルとした）
// float gSonarTagetAngle        検知したターゲットのロボから見た角度（-180-180度）
//
// 課題＠todo
// サンプリング周期(80msec毎)及び、検知エリア(5cm縲鰀60cm)の妥当性の検証
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
        LOGGER_DATAS32[0] = (S32)(mLeftMotor.getCount());
        LOGGER_DATAS32[1] = (S32)(mRightMotor.getCount());
        LOGGER_DATAS32[2] = (S32)(gSonarTagetDistance);
        LOGGER_DATAS32[3] = (S32)(gSonarTagetAngle);
        
        mLcd.clear();
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
    // K_THETADOT = 10.5F;

    connect_bt(mLcd, BT_NAME); // bluetooth接続
    mActivator.reset(USER_GYRO_OFFSET);

    while(1)
    {
        tail_control(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */

        if (ecrobot_get_touch_sensor(NXT_PORT_S4) == 1 || remote_start() == 1)
        {
            gTouchStarter = true;
            break; /* タッチセンサが押された */
        }
        systick_wait_ms(10); /* 10msecウェイト */
#if 1 // キャリブレーション用ディスプレ表示(0：解除、1：実施)
        //gDoSonar = true;//うまくいかないのでコメントアウト
        mLcd.clear();
        mLcd.putf("snn", "Get Ready?");
#ifdef KACHIROBO
        mLcd.putf("sn",  "Robot   = ROBO");
#endif
#ifdef KACHIICHI
        mLcd.putf("sn",  "Robot   = ICHI");
#endif
#ifdef KACHINEW
        mLcd.putf("sn",  "Robot   = NEW");
#endif
        mLcd.putf("ssn",  "Course  = ", (mCourse->getNameId() == 0 ? "In" : "Out"));
        mLcd.putf("sdn",  "Section = ", (int)mCourse->getState(), 5);
        mLcd.putf("sdn",  "Light   = ", (int)mLightSensor.get(), 5);//LightSensorの値をint型5桁で表示
        mLcd.putf("sdn",  "Gyro    = ", (int)mGyroSensor.get() , 5);//GyroSensorの値をint型5桁で表示
        mLcd.putf("sdn",  "Maimai  = ", (int)(gMaimaiValue * 100), 5);
        //mLcd.putf("sd" ,  "Sonar = ",  gSonarTagetDistance, 5);//うまくいかないのでコメントアウト
        mLcd.disp();
#endif
    }

    balance_init();                     /* 倒立振子制御初期化 */
    nxt_motor_set_count(NXT_PORT_C, 0); /* 左モータエンコーダリセット */
    nxt_motor_set_count(NXT_PORT_B, 0); /* 右モータエンコーダリセット */

    bool doDrive = true;
    while(1)
    {
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
 * Maimaiタスク
 */
TASK(TaskMaimai)
{
    // MAIMAI_PERIOD msec 毎にイベント通知する
    SetRelAlarm(AlarmMaimai, 1, MAIMAI_PERIOD);
    WaitEvent(EventMaimai);

    bool  is_light_on = 1;          /* 光センサの点灯/消灯状態   */
    U16   light_value[2] = {0, 0};  /* 0:消灯時、1:点灯時の光センサー値 */

    while(1)
    {
          if (! gDoMaimai) {
            ecrobot_set_light_sensor_active(NXT_PORT_S3);
            ClearEvent(EventMaimai);
            WaitEvent(EventMaimai);
            continue;
          }

          // MAIMAI(改): 光センサの値(0:消灯時または1:点灯時)を取得。
          light_value[is_light_on] = ecrobot_get_light_sensor(NXT_PORT_S3);
        
          // MAIMAI(改): まいまい式差分計算
          gMaimaiValue = calc_maimai(light_value[0], light_value[1]);

#if 0 // DEBUG
        {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn"  "MAIMAI*100");
            lcd.putf("dn", (int)(gMaimaiValue*100));
            lcd.putf("dn", (int)is_light_on);
            lcd.putf("dn", (int)light_value[0]);
            lcd.putf("dn", (int)light_value[1]);
            lcd.disp();
        }
#endif
        // MAIMAI(改): 光センサ明滅
        if (is_light_on) {
          ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
          is_light_on = 0;
        } else {
          ecrobot_set_light_sensor_active(NXT_PORT_S3);
          is_light_on = 1;
        }

        ClearEvent(EventMaimai);
        WaitEvent(EventMaimai);
    }
}

/*
 * GPS/History更新タスク
 */
TASK(TaskGps)
{
    // 4msec 毎にイベント通知する設定
    SetRelAlarm(AlarmGps, 1, 4); 
    WaitEvent(EventGps);

    while (1) {
        mLightHistory.update(mLightSensor.get());
        mRightMotorHistory.update(mRightMotor.getCount());
        mLeftMotorHistory.update(mLeftMotor.getCount());
        mRightMotorSpeedHistory.update(mRightMotorHistory.calcDifference());
        mLeftMotorSpeedHistory.update(mLeftMotorHistory.calcDifference());
        mDirectionHistory.update(mGps.getDirection());
        mDirectionAverageHistory.update(mDirectionHistory.calcAverage());
        mGyroHistory.update(mGyroSensor.get());
        mGps.update();
        // イベント通知を待つ
        ClearEvent(EventGps);
        WaitEvent(EventGps);
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
// 引数  : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
extern void tail_control(signed int angle)
{
    float pwm = (float)(angle - nxt_motor_get_count(NXT_PORT_A))*TAIL_P_GAIN; /* 比例制御 */
    /* PWM出力飽和処理 */
    if (pwm > TAIL_PWM_ABS_MAX)
    {
        pwm = TAIL_PWM_ABS_MAX;
    }
    else if (pwm < -TAIL_PWM_ABS_MAX)
    {
        pwm = -TAIL_PWM_ABS_MAX;
    }

    nxt_motor_set_speed(NXT_PORT_A, (signed char)pwm, 1);
}

//*****************************************************************************
// 関数名 : calc_maimai
// 引数  : light_off_value (消灯時の光センサー値) light_on_value(点灯時の光センサー値)
// 返り値 : コース明度 (差分計算などの結果)
// 概要 : 明度計（ルミノメーター）
//*****************************************************************************
static float calc_maimai(U16 light_off_value, U16 light_on_value)
{
    float luminance;  /* コース明度 */
    U16 light_diff;   /* 点灯時と消灯時の変化量 */
    float k;          /* 光センサー非線形補正値 */
    
    /* 光センサーの変化量を計算 */
    if (light_off_value - light_on_value > 0) {
        light_diff = light_off_value - light_on_value;
    } else {
        light_diff = 0U;
    }
    
    /* 光センサー非線形補正係数を計算 （実験データより） */
    k = (1.0382E-3 * light_off_value - 6.3295E-1) * light_off_value + 1.1024E+2;
    
    /* コース明度を計算 */
    luminance = (float) light_diff / k;
    return luminance;
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

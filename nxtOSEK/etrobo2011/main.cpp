//
// ファイル名 : main.cpp
//
//	概要       : 2輪倒立振子ライントレースロボットのTOPPERS/ATK(OSEK)用メインプログラム
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

/**
 * Bluetooth 接続
 *
 * この関数で指定したBluetoothデバイス名が接続先ロボットのデバイス名に設定されます。
 *
 * @param[in] lcd Lcdクラスオブジェクト
 * @param[in] bt_name Bluetoothデバイス名
 */
static void connect_bt(Lcd &lcd, char BT_NAME[16]);

/* 下記のマクロは個体/環境に合わせて変更する必要があります */
/* sample_c1マクロ */
#define GYRO_OFFSET  611 /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define LIGHT_WHITE	 500 /* 白色の光センサ値 */
#define LIGHT_BLACK	 700 /* 黒色の光センサ値 */
/* sample_c2マクロ */
#define SONAR_ALERT_DISTANCE 30 /* 超音波センサによる障害物検知距離[cm] */
/* sample_c3マクロ */
#define TAIL_ANGLE_STAND_UP 108 /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */
/* sample_c4マクロ */
#define DEVICE_NAME       "ET0"  /* Bluetooth通信用デバイス名 */
#define PASS_KEY          "1234" /* Bluetooth通信用パスキー */
#define CMD_START         '1'    /* リモートスタートコマンド(変更禁止) */
/* Bluetooth通信用データ受信バッファ */
char rx_buf[BT_MAX_RX_BUF_SIZE];

/* 関数プロトタイプ宣言 */
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
  ecrobot_init_sonar_sensor(NXT_PORT_S2); /* 超音波センサ(I2C通信)を初期化 */
  nxt_motor_set_count(NXT_PORT_A, 0); /* 完全停止用モータエンコーダリセット */
  ecrobot_init_bt_slave(PASS_KEY);
}

// ECRobot C API デバイスの終了
void ecrobot_device_terminate(void)
{
  ecrobot_set_light_sensor_inactive(NXT_PORT_S3); /* 光センサ赤色LEDをOFF */
  ecrobot_term_sonar_sensor(NXT_PORT_S2); /* 超音波センサ(I2C通信)を終了 */
  ecrobot_term_bt_connection(); /* Bluetooth通信を終了 */
}

// タスク間共有メモリ
bool gDoSonar = false; //!< ソナーセンサ発動フラグ
int gSonarDistance = 255; //!< ソナーセンサの結果
bool gSonarIsDetected = false; //!< 衝立検知の結果
bool gTouchStarter = false; //!< タッチセンサ押下フラグ

/*
 * Sonarタスク
 */
TASK(TaskSonar)
{
//     // 48msec 毎にイベント通知する設定
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
//         // イベント通知を待つ
//         ClearEvent(EventSonar);
//         WaitEvent(EventSonar);
//    }
}

/**
 * 走行タスク
 */
TASK(TaskDrive)
{
//     // 4msec 毎にイベント通知する設定
//     SetRelAlarm(AlarmDrive, 1, 4); 
//     WaitEvent(EventDrive);
// //     K_THETADOT = 10.5F;

//     //connect_bt(mLcd, BT_NAME); // bluetooth接続
//     mActivator.reset(USER_GYRO_OFFSET);

//     while(!(gTouchStarter = mTouchSensor.isPressed()));
//     bool doDrive = true;
//     while (1) {
//         if (mFailDetector.detect()) doDrive = false;
//         if (doDrive) mCourse->drive();
//     	else mActivator.stop();

//         // イベント通知を待つ
//         ClearEvent(EventDrive);
//         WaitEvent(EventDrive);
        
//     }
//     TerminateTask();
	signed char forward;      /* 前後進命令 */
	signed char turn;         /* 旋回命令 */
	signed char pwm_L, pwm_R; /* 左右モータPWM出力 */

	while(1)
	{
		tail_control(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */

		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == 1 || remote_start() == 1)
		{
			break; /* タッチセンサが押された */
		}

		systick_wait_ms(10); /* 10msecウェイト */
	}

	balance_init();						/* 倒立振子制御初期化 */
	nxt_motor_set_count(NXT_PORT_C, 0); /* 左モータエンコーダリセット */
	nxt_motor_set_count(NXT_PORT_B, 0); /* 右モータエンコーダリセット */
    VectorT<float> command(50, 0);
	while(1)
	{
		tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
        mActivator.runWithPid(command);
        // mActivator.run(command);

		// if (sonar_alert() == 1) /* 障害物検知 */
		// {
		// 	forward = turn = 0; /* 障害物を検知したら停止 */
		// }
		// else
		// {
		// 	forward = 50; /* 前進命令 */
		// 	if (ecrobot_get_light_sensor(NXT_PORT_S3) <= (LIGHT_WHITE + LIGHT_BLACK)/2)
		// 	{
		// 		turn = 50;  /* 右旋回命令 */
		// 	}
		// 	else
		// 	{
		// 		turn = -50; /* 左旋回命令 */
		// 	}
		// }

		 /* 倒立振子制御(forward = 0, turn = 0で静止バランス) */
		 // balance_control(
		 // 	(float)forward,								 /* 前後進命令(+:前進, -:後進) */
		 // 	(float)turn,								 /* 旋回命令(+:右旋回, -:左旋回) */
		 // 	(float)ecrobot_get_gyro_sensor(NXT_PORT_S1), /* ジャイロセンサ値 */
		 // 	(float)GYRO_OFFSET,							 /* ジャイロセンサオフセット値 */
		 // 	(float)nxt_motor_get_count(NXT_PORT_C),		 /* 左モータ回転角度[deg] */
		 // 	(float)nxt_motor_get_count(NXT_PORT_B),		 /* 右モータ回転角度[deg] */
		 // 	(float)ecrobot_get_battery_voltage(),		 /* バッテリ電圧[mV] */
		 // 	&pwm_L,										 /* 左モータPWM出力値 */
		 // 	&pwm_R);									 /* 右モータPWM出力値 */
		 // nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); /* 左モータPWM出力セット(-100〜100) */
		 // nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); /* 右モータPWM出力セット(-100〜100) */

		systick_wait_ms(4); /* 4msecウェイト */
	}
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
// 関数名 : sonar_alert
// 引数 : 無し
// 返り値 : 1(障害物あり)/0(障害物無し)
// 概要 : 超音波センサによる障害物検知
//*****************************************************************************
static int sonar_alert(void)
{
	static unsigned int counter = 0;
	static int alert = 0;

	signed int distance;

	if (++counter == 40/4) /* 約40msec周期毎に障害物検知  */
	{
		/*
		 * 超音波センサによる距離測定周期は、超音波の減衰特性に依存します。
		 * NXTの場合は、40msec周期程度が経験上の最短測定周期です。
		 */
		distance = ecrobot_get_sonar_sensor(NXT_PORT_S2);
		if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
		{
			alert = 1; /* 障害物を検知 */
		}
		else
		{
			alert = 0; /* 障害物無し */
		}
		counter = 0;
	}

	return alert;
}

//*****************************************************************************
// 関数名 : tail_control
// 引数  : angle (モータ目標角度[度])
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

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		/* 受信データあり */
		if (rx_buf[0] == CMD_START)
		{
			start = 1; /* 走行開始 */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* 受信データをエコーバック */
	}

	return start;
}

};

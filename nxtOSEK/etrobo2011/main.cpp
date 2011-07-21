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
// bool  gSonarIsDetected		ターゲット検知フラグ、見つけたらtrueが入る ※5cm～60cmのみを検知するよう設定している
// float gSonarTagetDistance	検知したターゲットとロボの距離（単位はGPSにあわせてミリメートル単位とした）
// float gSonarTagetAngle		検知したターゲットのロボから見た角度（-180～180度）
//
// 課題＠todo
// サンプリング周期(80msec毎)及び、検知エリア(5cm～60cm)の妥当性の検証
//  →☆注意☆ 停止状態の検証は意味がない。超信地旋回中の検証が必要
//   →ロボがブレながら旋回している際、どの程度ターゲットを検知してくれるか？これがET相撲の肝（ブレずに旋回出来ればなおよい）
TASK(TaskSonar)
{
    // 4msec 毎にイベント通知する設定
    SetRelAlarm(AlarmSonar, 1, 4); 
    WaitEvent(EventSonar);

	int distance = 0;
	int timecounter = 0;
	
    while (1) {
    	if(!gDoSonar){
    		gSonarIsDetected = false;
    	}
    	
    	if(gDoSonar){
    		
    		if(timecounter % (80/4) == 0) //80msec毎
    		{
		    	distance = mSonarSensor.getDistance();
		    	
		    	if((5 < distance) && (distance < 60)){
		    		gSonarIsDetected = true;
		    		gSonarTagetDistance = distance * 10;//ソナーのdistanceはcm単位なので、GPSにあわせて修正
		    		gSonarTagetAngle = Gps::marge180(mGps.getDirection());
		    	}
	    		else{
	    			gSonarIsDetected = false;
	    		}
    		}
    		timecounter++;
    	}
#if 0 // DEBUG
        gDoSonar = true;
        static int count = 0;
        if (count++ > 5) {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "TaskSonar");
            lcd.putf("dn", gDoSonar);
            lcd.putf("dn", gSonarDistance);
            lcd.putf("dn", gSonarDetectCount);
            lcd.putf("dn", gSonarTotalCount);
            lcd.putf("dn", gSonarIsDetected);
            lcd.disp();
        }
#endif
        // イベント通知を待つ
        ClearEvent(EventSonar);
        WaitEvent(EventSonar);
    }
}
/* ETロボコン2011 追記ここまで */

/*ETロボコン2011 コメントアウト
TASK(TaskSonar)
{
    // 48msec 毎にイベント通知する設定
    SetRelAlarm(AlarmSonar, 1, 48); 
    WaitEvent(EventSonar);

    int gSonarDetectCount = 0;
    int gSonarTotalCount = 0;
    float gSonarDetectRatio = 0.0;
    while (1) {
        if (! gDoSonar) {
            gSonarDetectCount = 0;
            gSonarTotalCount = 0;
        }
    	if (gDoSonar) {
            gSonarDistance = mSonarSensor.getDistance();
            gSonarDetectCount += (gSonarDistance < 60);
            gSonarTotalCount++;
            gSonarDetectRatio = (gSonarDetectCount / (float)gSonarTotalCount);
            gSonarIsDetected = (gSonarDetectRatio > 0.1);
        }

#if 0 // DEBUG
        gDoSonar = true;
        static int count = 0;
        if (count++ > 5) {
            Lcd lcd;
            lcd.clear();
            lcd.putf("sn", "TaskSonar");
            lcd.putf("dn", gDoSonar);
            lcd.putf("dn", gSonarDistance);
            lcd.putf("dn", gSonarDetectCount);
            lcd.putf("dn", gSonarTotalCount);
            lcd.putf("dn", gSonarIsDetected);
            lcd.disp();
        }
#endif
        // イベント通知を待つ
        ClearEvent(EventSonar);
        WaitEvent(EventSonar);
    }
}
ETロボコン2011 コメントアウトここまで
*/

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

    while(!(gTouchStarter = mTouchSensor.isPressed()));
    bool doDrive = true;
    while (1) {
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

};

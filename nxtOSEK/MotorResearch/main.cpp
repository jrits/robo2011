// NXTway-GS++ sample.cpp for TOPPERS/ATK(OSEK)

#include <stddef.h>

#define PWM_RESEARCH // モータに直接PWM値をセットして調査
//#define PWM_VERIFICATION // バランス制御を使用した検証

// モータ補正係数設定
float PWM_L = 1.0;
float PWM_R = 1.0;

// 車体設定
char BT_NAME[16] = "KACHIROBO";
int USER_GYRO_OFFSET = 605; // この値は適宜調整すること
//char BT_NAME[16] = "KACHIICHI";
//int USER_GYRO_OFFSET = 585; // この値は適宜調整すること
//char BT_NAME[16] = "KACHINEW";
//int USER_GYRO_OFFSET = 608; // この値は適宜調整すること

// ECRobot++ API
#include "TouchSensor.h"
#include "GyroSensor.h"
#include "LightSensor.h"
#include "Motor.h"
#include "Nxt.h"
#include "Clock.h"
#include "Lcd.h"
#include "Bluetooth.h"
#include "Daq.h"
#include "TouchStarter.h"
#include "Vector.h"
using namespace ecrobot;
Nxt         mNxt;
TouchSensor mTouchSensor(PORT_1);
LightSensor mLightSensor(PORT_3);
GyroSensor  mGyroSensor(PORT_4);
Motor       mLeftMotor(PORT_B);
Motor       mRightMotor(PORT_C);
Lcd         mLcd;
Bluetooth   mBluetooth;
Clock       mClock;
Daq         mDaq(mBluetooth);

S8  DEBUG_DATA08[2];
S32 DEBUG_DATA32;

extern "C"
{
#if 0
} // エディタのインデントをだますハック
#endif
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"

static void connect_bt(Lcd &lcd, char bt_name[16]);

//=============================================================================
// TOPPERS/ATK declarations
DeclareCounter(SysTimerCnt);
DeclareTask(TaskDrive);
DeclareAlarm(AlarmDrive);
DeclareEvent(EventDrive);

//=============================================================================
// 1msec timer interrupt hook
void user_1ms_isr_type2(void)
{
    (void)SignalCounter(SysTimerCnt); // Alarm counter

    SleeperMonitor(); // Need for Nxt and I2C device classes

    if (mNxt.getButtons() == Nxt::ENTR_ON && !mBluetooth.isConnected())
    {
        mBluetooth.cancelWaitForConnection(); // Cancel Bluetooth connection process
    }
}

//=============================================================================
// Drive Task
TASK(TaskDrive)
{
    SetRelAlarm(AlarmDrive, 1, 4); // Set 10msec periodical Alarm
    WaitEvent(EventDrive); // waiting for "Ready Go"

    connect_bt(mLcd, BT_NAME); // Set up Blutooth connection with data logger.
    TouchStarter(mTouchSensor).waitForReadyGo();  // Touch sensor is used to start driving

#ifdef PWM_RESEARCH
    int duration = 1200; // 同速度継続時間
    int pwm = -100; // 初期フォワード値
    int time = 0; // 時間カウンタ
    int prevLeftCount = 0; // 前回左モータエンコーダ値
    int prevRightCount = 0; // 前回右モータエンコーダ値
    while(1)
    {
        mLeftMotor.setPWM(pwm);
        mRightMotor.setPWM(pwm);

        if (time % 25 == 0) {
            mLcd.clear();
            mLcd.putf("dn", time);
            mLcd.putf("dn", pwm);
            mLcd.putf("dn", mLeftMotor.getCount());
            mLcd.putf("dn", mRightMotor.getCount());
            mLcd.disp();
        }

        if (time > duration/2) {
            // 速度が安定してからエンコーダ値の記録を開始
            prevLeftCount = mLeftMotor.getCount();
            prevRightCount = mRightMotor.getCount();
        }
        if (time > duration) {
            // データ送信
            {
                S8 dataS08[2] = {0, 0};
                U16 dataU16 = 0;
                S16 dataS16[4] = {0, 0, 0, 0};
                S32 dataS32[4] = {0, 0, 0, 0};
                dataS32[0] = pwm;
                dataS32[1] = mLeftMotor.getCount()  - prevLeftCount; // 左変化量(速度)
                dataS32[2] = mRightMotor.getCount() - prevRightCount; // 右変化量速度)
                mDaq.send(dataS08, dataU16, dataS16, dataS32);
            }
            // 次のPWMへ
            pwm += 10; // 10 ずつ増加
            time = 0;
            // 終了
            if (pwm > 100) break;
        }
        time++;

        ClearEvent(EventDrive); // Clear the drive event
        WaitEvent(EventDrive);  // Wait for drive event
    }
#endif
#ifdef PWM_VERIFICATION
    // 実際に走らせて確認してみる。
    int duration = 1000; // 同速度継続時間
    int time = 0; // 時間カウンタ
    VectorT<F32> command(10, 0);
    while(1)
    {
        time++;
        S8 pwmL, pwmR;
        
        if (time > duration) {
            time = 0;
            command.mX += 10; // 10 ずつ増加
            if (command.mX > 100) command.mX = 100;
        }

        balance_control(
            (F32)command.mX,
            (F32)command.mY,
            (F32)mGyroSensor.get(),
            (F32)USER_GYRO_OFFSET,
            (F32)mLeftMotor.getCount(),
            (F32)mRightMotor.getCount(),
            (F32)mNxt.getBattMv(),
            &pwmL,
            &pwmR);
        
        mLeftMotor.setPWM(pwmL);
        mRightMotor.setPWM(pwmR);
        
        if (time % 25 == 0) {
            mLcd.clear();
            mLcd.putf("dn", time);
            mLcd.putf("dn", command.mX);
            mLcd.putf("dn", pwmL);
            mLcd.putf("dn", pwmR);
            mLcd.putf("dn", mLeftMotor.getCount());
            mLcd.putf("dn", mRightMotor.getCount());
            mLcd.disp();
        }

        ClearEvent(EventDrive); // Clear the drive event
        WaitEvent(EventDrive);  // Wait for drive event
    }
#endif
}

//=============================================================================
// Sub functions
static void connect_bt(Lcd &lcd, char bt_name[16])
{
    //CHAR  name[16] = "Kachi";
    U8 address[7];

    lcd.clear();

    if (mBluetooth.getDeviceAddress(address)) // get the device address
    {
        lcd.putf("sn", "BD_ADDR:");
        for (SINT i=0; i<7; i++) lcd.putf("x", address[i],2);

        mBluetooth.setFriendlyName(bt_name); // set the friendly device name
        if (mBluetooth.getFriendlyName(bt_name)) // display the friendly device name
        {
            lcd.putf("nssn", "BD_NAME: ", bt_name);
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

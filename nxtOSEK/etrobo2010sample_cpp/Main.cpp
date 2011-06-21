//
// ファイル名 : Main.cpp
// 概要       : 2輪倒立振子ライントレースロボットのTOPPERS/ATK1(OSEK)用C++サンプルプログラム
//

// ECRobot C++ API
#include "TouchSensor.h"
#include "SonarSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Nxt.h"
#include "Clock.h"
#include "Lcd.h"
#include "Speaker.h"
using namespace ecrobot;

#include "NXTway_GS.h"
#include "Driver.h"
#include "StartStopDriver.h"
#include "SonarDriver.h"
#include "LineTraceDriver.h"
#include "DriverManager.h"

//=============================================================================
// デバイスオブジェクトの定義
TouchSensor touch(PORT_1);
SonarSensor sonar(PORT_2);
LightSensor light(PORT_3);
GyroSensor   gyro(PORT_4);
Motor      motorR(PORT_B);
Motor      motorL(PORT_C);
Clock clock;
Lcd lcd;
Nxt nxt;
Speaker speaker;

// 複数タスクからコールされるドライバ
StartStopDriver ssDriver;
SonarDriver sonarDriver;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

//=============================================================================
// TOPPERS/ATK1の宣言
DeclareCounter(SysTimerCnt);
DeclareAlarm(Alarm4msec);
DeclareEvent(EventDrive);

// タスクの起動周期[msec]
#define DRIVE_PERIOD       (4)
#define BACKGROUND_PERIOD (48)

//=============================================================================
// 関数名	: user_1ms_isr_type2
// 引数	: なし
// 戻り値	: なし
// 概要	: 1msec周期割り込みフック関数(OSEK ISR type2カテゴリ)
//=============================================================================
void user_1ms_isr_type2(void)
{
	(void)SignalCounter(SysTimerCnt); // Alarmハンドラ
	SleeperMonitor(); // NxtおよびI2Cデバイスの使用時に必要
}

//=============================================================================
// タスク名　: TaskMain
// 概要	　: メインタスク
//=============================================================================
TASK(TaskMain)
{
	lcd.clear();
	lcd.putf("s", "TOUCH:START/STOP");
	lcd.disp();

	SetRelAlarm(Alarm4msec, 1, DRIVE_PERIOD); // ドライブ用Alarmタイマーのセット

	while(1)
	{
		ssDriver.checkInput(touch.isPressed()); // タッチセンサの状態を検出
		sonarDriver.checkObstacles(sonar); // 超音波センサによる障害物検出

		clock.wait(BACKGROUND_PERIOD); // ウェイトループ
	}
}

//=============================================================================
// タスク名　: TaskDrive
// 概要	　: ドライブタスク
//=============================================================================
TASK(TaskDrive)
{
	LineTraceDriver ltDriver(light);
	DriverManager drivers;
	(void)drivers.createDriverTable(3);
	(void)drivers.add(&ssDriver); // 最高優先度
	(void)drivers.add(&sonarDriver);
	(void)drivers.add(&ltDriver); // 最低優先度

	NXTway_GS robot(nxt, gyro, motorL, motorR);

	while(1)
	{
        ClearEvent(EventDrive); // ドライブイベントのクリア
		WaitEvent(EventDrive);  // ドライブイベント待ち

		drivers.notify(robot.readInternalData()); // 走行体の内部データをドライバーへ通知
		drivers.update(); // ドライバリクエストを取得する前に更新
		switch(drivers.getRequest())
		{
			case Driver::STOP:
				robot.stop();
				break;
			case Driver::START:
				CancelAlarm(Alarm4msec);            // ドライブ用Alarmタイマーのキャンセル

				robot.reset(robot.calGyroOffset()); // ジャイロセンサのキャリブレーションに約400msecかかる
				drivers.reset();
				speaker.playTone(440U, 500U, 30U);  // リセット完了通知ブザー

				SetRelAlarm(Alarm4msec, 1, DRIVE_PERIOD); // ドライブ用Alarmタイマーのセット
				break;
			case Driver::DRIVE:
				robot.drive(drivers.getCommand());
				break;
			case Driver::NO_REQ:
			default:
				break; // Do nothing
		}
	}
}
};


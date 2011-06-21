//
// ファイル名 : sample.cpp
//
//	概要       : 2輪倒立振子ライントレースロボットのTOPPERS/ATK(OSEK)用サンプルプログラム
//

// ECRobot++ API
#include "TouchSensor.h"
#include "LightSensor.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "Nxt.h"
using namespace ecrobot;

#include "TouchStarter.h"
#include "NXTway_GS.h"
#include "LineTrace.h"

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

//=============================================================================
// デバイスオブジェクトの生成
//
TouchSensor touch(PORT_1);
LightSensor light(PORT_3);
GyroSensor   gyro(PORT_4);
Motor motorL(PORT_C);
Motor motorR(PORT_B);

#define USER_GYRO_OFFSET (610) // この値は適宜調整すること

//=============================================================================
// 関数名	: user_1ms_isr_type2
// 引数		: なし
// 戻り値	: なし
// 概要		: 1msec周期割り込みフック関数(OSEK ISR type2カテゴリ)
//
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // ClockクラスまたはI2Cデバイスクラス使用時に必要
}

//=============================================================================
// タスク名	: TaskMain
// 概要		: メインタスク
//
TASK(TaskMain)
{
	Clock clock;
	Nxt nxt;

	LineTrace lineTraceDriver(light); // ライントレースドライバー
	NXTway_GS robot(nxt, gyro, motorL, motorR); // 走行体

	TouchStarter(touch).waitForReadyGo(); // タッチセンサを走行開始判定に使用

	robot.reset(USER_GYRO_OFFSET); // 走行体の制御初期化

	while(1)
	{
		robot.drive(&lineTraceDriver); // ライントレース運転
		clock.wait(4);                 // 4msecウェイト
	}
}

};

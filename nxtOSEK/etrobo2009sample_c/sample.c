/**
 ******************************************************************************
 **	ファイル名 : sample.c
 **
 **	概要       : 2輪倒立振子ライントレースロボットのTOPPERS/ATK(OSEK)用サンプルプログラム
 **
 ******************************************************************************
 **/
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* 倒立振子制御用ヘッダファイル */

/* 下記のパラメータはセンサ個体/環境に合わせてチューニングする必要があります */
#define GYRO_OFFSET  610 /* ジャイロセンサオフセット値(角速度0[deg/sec]時)  */
#define WHITE		 500 /* 白色の光センサ値 */
#define BLACK		 700 /* 黒色の光センサ値 */

//*****************************************************************************
// 関数名	: user_1ms_isr_type2
// 引数		: なし
// 戻り値	: なし
// 概要		: 1msec周期割り込みフック関数(OSEK ISR type2カテゴリ)
//*****************************************************************************
void user_1ms_isr_type2(void){}

//*****************************************************************************
// タスク名	: TaskMain
// 概要		: メインタスク
//*****************************************************************************
TASK(TaskMain)
{
	signed char forward;      /* 前後進命令: -100(後進)～100(前進)   */
	signed char turn;         /* 旋回命令: -100(左旋回)～100(右旋回) */
	signed char pwm_L, pwm_R; /* 左右モータPWM出力 */

	ecrobot_set_light_sensor_active(NXT_PORT_S3);       /* 光センサ赤色LEDをON */
	while (ecrobot_get_touch_sensor(NXT_PORT_S1) == 0); /* タッチセンサ押下待機 */

	balance_init();						/* 倒立振子制御初期化 */
	nxt_motor_set_count(NXT_PORT_C, 0); /* 左モータエンコーダリセット */
	nxt_motor_set_count(NXT_PORT_B, 0); /* 右モータエンコーダリセット */
	forward = 50;						/* 前進命令 */
	while(1)
	{
		if (ecrobot_get_light_sensor(NXT_PORT_S3) <= (WHITE + BLACK)/2)
			turn = 50;  /* 右折 */
		else
			turn = -50; /* 左折 */

		/* 倒立振子制御(forward = 0, turn = 0で静止バランス) */
		balance_control(
			(float)forward,                              /* 前後進命令                 */
			(float)turn,                                 /* 旋回命令                   */
			(float)ecrobot_get_gyro_sensor(NXT_PORT_S4), /* ジャイロセンサ値           */
			(float)GYRO_OFFSET,                          /* ジャイロセンサオフセット値 */
			(float)nxt_motor_get_count(NXT_PORT_C),      /* 左モータ回転角度[deg]      */
			(float)nxt_motor_get_count(NXT_PORT_B),      /* 右モータ回転角度[deg]      */
			(float)ecrobot_get_battery_voltage(),        /* バッテリ電圧[mV]           */
			&pwm_L,  /* 左モータPWM出力値 */
			&pwm_R); /* 右モータPWM出力値 */
		nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); /* 左モータPWM出力セット */
		nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); /* 右モータPWM出力セット */

		systick_wait_ms(4); /* 4msecウエイト */
	}
}

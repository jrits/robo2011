/* helloworld.c for TOPPERS/ATK(OSEK) */ 
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* 関数プロトタイプ宣言 */
static void tail_control(signed int angle);
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

TASK(OSEK_Task_Background)
{
  ecrobot_set_light_sensor_active(NXT_PORT_S3);
	while(1)
	{
    tail_control(70);
    ecrobot_status_monitor("OSEK HelloWorld!");
    systick_wait_ms(10); /* 10msec wait */
	}
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

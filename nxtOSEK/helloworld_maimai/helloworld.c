/* helloworld.c for TOPPERS/ATK(OSEK) */ 
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* 尻尾コントロール */
#define TAIL_P_GAIN        2.5F /* 完全停止用モータ制御比例係数 */
#define TAIL_PWM_ABS_MAX   60   /* 完全停止用モータ制御PWM絶対最大値 */
static void tail_control(signed int angle);

/* まいまい式差分計算 */
int gDoMaimai = 0;              /* まいまい式を利用 */
float gMaimaiValue;             /* まいまい式の結果 */
#define MAIMAI_PERIOD 10        /* まいまいタスクの周期 */
static float calc_maimai(U16 light_off_value, U16 light_on_value);

//=============================================================================
// TOPPERS/ATK declarations
DeclareCounter(SysTimerCnt);
DeclareTask(TaskMaimai);
DeclareEvent(EventMaimai);
DeclareAlarm(AlarmMaimai);

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void){ /* do nothing */ }

TASK(OSEK_Task_Background)
{
    ecrobot_set_light_sensor_active(NXT_PORT_S3);
    gDoMaimai = 1;
    while(1)
    {
        tail_control(70);
        ecrobot_status_monitor("OSEK HelloWorld!");
        systick_wait_ms(10); /* 10msec wait */
    }
}

/*
 * Maimaiタスク
 */
TASK(TaskMaimai)
{
    // MAIMAI_PERIOD msec 毎にイベント通知する
    SetRelAlarm(AlarmMaimai, 1, MAIMAI_PERIOD);
    WaitEvent(EventMaimai);

    int  is_light_on = 1;          /* 光センサの点灯/消灯状態   */
    U16  light_value[2] = {0, 0};  /* 0:消灯時、1:点灯時の光センサー値 */

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

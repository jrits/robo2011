//
// Pid.h
//
#include"Pid.h"

/**
 * コンストラクタ
 *
 * @param[in] kp 比例係数
 * @param[in] ki 積分係数
 * @param[in] kd 微分係数
 */
Pid::Pid(float kp, float ki, float kd) : 
    mKp(kp), mKi(ki), mKd(kd)
{
	index = 0;
	for(int i = 0; i < MAX_BUF_SIZE; i++)
	{
		buf[i] = 0;
	}
}

/**
 * デストラクタ
 */
Pid::~Pid()
{
}

/**
 * PID制御による操作量の計算
 *
 * @param[in] P 比例成分
 * @return 操作量
 */
float Pid::control(float P)
{
    // Nothing but buffering (Ring bufffer)
    float pP = buf[index]; //oldest value
    buf[index] = P;  // newest value
	index = (index + 1) % MAX_BUF_SIZE;
    
    // Integral
	// I += P;
    float I = 0;
	for(int i = 0; i < MAX_BUF_SIZE; i++)
	{
        I += buf[i];
    }

    //Derivative
	float D = P - pP;

	float Y = (mKp * P) + (mKi * I) + (mKd * D);
	
    return Y;
}

//
// Pid.h
//
#ifndef PID_H_
#define PID_H_

#include "constants.h"

#define MAX_BUF_SIZE 10

/**
 * PID 制御クラス
 */
class Pid
{
private:
	float mKp; //!< 比例係数
	float mKi; //!< 積分係数
	float mKd; //!< 微分係数
	int index; //!< リングバッファの現在インデックス
	float buf[MAX_BUF_SIZE]; //!< リングバッファのバッファ
public:
	Pid(float kp, float ki, float kd);
	virtual ~Pid();
public:
	float control(float P);
};

#endif

//
// Gps.cpp
//

#include "GpsTest.h"

//Historyクラス完成後は、各メソッド内で宣言
float prevXCoordinate = 0.0;
float prevYCoordinate = 0.0;
float prevEl = 0.0;
float prevEr = 0.0;


Gps::Gps(Motor &aMotorL, Motor &aMotorR) :
	motorL(aMotorL), 
	motorR(aMotorR),
    mXCoordinate(100.0),
    mYCoordinate(-100.0),
    mDirection(180.0),
	mDistance(0.0),
	mWheelRadius(4.05),
	mXOffset(0.0),
	mWheelDistance(16.20),
	mYOffset(0.0),
	mDirectionOffset(0.0),
	mDistanceOffset(0.0),
	//以下補正関係変数,扱い不明(クラス図にはない変数)
    timeCounterHelper(0),timeCounter(0),xHistory(0),
	yHistory(0),mDirectionHistory(0),posFlag(0)
{

}

/**
 * PID制御
 *
 * @param[in] P 比例成分(偏差)
 * @return 操作量
 */
 
void Gps::update()
{
	float angle = 0.0;  /* 4msecごとの車体角度の変化量 (度) */
	float radius = 0.0; /* 4msecごとの車体の描く円の半径 */
	float el = 0.0; /* 4msec間のエンコーダー値の変化量(left) */
	float er = 0.0; /* 4msec間のエンコーダー値の変化量(right) */
	float currEl = 0.0; /* 現在のエンコーダー値(left) */
	float currEr = 0.0; /* 現在のエンコーダー値(right) */
	timeCounter++;
	
		
/*********   値取得   *********/
	/* エンコーダー値を取得 */
	motorL.setCount();
	motorR.setCount();
	currEl = motorL.getCount();
	currEr = motorR.getCount();
	
	/* 4msec間のエンコーダー値の変化量を計算 */
	el = currEl - prevEl;
	er = currEr - prevEr;
/********* 値取得終了 *********/
	
/********* 座標計算 ***********/
	angle = calcAngle(el, er); /* 曲がった角度を計算 */
	
	radius = calcRadius(el, angle); /* 走行体の描く円の半径を計算 */
	
	calcCoordinates(angle, radius, el); /* 座標の更新 */
	
	calcDirection(angle); /* 現在向いている方向の更新 */
/********* 座標計算終了 *******/

/******* prevEl,Erの更新 ******/
	prevEl = currEl;
	prevEr = currEr;
/**** prevEl,Erの更新の終了****/
	
/************ 座標補正 *****************/
	//lcd.clear();
	//lcd.putf("d", );
	//lcd .disp();
	/*
	if(timeCounter%50 !=0)
	{	
		xHistory += mXCoordinate;
		yHistory += mYCoordinate;
		mDirectionHistory += mDirection;
	}
	else
	{
		xHistory = (xHistory+mXCoordinate)/timeCounter;
		yHistory = (yHistory + mYCoordinate)/timeCounter;
		mDirectionHistory = (mDirectionHistory + mDirection)/timeCounter;
		if(!gAngleTraceFlag)
			adjustPosition(xHistory, yHistory, mDirectionHistory);
		xHistory = 0;
		yHistory = 0;
		mDirectionHistory = 0;
		timeCounter = 0;

	}
	*/
	prevEl = currEl;
	prevEr = currEr;
}

/******************************/
/* calcmDirection              */
/******************************/
/* 走行体の向きを計算         */
/******************************/
void Gps::calcDirection(float angle)
{
	mDirection += angle;
}

/******************************/
/* calcAngle                  */
/******************************/
/* 走行体の曲がった角度を計算 */
/******************************/
float Gps::calcAngle(float el, float er)
{
	/* スタート時の正面を0度として、上から見て反時計回りするたびに値が増加 */
	/* 時計回りで減少 */
	/* 一周360度 */

	float angle = (mWheelRadius * (er - el)) / mWheelDistance;
	if(isinf(angle))
	{
		angle = 0;
	}

	return angle;
}

/******************************/
/* calcRadius                 */
/******************************/
/* 走行体の描く円の半径を計算 */
/******************************/
float Gps::calcRadius(float encoderLeft, float angle)
{
	/* ある点からある点へ移動する際に描く円の半径を計算 */
	
	float gpsRadius = 0.0;
	
	if(angle > 0)
	{
		gpsRadius = ((encoderLeft * mWheelRadius) / angle) + (mWheelDistance / 2);
	}
	else if(angle < 0)
	{
		gpsRadius = ((encoderLeft * mWheelRadius / angle)) * (-1) - (mWheelDistance / 2);
	}
	else //angle = 0
	{
		gpsRadius = 0;
	}
	if(isinf(gpsRadius))
	{
		//エラー処理。将来的にはヒストリクラスから直近の値を参照して挿入するのが良いか？
		gpsRadius = 0;
	}
	
	return gpsRadius;
}

/******************************/
/* calcmDistance               */
/******************************/
/* 走行体の移動した距離を計算 */
/*                            */
/******************************/ 
void Gps::calcDistance()
{
	
	float distance;
	float x = mXCoordinate - prevXCoordinate;
	float y = mYCoordinate - prevYCoordinate;
	distance = sqrt(x*x + y*y);
	mDistance += distance;
	if(isinf(mDistance))
	{
		//エラー処理
		//mDistance = -1;
	}
	
}

/**********************/
/* calcCoordinates    */
/**********************/
/* x座標、y座標を計算 */
/**********************/
void Gps::calcCoordinates(float angle, float radius, float encoderL)
{	
	
	float x = 0;
	float y = 0;
	float *circleX = &x, *circleY = &y;
	bool result = false;
	
	/* 走行体の描く円の中心の座標を求める */
	result = calcCircleCoordinates(angle, radius, circleX, circleY);
	
	/* 座標の計算 */
	
	/* prev */
	prevXCoordinate = mXCoordinate;
	prevYCoordinate = mYCoordinate;
	if(!result)/* 直進の場合 */
	{
		mXCoordinate = prevXCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * PI * (cos(degreeToRadian(marge360(mDirection))));
		mYCoordinate = prevYCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * PI * (sin(degreeToRadian(marge360(mDirection))));
	}
	else/* 円を描く場合 */ 
	{
		mXCoordinate = (cos(degreeToRadian(angle))*(prevXCoordinate - *circleX)) - (sin(degreeToRadian(angle))*(mYCoordinate - *circleY)) + *circleX;
		mYCoordinate = (sin(degreeToRadian(angle))*(prevXCoordinate - *circleX)) + (cos(degreeToRadian(angle))*(mYCoordinate - *circleY)) + *circleY;
	}
	if(*circleX == -1.0 || *circleY  == 1)
	{
		//中心座標がinfの場合、計算される座標もおかしな値になるため、適当な値で補足
		//ヒストリクラスから直近の値を参照する形が良いか？
		mXCoordinate = -1.0;
		mYCoordinate = 1.0;
	}
	else{
		if(mXCoordinate <0)
		{
			mXCoordinate = 0;
		}
		if(mXCoordinate >545)
		{
			mXCoordinate = 545;
		}
		if(mYCoordinate >0)
		{
			mYCoordinate = 0;
		}
		if(mYCoordinate < -360)
		{
			mYCoordinate = -360;
		}
	}
}

float Gps::degreeToRadian(float degree)
{
	if(isinf(degree))
	{
		return 1001;
	}
		
	return (degree * M_PI / 180);
}

float Gps::radianToDegree(float radian)
{
	if(isinf(radian))
	{
		return 1000;
	}
	return (radian * 180 / M_PI);
}

float Gps::getXCoordinate()
{
	return mXCoordinate;
}

float Gps::getYCoordinate()
{
	return mYCoordinate;
}

float Gps::getDirection()
{
	return mDirection;
}

float Gps::getDistance()
{
	return mDistance;
}



void Gps::adjustXCoordinate(float offset)
{
	if(isinf(offset))
	{
		if(offset >=0)
		{
			mXOffset = FLT_MAX;
		}
		else
		{
			mXOffset = FLT_MAX*(-1);
		}
	}
	else
	{
		mXOffset = mXCoordinate + offset;
	}
}


void Gps::adjustYCoordinate(float offset)
{
	if(isinf(offset))
	{
		if(offset >=0)
		{
			mYOffset = FLT_MAX;
		}
		else
		{
			mYOffset = FLT_MAX*(-1);
		}
	}
	else
	{
		mYOffset = mYCoordinate + offset;
	}
	
}
	
void Gps::adjustDirection(float offset)
{
	if(isinf(offset))
	{
		if(offset >= 0)
		{
			mDirectionOffset = FLT_MAX;
		}
		else
		{
			mDirectionOffset = FLT_MAX*(-1);
		}
		
	}
	else
	{
		mDirectionOffset = mDirection + offset;
	}
	
}

void Gps::adjustDistance(float offset)
{
	if(isinf(offset))
	{
		if(offset >=0)
		{
			mDistanceOffset = FLT_MAX;
		}
		else
		{
			mDistanceOffset = FLT_MAX*(-1);
		}
		
	}
	else
	{
		mDistanceOffset = mDistance + offset;
	}
}

float Gps::marge360(float margeTarget)
{
	int sign = 1;//符号判別変数 0以上なら1, 0未満なら-1
	float margeT = fabs(margeTarget);
	if(margeTarget >= 0)
	{
		sign = 1;
	}
	else
	{
		sign = -1;
	}
	while(margeT >= 360)
	{
		margeT -= 360;
	}
	
	margeT = (float)sign*margeT;
	
	return margeT;
}

bool Gps::calcCircleCoordinates(float angle, float radius, float *circleX, float *circleY)
{
	/* 走行体の描く円の中心の座標を求める */
	float dirAngle = 0.0;
	
	if(angle < 0)
	{
		dirAngle = marge360(mDirection + 90.0);
	}
	else if( angle >0)
	{
		dirAngle = marge360(mDirection - 90.0);
	}
	else
	{
		return false;
	}
	*circleX = mXCoordinate - radius * cos(degreeToRadian(dirAngle));
	if(isinf(*circleX))
	{
		*circleX = -1.0;
	}
	*circleY = mYCoordinate - radius * sin(degreeToRadian(dirAngle));
	if(isinf(*circleY))
	{
		*circleY = 1.0;
	}
	return true;
}
//■■■■■■以下クラス図にないが実装段階で作成した関数,扱い不明■■■■■■■■■■
void Gps::adjustPosition(float avgX,float avgY,float avgD)
{
	/*直線上を走行中かつGPSの座標から、走行位置、向きを補正*/
	/*GPSVisualizerの座標を利用してマッピングを行う*/
	//座標指定走行完成後、座標補正を行えた後に、switch文の中身追加。
	//現時点では、向き、直線から、どんな場所でも向きを補正するのは困難->区間を座標で指定するため、判定がシビアになる
	/**/
	avgY *= (-1);	//座標符号反転
	int mDirectionDiv = (int)avgD%90;
	//int posFlag = (int)(((avgD + dirThresh))/90);
	posFlag = (int)(((avgD + dirThresh))/90);
	
	if(mDirectionDiv <= dirThresh)
	{
		//
	}
	else if( mDirectionDiv >= 90 - dirThresh)
	{
		mDirectionDiv = 90 - mDirectionDiv;
	}
	else
	{
		return;
	}
	//区間の縦幅+-コース幅、区間の横幅->直線区間距離
	switch(posFlag){
		case 0:
			if( (((avgY >= 340 -courseWidth) && (avgY <= 340 + courseWidth)))  && (((avgX>=118.2) && (avgX <= 419.1 )) && ((mXCoordinate>=118.2) && (mXCoordinate <= 419.1))))
			{
//				beep(200);
				mDirection = 90 * posFlag;
				mYCoordinate = -340.0;
			}
			break;
		case 4:
			if( (((avgY >= 340 -courseWidth) && (avgY <= 340 + courseWidth)))  && (((avgX>=118.2) && (avgX <= 419.1 )) && ((mXCoordinate>=118.2) && (mXCoordinate <= 419.1))))
			{
			//	if(mXCoordinate > 260 && mXCoordinate < 419)beep(200);
				mDirection = 90 * posFlag;
				mYCoordinate = -340.0;
			}
			break;
		
		case 1:
			if( avgX >= 522.6 - courseWidth && avgX <= 522.6 + courseWidth  && avgY <= 270.3 && avgY >= 133.2
			&& mYCoordinate >= -293.0 && mYCoordinate <= -100)
			{
//				beep(210);
				mDirection = 90 * posFlag;
				mXCoordinate = 522.6;
			}
			break;
		case 5:
			if( avgX >= 522.6 -20 - courseWidth && avgX <= 522.6 +40 + courseWidth  
				&& avgY <= 290.0 && avgY >= 133.2 && mYCoordinate >= -293.0 && mYCoordinate <= -100)
			{
//				beep(210);
				mDirection = 90 * posFlag;
				mXCoordinate = 522.6;
			}
			break;
		case 2:
			if( avgY >= 23.1 -courseWidth && avgY <= 23.1 + courseWidth  && avgX>=150.6 && avgX <= 405.6 && mXCoordinate>=150.6 && mXCoordinate <= 405.6)
			{
//				beep(220);
				mDirection = 90 * posFlag;
				mYCoordinate = -23.1;
			}

			break;
		case 6:
			if( avgY >= 23.1 -courseWidth && avgY <= 23.1 + courseWidth  && avgX>=150.6 && avgX <= 405.6 && mXCoordinate>=150.6 && mXCoordinate <= 405.6)
			{
//				beep(220);
				mDirection = 90 * posFlag;
				mYCoordinate = -23.1;
			}

			break;
		case 3:
			if( avgX >= 17.0 -courseWidth && avgX <= 17.0 + courseWidth  && avgY>=131.1 && avgY <= 218.1 && mYCoordinate <= -131.1 && mYCoordinate >= -218.1)
			{
//				beep(230);
				mDirection = 90 * posFlag;
				mXCoordinate = 17.0;
			}
			break;
		case 7:
			if( avgX >= 17.0 -courseWidth && avgX <= 17.0 + courseWidth  && avgY>=131.1 && avgY <= 218.1 && mYCoordinate <= -131.1 && mYCoordinate >= -218.1)
			{
//				beep(230);
				mDirection = 90 * posFlag;
				mXCoordinate = 17.0;
			}
			break;
		/*case 4:
			if( avgY >= 340 -courseWidth && avgY <= 340 + courseWidth  && avgX>=118.2 && avgX <= 419.1 && mXCoordinate>=118.2 && mXCoordinate <= 419.1)
			{
				mDirection = 90 * posFlag;
				mYCoordinate = -340.0;
			}
			break;
		*/
		default:
			break;
	}
	
}

float Gps::getMR()
{
	return motorR.getCount();
}
float Gps::getML()
{
	return motorL.getCount();
}
void Gps::setXCoordinate(float xCo)
{
    mXCoordinate = xCo;
}
void Gps::setYCoordinate(float yCo)
{
	mYCoordinate = yCo;
}

float Gps::getXOffset()
{
	return mXOffset;
}
float Gps::getYOffset()
{
	return mYOffset;
}

float Gps::getDirectionOffset()
{
	return mDirectionOffset;
}
float Gps::getDistanceOffset()
{
	return mDistanceOffset;
}
void Gps::setDirection(float dir)
{
	mDirection = dir;
}
void Gps::setDistance(float distance)
{
	mDistance = distance;
}


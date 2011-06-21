//
// Gps.cpp
//

#include "Gps.h"
//ログ取得用
#include "Bluetooth.h"
#include "Daq.h"
#include "Gps.h"
#include "Lcd.h"
extern Bluetooth bt;
extern Daq mDaq;
//--------------------
//Historyクラス完成後は、各メソッド内で宣言
float prevXCoordinate = 0.0;
float prevYCoordinate = 0.0;
float prevEl = 0.0;
float prevEr = 0.0;

// compile エラー回避
bool gAngleTraceFlag; 

//	補正関係定数
#define DIRECTION_THRESHOLD 25
#define COURSE_WIDTH 500

//outcourse用
#define X_CASE_0_4_START (1500)
#define X_CASE_0_4_END (4500)
#define Y_CASE_0_4 (-3400)
#define Y_CASE_1_5_START (-1132)
#define Y_CASE_1_5_END (-2703)
#define X_CASE_1_5 (5226)
#define X_CASE_2_6_START (1506)
#define X_CASE_2_6_END (4056)
#define Y_CASE_2_6 (-231)
#define Y_CASE_3_7_START (-1311)
#define Y_CASE_3_7_END (-2181)
#define X_CASE_3_7 (170)
//incourse用
#define X_IN_2_6_START (1500)
#define X_IN_2_6_END (-4080)
#define Y_IN_2_6 (-525)
#define X_IN_3 (462)
#define Y_IN_3_START (-1230)
#define Y_IN_3_END (-2082)
#define X_IN_4_START (3010)
#define X_IN_4_END (3690)
#define Y_IN_4 (-3111)

//================== メンバメソッド ===================
/**
 * コンストラクタ
 *
 * @param[in] aMotorL 左モータ
 * @param[in] aMotorR 右モータ
 * @param[in] aCourse コース識別子
 */
Gps::Gps(Motor &aMotorL, Motor &aMotorR, Gps::eCourse aCourse) :
	mWheelRadius(WHEEL_RADIUS),
	mWheelDistance(WHEEL_DISTANCE),
	motorL(aMotorL), 
	motorR(aMotorR),
    mCourse(aCourse),
    mXCoordinate(GPS_COURSE_START_X),
    mYCoordinate(GPS_COURSE_START_Y),
    mDirection(GPS_COURSE_START_DIRECTION),
	mDistance(0.0),
	mXOffset(0.0),
	mYOffset(0.0),
	mDirectionOffset(0.0),
	mDistanceOffset(0.0),
	//以下補正関係変数
    mXAverage(0.0),
    mYAverage(0.0),
    mDirectionAverage(0.0),
    mTimeCounter(0)
{

}

/**
 * Gps情報の更新
 */
void Gps::update()
{
	float angle = 0.0;  /* 4msecごとの車体角度の変化量 (度) */
	float radius = 0.0; /* 4msecごとの車体の描く円の半径 */
	float el = 0.0; /* 4msec間のエンコーダー値の変化量(left) */
	float er = 0.0; /* 4msec間のエンコーダー値の変化量(right) */
	float currEl = 0.0; /* 現在のエンコーダー値(left) */
	float currEr = 0.0; /* 現在のエンコーダー値(right) */
		
 /*********   値取得   *********/
	/* エンコーダー値を取得 */
	currEl = motorL.getCount();
	currEr = motorR.getCount();
	
	/* 4msec間のエンコーダー値の変化量を計算 */
	el = currEl - prevEl;
	er = currEr - prevEr;
/********* 値取得終了 *********/
	
/********* 座標計算 ***********/
	angle = calcAngle(el, er); /* 曲がった角度を計算 */
	
	radius = calcRadius(el, angle); /* 走行体の描く円の半径を計算 */
	
	calcCoordinates(angle, radius, el, er); /* 座標の更新 */
	calcDistance();
	
	calcDirection(angle); /* 現在向いている方向の更新 */


/********* 座標計算終了 *******/

/************ 座標自動補正 *****************/

    mXAverage += getXCoordinate();
    mYAverage += getYCoordinate();
    mDirectionAverage += getDirection();
    /**
     * 座標、向き補正関数呼び出し。呼び出す周期は適当なものに変更するべし。
     */
    if (mTimeCounter % 50 == 0) {
        mXAverage /= 50.0;
        mYAverage /= 50.0;
        mDirectionAverage /= 50.0;
        
        if (mCourse == Gps::OUT && gLineTrace ==true) {
            adjustPositionOut(mXAverage, mYAverage, mDirectionAverage);
        }
    	else if(mCourse == Gps::IN && gLineTrace == true){
            adjustPositionIn(mXAverage, mYAverage, mDirectionAverage);
        }

        mXAverage = 0.0;
        mYAverage = 0.0;
        mDirectionAverage = 0.0;
        mTimeCounter = 0;
    }
	mTimeCounter++;

/************ 座標自動補正終了 *****************/

/******* prevEl,Erの更新 ******/
	prevEl = currEl;
	prevEr = currEr;
/**** prevEl,Erの更新の終了****/

}

/**
 * 走行体の向き(絶対角度、方角)を計算
 *
 * @param[in] angle 単位時間あたりの走行体の曲がった角度
 */
void Gps::calcDirection(float angle)
{
	mDirection += angle;
}

/**
 * 走行体の曲がった角度を計算
 *
 * @param[in] el 単位時間あたりの左モータエンコーダ値変化量
 * @param[in] er 単位時間あたりの右モータエンコーダ値変化量
 */
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

/**
 * 走行体の描く円の半径を計算
 *
 * @param[in] encoderLeft 単位時間あたりの左モータエンコーダ値変化量
 * @param[in] angle 単位時間あたりの走行体の曲がった角度
 */
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

/**
 * 走行体の移動した距離を計算
 */
void Gps::calcDistance()
{
    //// エンコーダ値ベース
	float el = motorL.getCount();
    float er = motorR.getCount();
    float e  = (el + er)/2.0; // 平均
    mDistance = (e / 360.0) * 2.0 * mWheelRadius * M_PI;
    //// 座標ベース(バックしても加算されてしまう。。。)
	// float distance;
	// float x = mXCoordinate - prevXCoordinate;
	// float y = mYCoordinate - prevYCoordinate;
	// distance = sqrt(x*x + y*y);
	// mDistance += distance;
}

/**
 * x座標、y座標を計算
 *
 * @param[in] angle 単位時間あたりの走行体の曲がった角度
 * @param[in] radius 車輪半径
 * @param[in] encoderL 単位時間あたりの左モータエンコーダ値変化量
 * @param[in] encoderR 単位時間あたりの右モータエンコーダ値変化量
 */
void Gps::calcCoordinates(float angle, float radius, float encoderL, float encoderR)
{	
	float circleX = 0.0, circleY = 0.0;
	prevXCoordinate = mXCoordinate;
	prevYCoordinate = mYCoordinate;
    
    if(encoderL*encoderR >= 0)
    {  
        /* 走行体の描く円の中心の座標を求める */
        if(angle < 0)
        {
            circleX = mXCoordinate - radius * cos(degreeToRadian(marge360(mDirection) + 90.0));
            circleY = mYCoordinate - radius * sin(degreeToRadian(marge360(mDirection) + 90.0));
        }
        else
        {
            circleX = mXCoordinate - radius * cos(degreeToRadian(marge360(mDirection) - 90.0));
            circleY = mYCoordinate - radius * sin(degreeToRadian(marge360(mDirection) - 90.0));
        }
        
        /* 座標の計算 */
        if(angle == 0)/* 直進の場合 */
        {
            mXCoordinate = mXCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * M_PI * (cos(degreeToRadian(((int)mDirection)%360)));
            mYCoordinate = mYCoordinate + (encoderL / 360.0) * 2.0 * mWheelRadius * M_PI * (sin(degreeToRadian(((int)mDirection)%360)));
        }
        else/* 円を描く場合 */ 
        {
            mXCoordinate = (cos(degreeToRadian(angle))*(prevXCoordinate - circleX)) - (sin(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleX;
            mYCoordinate = (sin(degreeToRadian(angle))*(prevXCoordinate - circleX)) + (cos(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleY;
        }	
    }else
    {
        float rightWheelXCoordinate = 0.0;
        float rightWheelYCoordinate = 0.0;
        float leftWheelXCoordinate = 0.0;
        float leftWheelYCoordinate = 0.0;
        /* 右車輪位置座標を求める */
        rightWheelXCoordinate = mXCoordinate + (mWheelDistance/2.0) * cos(degreeToRadian(marge360(getDirection() - 90.0)));
        rightWheelYCoordinate = mYCoordinate + (mWheelDistance/2.0) * sin(degreeToRadian(marge360(getDirection() - 90.0)));
       	
        /* 左車輪位置座標を求める */
        leftWheelXCoordinate = mXCoordinate - (mWheelDistance/2.0)*cos(degreeToRadian(marge360(getDirection() - 90.0)));
        leftWheelYCoordinate = mYCoordinate - (mWheelDistance/2.0)*sin(degreeToRadian(marge360(getDirection() - 90.0)));
        /* 回転中心座標を求める。 */
        circleX = (float)(leftWheelXCoordinate + (fabs(encoderL)/(fabs(encoderL)+fabs(encoderR)))*(rightWheelXCoordinate - leftWheelXCoordinate));
        circleY = (float)(leftWheelYCoordinate + (fabs(encoderL)/(fabs(encoderL)+fabs(encoderR)))*(rightWheelYCoordinate - leftWheelYCoordinate));
        /* 移動後のロボットの位置座標を求める。 */
        mXCoordinate = (cos(degreeToRadian(angle))*(prevXCoordinate - circleX)) - (sin(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleX;
    	mYCoordinate = (sin(degreeToRadian(angle))*(prevXCoordinate - circleX)) + (cos(degreeToRadian(angle))*(mYCoordinate - circleY)) + circleY;
    }
	if(circleX == -1.0 || circleY  == 1)
	{
		/*中心座標がinfの場合、計算される座標もおかしな値になるため、適当な値で補足 */
		/*ヒストリクラスから直近の値を参照する形が良いか？ */
		mXCoordinate = -1.0;
		mYCoordinate = 1.0;
	}
	else{
		// if(mXCoordinate < 0)
		// {
		// 	mXCoordinate = 0;
		// }
		// if(mXCoordinate > X_RIGHT)
		// {
		// 	mXCoordinate = X_RIGHT;
		// }
		// if(mYCoordinate > 0)
		// {
		// 	mYCoordinate = 0;
		// }
		// if(mYCoordinate < Y_DOWN)
		// {
		// 	mYCoordinate = Y_DOWN;
		// }
	}
}

/**
 * X座標を取得する
 *
 * @return X座標
 */
float Gps::getXCoordinate()
{

	return mXCoordinate + mXOffset;

}

/**
 * Y座標を取得する
 *
 * @return Y座標
 */
float Gps::getYCoordinate()
{
	return mYCoordinate + mYOffset;
}

/**
 * 車体向きを取得する
 *
 * @return 車体向き
 */
float Gps::getDirection()
{
	return mDirection + mDirectionOffset;
}

/**
 * 距離を取得する
 *
 * @return 距離
 */
float Gps::getDistance()
{
	return mDistance + mDistanceOffset;
}


/**
 * X座標を補正する
 *
 * @param[in] trueValue 真値
 */
void Gps::adjustXCoordinate(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mXCoordinate = trueValue;
    // @todo: 各種計算式をオフセット対応に修正
    //mXOffset = trueValue - mXCoordinate;
}


/**
 * Y座標を補正する
 *
 * @param[in] trueValue 真値
 */
void Gps::adjustYCoordinate(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mYCoordinate = trueValue;
    // @todo: 各種計算式をオフセット対応に修正
    //mYOffset = trueValue - mYCoordinate;
}
	
/**
 * 向きを補正する
 *
 * @param[in] trueValue 真値
 */
void Gps::adjustDirection(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mDirection = trueValue;
    // @todo: 各種計算式をオフセット対応に修正
    //mDirectionOffset = trueValue - mDirection;
}

/**
 * 距離を補正する
 *
 * @param[in] trueValue 真値
 */
void Gps::adjustDistance(float trueValue)
{
	if (isinf(trueValue)) {
		if(trueValue >= 0) {
			trueValue = FLT_MAX;
		}
		else {
			trueValue = FLT_MAX*(-1);
		}
	}
    mDistance = trueValue;
    // @todo: 各種計算式をオフセット対応に修正
    //mDistanceOffset = trueValue - mDistance;
}

/**
 * 走行体の描く円の中心の座標を求める
 *
 * @param[in] angle 単位時間あたりの走行体の曲がった角度
 * @param[in] radius 車輪半径
 * @param[out] circleX 中心のX座標
 * @param[out] circleY 中心のX座標
 * @retval false 非対応の動き
 */
bool Gps::calcCenterCoordinates(float angle, float radius, float *circleX, float *circleY)
{
	/* 走行体の描く円の中心の座標を求める */
	float dirAngle = 0.0;
	
	if(angle < 0)
	{
		dirAngle = marge360(getDirection() + 90.0);
	}
	else if( angle >0)
	{
		dirAngle = marge360(getDirection() - 90.0);
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

/**
 * アウトコース自動補正関数
 *
 * @param[in] avgX x座標一定時間平均値
 * @param[in] avgY y座標一定時間平均値
 * @param[in] avgD 向き一転時間平均値
 * @author tatsuno
 */
void Gps::adjustPositionOut(float avgX,float avgY,float avgD)
{
	/**
     * 直線上を走行中かつGPSの座標から、走行位置、向きを補正
	 * GPSVisualizerの座標を利用してマッピングを行う
	 * 座標指定走行完成後、座標補正を行えた後に、switch文の中身追加。
	 * 現時点では、向き、直線から、どんな場所でも向きを補正するのは困難->区間を座標で指定するため、判定がシビアになる
	 */
    int direction = (int)marge360(avgD);
	int mDirectionDiv = direction%90;
	int posFlag = (int)(((direction + DIRECTION_THRESHOLD))/90);
	
	if(mDirectionDiv <= DIRECTION_THRESHOLD)
	{
		//
	}
	else if( mDirectionDiv >= 90 - DIRECTION_THRESHOLD)
	{
		mDirectionDiv = 90 - mDirectionDiv;
	}
	else
	{
		return;
	}
	/* 区間の縦幅+-コース幅、区間の横幅->直線区間距離 */
	switch(posFlag){
		case 0:
			if( (((avgY >= Y_CASE_0_4 -COURSE_WIDTH) && (avgY <= Y_CASE_0_4 + COURSE_WIDTH)))  && (((avgX>=X_CASE_0_4_START) && (avgX <= X_CASE_0_4_END )) && ((getXCoordinate()>=X_CASE_0_4_START) && (getXCoordinate() <= X_CASE_0_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_CASE_0_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		case 1:
			if( avgX >= X_CASE_1_5 - COURSE_WIDTH && avgX <= X_CASE_1_5 + COURSE_WIDTH  && avgY <= Y_CASE_1_5_START && avgY >= Y_CASE_1_5_END
			&& getYCoordinate() >= Y_CASE_1_5_END && getYCoordinate() <= Y_CASE_1_5_START)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(getYCoordinate());
			    adjustXCoordinate(X_CASE_1_5);
			}
			break;
		case 2:
			if( avgY >= Y_CASE_2_6 -COURSE_WIDTH && avgY <= Y_CASE_2_6 + COURSE_WIDTH  && avgX>=X_CASE_2_6_START && avgX <= X_CASE_2_6_END && getXCoordinate()>=X_CASE_2_6_START && getXCoordinate() <= X_CASE_2_6_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_CASE_2_6);
			    adjustXCoordinate(getXCoordinate());
			}

			break;
		case 3:
			if( avgX >= X_CASE_3_7 -COURSE_WIDTH && avgX <= X_CASE_3_7 + COURSE_WIDTH  && avgY>=Y_CASE_3_7_END && avgY <= Y_CASE_3_7_START && getYCoordinate() <= Y_CASE_3_7_START && getYCoordinate() >= Y_CASE_3_7_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(getYCoordinate());
			    adjustXCoordinate(X_CASE_3_7);
			}
			break;
		case 4:
			if( (((avgY >= Y_CASE_0_4 -COURSE_WIDTH) && (avgY <= Y_CASE_0_4 + COURSE_WIDTH)))  && (((avgX>=X_CASE_0_4_START) && (avgX <= X_CASE_0_4_END )) && ((getXCoordinate()>=X_CASE_0_4_START) && (getXCoordinate() <= X_CASE_0_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_CASE_0_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		default:
			break;
	}
}

/**
 * インコース自動補正関数
 *
 * @param[in] avgX x座標一定時間平均値
 * @param[in] avgY y座標一定時間平均値
 * @param[in] avgD 向き一転時間平均値
 * @author tatsuno
 */
void Gps::adjustPositionIn(float avgX, float avgY, float avgD)
{
	/**
     * 直線上を走行中かつGPSの座標から、走行位置、向きを補正
	 * GPSVisualizerの座標を利用してマッピングを行う
	 * 座標指定走行完成後、座標補正を行えた後に、switch文の中身追加。
	 * 現時点では、向き、直線から、どんな場所でも向きを補正するのは困難->区間を座標で指定するため、判定がシビアになる
	 */
    int direction = (int)marge360(avgD);
	int mDirectionDiv = direction%90;
	int posFlag = (int)(((direction + DIRECTION_THRESHOLD))/90);
	
	if(mDirectionDiv <= DIRECTION_THRESHOLD)
	{
		//
	}
	else if( mDirectionDiv >= 90 - DIRECTION_THRESHOLD)
	{
		mDirectionDiv = 90 - mDirectionDiv;
	}
	else
	{
		return;
	}
	/* 区間の縦幅+-コース幅、区間の横幅->直線区間距離 */
	switch(posFlag){
		case 0:
			if( (((avgY >= Y_IN_4 -COURSE_WIDTH) && (avgY <= Y_IN_4 + COURSE_WIDTH)))  && (((avgX>= X_IN_4_START ) && (avgX <= X_IN_4_END )) && ((getXCoordinate()>=X_IN_4_START) && (getXCoordinate() <= X_IN_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_IN_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		case 1:
			break;
		case 2:
			if( avgY >= Y_IN_2_6 -COURSE_WIDTH && avgY <= Y_IN_2_6 + COURSE_WIDTH  && avgX>=X_IN_2_6_START && avgX <= X_IN_2_6_END && getXCoordinate()>=X_IN_2_6_START && getXCoordinate() <= X_IN_2_6_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_IN_2_6);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		case 3:
			if( avgX >= X_IN_3 -COURSE_WIDTH && avgX <= X_IN_3 + COURSE_WIDTH  && avgY>=Y_IN_3_END && avgY <= Y_IN_3_START && getYCoordinate() <= Y_IN_3_START && getYCoordinate() >= Y_IN_3_END)
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(getYCoordinate());
			    adjustXCoordinate(X_IN_3);
			}
			break;
   		case 4:
			if( (((avgY >= Y_IN_4 -COURSE_WIDTH) && (avgY <= Y_IN_4 + COURSE_WIDTH)))  && (((avgX>= X_IN_4_START ) && (avgX <= X_IN_4_END )) && ((getXCoordinate()>=X_IN_4_START) && (getXCoordinate() <= X_IN_4_END))))
			{
			    adjustDirection(90 * posFlag);
			    adjustYCoordinate(Y_IN_4);
			    adjustXCoordinate(getXCoordinate());
			}
			break;
		default:
			break;
	}
}

//================== クラスメソッド ===================
/**
 * ユーティリティ関数:角度を [0, 360] または [0, -360]に抑える
 *
 * @param[in] margeTarget 角度
 * @return [0, 360] または [0, -360] の対応する角度
 */
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

/**
 * ユーティリティ関数:角度を [-180, 180] に抑える
 *
 * @param[in] margeTarget 角度
 * @return [-180, 180] の対応する角度
 */
float Gps::marge180(float margeTarget)
{
	float margeResult = margeTarget;
	while(margeResult >= 180)
    {
		margeResult -= 360;
	}
	while(margeResult < -180)
    {
		margeResult += 360;
	}
	return margeResult;
}

/**
 * ユーティリティ関数:角度(degree) をラジアン(radian)に変換
 *
 * @param[in] degree 角度
 * @return ラジアン
 */
float Gps::degreeToRadian(float degree)
{
	if(isinf(degree))
	{
		return 1001;
	}
		
	return (degree * M_PI / 180);
}

/**
 * ユーティリティ関数:ラジアン(radian)を角度(degree)に変換
 *
 * @param[in] radian ラジアン
 * @return 角度
 */
float Gps::radianToDegree(float radian)
{
	if(isinf(radian))
	{
		return 1000;
	}
	return (radian * 180 / M_PI);
}

/**
 * ユーティリティ関数:atan を使った自作 atan2 関数
 *
 * atan2 を使用すると固まることがあるため
 *
 * @param[in] y
 * @param[in] x
 * @return ラジアン
 */
double Gps::atan2(double y, double x)
{
    double radian;
    if (x == 0.0) {
        if (y > 0) radian = M_PI/2.0;
        else radian = -M_PI/2.0;
    }
    else {
        radian = atan(y / x);
        if (x < 0 && y < 0) radian = (-M_PI/2.0) - radian;
        if (x < 0 && y >= 0) radian = M_PI + radian;
    }
    return radian;
}

/**
 * 現在座標と指定座標間の距離を計算する
 *
 * @param[in] targetCoordinate 指定座標
 *
 * @return 距離
 */
float Gps::calcDistanceTo(Point targetCoordinate)
{
    Point current =  MakePoint(getXCoordinate(), getYCoordinate());
    double dX = current.X - targetCoordinate.X;
    double dY = current.Y - targetCoordinate.Y;
    return sqrt(dX*dX + dY*dY);
}


//============== Obsolete =============
/**
 * x座標セッタ
 *
 * @param[in] xCo 指定x座標
 *
 */
void Gps::setXCoordinate(float xCo)
{
    mXCoordinate = xCo;
}
/**
 * y座標セッタ
 *
 * @param[in] yCo 指定y座標
 *
 */
void Gps::setYCoordinate(float yCo)
{
	mYCoordinate = yCo;
}
/**
 * directionセッタ
 *
 * @param[in] direction 指定向き
 *
 */
void Gps::setDirection(float direction)
{
    mDirection = direction;
}
/**
 * 距離セッタ
 *
 * @param[in] distance 指定距離
 *
 */
void Gps::setDistance(float distance)
{
    mDistance = distance;
}


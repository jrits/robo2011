//
// Gps.h
//
#ifndef GPS_H_
#define GPS_H_

#include "Motor.h"
//#include "constants.h"
#include <cmath>
#include <float.h>
#include <math.h>

//#define RADIUS 3.85
//#define DISTANCE 16.8
//以下勝ちロボの数値です
//#define RADIUS 4.05
//#define DISTANCE 16.2
#define PI 3.14159265358979

//	補正関係定数
#define dirThresh 10
#define courseWidth 30
#define CORRECTIONINTERVAL 250

class Gps
{
private:
	Motor &motorL;
	Motor &motorR;

	
	float mXCoordinate;
	float mYCoordinate;
	float mDirection;
	float mDistance;
	float mWheelRadius;
	float mXOffset;
	float mWheelDistance;
	float mYOffset;
	float mDirectionOffset;
	float mDistanceOffset;
	
	//以下補正関係変数,扱い不明(クラス図にはない変数)
	int timeCounterHelper;
	int timeCounter;
	float xHistory;
	float yHistory;
	float mDirectionHistory;
	int posFlag;

public:
	Gps(Motor &aMotorL, Motor &aMotorR);
	~Gps(){}
	void update();
	float getXCoordinate();
	float getYCoordinate();
	float getDirection();
	float getDistance();
	void adjustXCoordinate(float offset);
	void adjustYCoordinate(float offset);
	void adjustDirection(float offset);
	void adjustDistance(float offset);
	float marge360(float margeTarget);
	float calcAngle(float el, float er);
	float calcRadius(float encoderLeft, float angle);
	void calcDirection(float angle);
	void calcDistance();
	float radianToDegree(float radian);
	float degreeToRadian(float degree);
	bool calcCircleCoordinates(float angle, float radius, float *circleX, float *circleY);//追加,未実装
	void calcCoordinates(float angle, float distance, float encoderL);
	
	
	
	
	//以下補正関係関数,扱い不明(クラス図にはない変数)
	float getMR();
	float getML();
	void adjustPosition(float avgX, float avgY, float avgD);
	void setYCoordinate(float yCo);
	void setXCoordinate(float xCo);
	float getXOffset();
	float getYOffset();
	float getDirectionOffset();
	float getDistanceOffset();
	void setDirection(float dir);
	void setDistance(float distance);
};

#endif

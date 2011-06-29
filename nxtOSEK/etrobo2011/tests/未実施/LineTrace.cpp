///////////////////////////////////////////////////////////
//  LineTrace.cpp
//  Implementation of the Class LineTrace
//  Created on:      10-9-2010 19:43:08
//  Original author: skawasak
///////////////////////////////////////////////////////////

#include "LineTrace.h"
//extern INIT_FORWARD;
//extern FORWARD;
//extern LEFT (-1)
//extern RIGHT (1)
//extern TRACE_EDGE (1)
#define INIT_FORWARD (50.0)
#define FORWARD (100.0)
#define LEFT (-1)
#define RIGHT (1)
//#define TRACE_EDGE (1)


int TRACE_EDGE[3] = {LEFT, RIGHT, 0};														//テスト用

LineTrace::LineTrace(float black, float white, float lightThresh, LightSensor &light):
	mBlack(black), mWhite(white),mThreshold(lightThresh),
	mLightSensor(light)
{
	/*this -> mBlack = black;
	this -> mWhite = white;
	this -> mThreshold = lightThresh;
	this -> mLightSensor = light;
	*/
}

LineTrace::~LineTrace(){

}
	
float LineTrace::calcVector(){

int i;																						//テスト用

	//正規化した光センサ値をPに格納する
	float P = this->normalizeLightValue();
	
	//光センサ値をPID制御にかける
//	float Y = mPid.control(P);
	float Y = 5;																			//テスト用

for(i=0; i<3; i++){																			//テスト用
	
	
	if (TRACE_EDGE[i] == LEFT) {
		Y *= -1;
    	assert(TRACE_EDGE[i] == LEFT);														//テスト用
    	cout << "Test No.1 OK" << endl;														//テスト用
    }
	else if(TRACE_EDGE[i] == RIGHT) {
		Y *= 1;
		assert(TRACE_EDGE[i] == RIGHT);														//テスト用
    	cout << "Test No.2 OK" << endl;														//テスト用
	}
	else {
		/* 例外 */
		Y *= 1;
		assert(TRACE_EDGE[i] != LEFT && TRACE_EDGE[i] != RIGHT);							//テスト用
    	cout << "Test No.3 OK" << endl;														//テスト用
	}

}																							//テスト用
	
	return Y;
}


float LineTrace::normalizeLightValue(){
//	float L = 0.0;
//	L = mLightSensor.get();

//	L = mLightSensor.getBrightness();														//テスト用
	int i;																					//テスト用
	float L[12] = {800.0, 800.0, 800.0, 500.0, 700.0, 400.0, 800.0, 800.0, 800.0, NULL , NAN, INFINITY};//テスト用

	float P = 0.0; // 偏差

	for(i=0; i<12; i++){																	//テスト用
	
	//L値チェック
	if(L[i] == NULL)
	{
		assert(L[i] == NULL);																//テスト用
    	cout << "Test No.10 OK" << endl;													//テスト用
		P = 0.0;
	}
	else if(isnan(L[i]))
	{
		assert(isnan(L[i]));																//テスト用
    	cout << "Test No.11 OK" << endl;													//テスト用
		P = 0.0;
	}
	else if(isinf(L[i]))
	{
		assert(isinf(L[i]));																//テスト用
    	cout << "Test No.12 OK" << endl;													//テスト用
		P = 0.0;
	}
	else
	{
		P = L[i] - mThreshold;
	}
    
    if(L[i] < mThreshold){ // 白
    	assert(L[i] < mThreshold);															//テスト用
    	cout << "Test No.4 OK" << endl;														//テスト用
    	P = P / (mThreshold - mWhite); // [-1.0, 1.0] の値に正規化
    	
    }
	else if(L[i] >= mThreshold){ // 黒
    	assert(L[i] >= mThreshold);															//テスト用
    	cout << "Test No.5 OK" << endl;														//テスト用
    	P = P / (mBlack - mThreshold); // [-1.0, 1.0] の値に正規化
    	
    }
	else{}
		
	int j;																					//テスト用
	float tP[3] = {P, NAN, INFINITY};				//テスト用
	for(j=0; j<3; j++){																	//テスト用
	
	if(isinf(tP[j]))
	{
		assert(isinf(tP[j]));																	//テスト用
    	cout << "Test No.9 OK" << endl;														//テスト用
		P = 0.0;
	}
	else if(isnan(tP[j]))
	{
		assert(isnan(tP[j]));																	//テスト用
    	cout << "Test No.8 OK" << endl;														//テスト用
		P = 0.0;
	}
	else if(tP[j] < -1)
	{
		assert(tP[j] < -1);																		//テスト用
    	cout << "Test No.6 OK" << endl;														//テスト用
		P = -1;
	}
	else if(tP[j] > 1)
	{
		assert(tP[j] > 1);																		//テスト用
    	cout << "Test No.7 OK" << endl;														//テスト用
		P = 1;
	}
}																							//テスト用
		
}																							//テスト用
		
    return P;
}


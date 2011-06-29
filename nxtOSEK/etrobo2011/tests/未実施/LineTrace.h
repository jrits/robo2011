///////////////////////////////////////////////////////////
//  LineTrace.h
//  Implementation of the Class LineTrace
//  Created on:      10-9-2010 19:43:08
//  Original author: skawasak
///////////////////////////////////////////////////////////

#if !defined(EA_F7286BF8_B846_4e5c_82B2_A69DC6C89781__INCLUDED_)
#define EA_F7286BF8_B846_4e5c_82B2_A69DC6C89781__INCLUDED_

#include "LightSensor.h"
#include <iostream>
#include <cassert>
using namespace std;
//#include "PID.h"
//#include "Skill.h"


class LineTrace //: public Skill
{
private:
	float mBlack;
	float mWhite;
	float mThreshold;
	LightSensor &mLightSensor;

//	VectorT<float> calcVector();
	float calcVector();
	float normalizeLightValue();
	
public:
	LineTrace(float black, float white, float lineThresh, LightSensor &light);
	virtual ~LineTrace();
	//skillクラスがないためヘッダ内にてdrive関数を宣言,実際はオーバーロードするはず
	void drive() {
        float Y; // 旋回量
        Y = calcVector();
        
        //VectorT<F32> command;
        //command.mX = (F32)mForward; // 前後進命令セット
        //command.mY = (F32)Y;        // 旋回命令セット
        
        //mActivator.run(command);//制御機器にセット
    }
//	PID &mPID;

};
#endif // !defined(EA_F7286BF8_B846_4e5c_82B2_A69DC6C89781__INCLUDED_)

//
// LightSensor.cpp
//
// Copyright 2009 by Takashi Chikamasa, Jon C. Martin and Robert W. Kramer
//

#include "LightSensor.h"
//using namespace ecrobot;

//=============================================================================
// Constructor
LightSensor::LightSensor(char* fileName)
{
	/* ファイル読み込み */
	ifstream infile( fileName );

	// 初期化 
	for(int i = 0; i < TSIZE; i++){
		mTable[i] = 0;
	} 
	
	/* テーブルにファイルを格納 */
	for(int i = 0; !infile.eof(); i++){
		string inString;
		//infile >> mTable[i];
		infile >> inString;
		inString.erase(inString.size(),inString.length());
		for(unsigned int j = 0; j < inString.size(); j++) {
			mTable[i] += (char(inString[j]) - '0') * (int)pow(10.0f, (float)(inString.size() - 1 - j));
    	}
	}
	index = -1;
}

//=============================================================================
// Destructor
LightSensor::~LightSensor(void)
{
	//unset_digi0(Sensor::getPort()); // turn off lamp
}

//=============================================================================
// get brightness (greater value means brighter)
S16 LightSensor::getBrightness(void) const
{
	//ここいじる
	return mTable[index];
	//return (1023 - Sensor::get());
}

bool LightSensor::setBrightness(void)
{
	index++;
	if(index < TSIZE)
	{
		return true;
	}
	else{
		return false;
	}
}

//=============================================================================
// turn on/off lamp
void LightSensor::setLamp(bool lamp)
{ 
	/*if (lamp == true)
	{
		set_digi0(Sensor::getPort());
	}
	else
	{
		unset_digi0(Sensor::getPort());
	}
	*/
}

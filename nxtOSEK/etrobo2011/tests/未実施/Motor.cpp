//
// Motor.cpp
//
// Tsubasa
//

#include "Motor.h"

//=============================================================================
// Constructor
Motor::Motor(char* filename){

	/* ファイル読み込み */
	ifstream infile( filename );

	/* 初期化  */
	index = -1;
	for(int i = 0; i < sizeof(mTable) / sizeof(int); i++){
		mTable[i] = 0;
	}
	
	/* テーブルにファイルを格納 */
	for(int i = 0; !infile.eof(); i++){
		infile >> mTable[i];
	}
}
//=============================================================================

void Motor::setCount(){
	index ++;
}

S32 Motor::getCount(){
	return mTable[index];
}

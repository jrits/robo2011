#include <iostream>
#include "LineTrace.h"
#include <cmath>
#include <cassert>
#include <float.h>

int makeArray(float array[]);
int main(){
	char fileName[] = "test.csv";
	float black = 700.0;
	float white = 500.0;
	float lineThresh = 600.0;
	float X = 0.0, Y = 0.0;
	int count = 0;
	LightSensor mLight(fileName);
	LineTrace mLine(black, white, lineThresh, mLight);
	while(mLight.setBrightness())
	{
		if(mLight.getBrightness() == 1111)break;
		mLine.drive();
	}
	
	return 0;
}

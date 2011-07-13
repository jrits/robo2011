///////////////////////////////////////////////////////////
//  Histrory_Test.cpp
//  testcode of the Class Histrory
//  Created on:      29-9-2010 9:49:00
//  Original author: yhayama
///////////////////////////////////////////////////////////

//#include <iostream>
#include "History.h"
#include "Motor.h"
#include "factory.h"

void jadge(int count, float num, float dValue){

int main()//テストコード(全て網羅できていない)
{
	float num = 0;
	int count = 0;
	
	char fileName[] = "test.dat";
	Motor motor(fileName);
	
	History histSensor(motor,data_3,3);
	
	while(count < 9){
	    motor.setCount();
		switch (count)
		{
		case 0:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,8.0);
			break;
		case 1:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,3.0);
			break;
		case 2:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,1.0);
			break;
		case 3:
//			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,1.0);
			num = histSensor.get(2);
			jadge(count,num,8.0);
			num = histSensor.get(3);
			jadge(count,num,1.0);
			num = histSensor.get(-1);
			jadge(count,num,8.0);
			break;
		case 4:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,80);
			break;
		case 5:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,30);
			break;
		case 6:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,10);
			break;
		case 7:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			jadge(count,num,100);
			break;
		case 8:
			histSensor.updateBuffer();
			num = histSensor.get(0);
			cout<<"get(0)="<<num<<endl;
			num = histSensor.get(1);
			cout<<"get(1)="<<num<<endl;
			num = histSensor.get(2);
			cout<<"get(2)="<<num<<endl;
			num = histSensor.get(3);
			cout<<"get(3)="<<num<<endl;
			num = histSensor.get(-1);
			cout<<"get(-1)="<<num<<endl;
			num = histSensor.get(-2);
			cout<<"get(-2)="<<num<<endl;
			num = histSensor.get(-10);
			cout<<"get(-10)="<<num<<endl;
			num = histSensor.get(3);
			cout<<"get(-10)="<<num<<endl;
			jadge(count,num,-1);
			break;
		default:
			break;
		}
        count++;
	}

	
	
	
	return 0;
}
	
//正しい値かどうかの判定を行う
void jadge(int count, float num, float dValue){
	if(num == dValue){
		cout<<"case"<<count<<" OK = "<<num<<endl;
	}
	else{
		cout<<"case"<<count<<" NG = "<<num<<endl;
	}
}


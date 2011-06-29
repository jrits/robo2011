#include "GpsTest.h"
//using namespace std;
int totalNum = 50;
int successCount = 0;
#define ADJUST_CASE (20)
#define GET_DIRECTION_VALUE (180)
#define GET_XCOORDINATE_VALUE (100)
#define GET_YCOORDINATE_VALUE (-100)
#define CALCANGLE_VALUE 0
#define CALCANGLE_ER_CASE 0
#define CALCANGLE_EL_CASE 0
#define CALCRADIUS_VALUE (12.15)
#define CALCRADIUS_NAN 0
#define CALCRADIUS_INF 0
#define CALC_DIRECTION_VALUE 540
#define CALC_DISTANCE_VALUE (0)
#define MARGET_PLUS (280)
#define MARGET_MINUS (-280)
#define MARGE_INPUT_ZERO (0)
#define RAD_TO_DEG_PI_VALUE (180)
#define RAD_TO_DEG_ERROR (1000)
#define DEG_TO_RAD_CORRECT (PI)
#define DEG_TO_RAD_ERROR (1001)
//UPDATEはx,yセットでテストNo39をチェック
#define UPDATE_CHECK_X (100)
#define UPDATE_CHECK_Y (-100)
//CORRECTはテストNO41,42で利用
#define CALCCIRCLE_CORRECT (true)
#define CALC_CIRCLE_ERROR (false)
//以後、x,yセットで一項目

#define CALC_COORDINATES_CASE1_X (50.0)
#define CALC_COORDINATES_CASE1_Y (-150.0)
#define CALC_COORDINATES_CASE2_X (50.0)
#define CALC_COORDINATES_CASE2_Y (-50.0)
#define CALC_COORDINATES_CASE3_X (49.1062)
#define CALC_COORDINATES_CASE3_Y (-100.0)
#define CALC_COORDINATES_CASE4_X (0.0)
#define CALC_COORDINATES_CASE4_Y (-250.0)
#define CALC_COORDINATES_CASE5_X (0.0)
#define CALC_COORDINATES_CASE5_Y (0.0)
#define CALC_COORDINATES_CASE6_X (0.0)
#define CALC_COORDINATES_CASE6_Y (-100.0)
#define CALC_COORDINATES_CASE7_X (-1.0)
#define CALC_COORDINATES_CASE7_Y (1.0)


float adjustCase[ADJUST_CASE]=
	{0,30,-30,(FLT_MAX)*10,(-1)*(FLT_MAX)*10,
		0,30,-30,(FLT_MAX)*10,(-1)*(FLT_MAX)*10,
		0,30,-30,(FLT_MAX)*10,(-1)*(FLT_MAX)*10,
		0,30,-30,(FLT_MAX)*10,(-1)*(FLT_MAX)*10,
	};





void judge(int testNumber, bool result);
int main(){
	char file1[] = "testL.dat";
	char file2[] = "testR.dat";
	Motor aMotorL(file1);
	Motor aMotorR(file2);

	Gps mGps(aMotorL,aMotorR);
	mGps.update();
	float cirX = 0;
	float cirY = 0;
	float* cirXCo = &cirX;
	float* cirYCo = &cirY;
	bool buf = true;
	
	float xCo;
	float yCo;
	int counter = 0;
	for(int i = 0; i < totalNum ; i++)
	{
		switch(i)
		{
		case 0:
			mGps.adjustDirection(adjustCase[i]);
			if(mGps.getDirectionOffset() == mGps.getDirection() + 0)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1,false);
			}
			break;
		case 1:
			mGps.adjustDirection(adjustCase[i]);
			if(mGps.getDirectionOffset() == mGps.getDirection() + 30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1,false);
			}
			break;
		case 2:
			mGps.adjustDirection(adjustCase[i]);
			if(mGps.getDirectionOffset() == mGps.getDirection() -30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1,false);
			}
			break;
		case 3:
			mGps.adjustDirection(adjustCase[i]);
			if(mGps.getDirectionOffset() == FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1,false);
			}
			break;
		case 4:
			mGps.adjustDirection(adjustCase[i]);
			if(mGps.getDirectionOffset() == (-1)*FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1,false);
			}
			break;
			//XCO
		case 5:
			mGps.adjustXCoordinate(adjustCase[i]);
			if(mGps.getXOffset() == mGps.getXCoordinate() + 0)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1,false);
			}
			break;
		case 6:
			mGps.adjustXCoordinate(adjustCase[i]);
			if(mGps.getXOffset() == mGps.getXCoordinate() + 30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 7:
			mGps.adjustXCoordinate(adjustCase[i]);
			if(mGps.getXOffset() == mGps.getXCoordinate() - 30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 8:
			mGps.adjustXCoordinate(adjustCase[i]);
			if(mGps.getXOffset() == FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 9:
			mGps.adjustXCoordinate(adjustCase[i]);
			if(mGps.getXOffset() == (-1)*FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
			//YCO
		case 10:
			mGps.adjustYCoordinate(adjustCase[i]);
			if(mGps.getYOffset() == mGps.getYCoordinate() + 0)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 11:
			mGps.adjustYCoordinate(adjustCase[i]);
			if(mGps.getYOffset() == mGps.getYCoordinate() + 30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 12:
			mGps.adjustYCoordinate(adjustCase[i]);
			if(mGps.getYOffset() == mGps.getYCoordinate() - 30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 13:
			mGps.adjustYCoordinate(adjustCase[i]);
			if(mGps.getYOffset() == FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 14:
			mGps.adjustYCoordinate(adjustCase[i]);
			if(mGps.getYOffset() == (-1)*FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
			//DISTANCE
		case 15:
			mGps.adjustDistance(adjustCase[i]);
			if(mGps.getDistanceOffset() == mGps.getDistance() + 0)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 16:
			mGps.adjustDistance(adjustCase[i]);
			if(mGps.getDistanceOffset() == mGps.getDistance() + 30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 17:
			mGps.adjustDistance(adjustCase[i]);
			if(mGps.getDistanceOffset() == mGps.getDistance() - 30)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 18:
			mGps.adjustDistance(adjustCase[i]);
			if(mGps.getDistanceOffset() == FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 19:
			mGps.adjustDistance(adjustCase[i]);
			if(mGps.getDistanceOffset() == (-1)*FLT_MAX)
			{
				judge(i + 1,true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 20:
			if(mGps.getDirection() == GET_DIRECTION_VALUE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 21:
			if(mGps.getXCoordinate() == GET_XCOORDINATE_VALUE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 22:
			if(mGps.getYCoordinate() == GET_YCOORDINATE_VALUE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 23:
			if(mGps.calcAngle(0 , 0) == CALCANGLE_VALUE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 24:
			if(mGps.calcAngle(0, FLT_MAX*10) == CALCANGLE_ER_CASE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 25:
			if(mGps.calcAngle(FLT_MAX*10,0) == CALCANGLE_EL_CASE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 26:
			//if(mGps.calcRadius(10,10) == CALCRADIUS_VALUE)
			if((int)(mGps.calcRadius(10,10) - CALCRADIUS_VALUE) == 0)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 27:
			if(mGps.calcRadius(FLT_MAX*10,0) == CALCRADIUS_NAN)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 28:
			if(mGps.calcRadius(FLT_MAX*10,FLT_MIN) == CALCRADIUS_INF)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 29:
			mGps.calcDirection(360);
			if(mGps.getDirection() == CALC_DIRECTION_VALUE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 30:
			mGps.calcDistance();
			if(mGps.getDistance() == CALC_DISTANCE_VALUE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
				
		case 31:
			if(mGps.marge360(1000.0) == MARGET_PLUS)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 32:
			if(mGps.marge360(-1000.0) == MARGET_MINUS)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 33:
			if(mGps.marge360(0.0) == MARGE_INPUT_ZERO)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 34:
			if(mGps.radianToDegree(PI) == RAD_TO_DEG_PI_VALUE)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 35:
			if(mGps.radianToDegree(FLT_MAX*100) == RAD_TO_DEG_ERROR)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 36:
			if((int)(mGps.degreeToRadian(180) - DEG_TO_RAD_CORRECT) == 0)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 37:
			if(mGps.degreeToRadian(FLT_MAX*100) == DEG_TO_RAD_ERROR)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 38:
			mGps.update();
			if((mGps.getXCoordinate() != UPDATE_CHECK_X) && (mGps.getYCoordinate() != UPDATE_CHECK_Y) )
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 39:
			mGps.update();
			if(mGps.calcCircleCoordinates(10.0, 50.0, cirXCo, cirYCo) == CALCCIRCLE_CORRECT)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			mGps.setDirection(180.0);
			mGps.setDistance(0.0);
			break;
		case 40:
			if(mGps.calcCircleCoordinates(-10.0, 50.0, cirXCo, cirYCo) == CALCCIRCLE_CORRECT)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 41:
			if(mGps.calcCircleCoordinates(0.0, 50.0, cirXCo, cirYCo) == CALC_CIRCLE_ERROR)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 42:
			buf = mGps.calcCircleCoordinates(180.0, FLT_MAX*10, cirXCo, cirYCo);
			if(*cirXCo == -1.0)
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			break;
		case 43:
			mGps.calcCoordinates(90.0, 50.0, 720);
			if(((int)(mGps.getXCoordinate() - CALC_COORDINATES_CASE1_X) == 0 )
				&& ((int)(mGps.getYCoordinate() - CALC_COORDINATES_CASE1_Y) == 0 ))
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			
			break;
		case 44:
			mGps.calcCoordinates(-90.0, 50.0, 720);
			if(((int)(mGps.getXCoordinate() - CALC_COORDINATES_CASE2_X) == 0 )
				&& ((int)(mGps.getYCoordinate() - CALC_COORDINATES_CASE2_Y) == 0 ))
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			break;
		case 45:
			mGps.calcCoordinates(0.0, 50.0, 720);
			if(((int)(mGps.getXCoordinate() - CALC_COORDINATES_CASE3_X) == 0 )
				&& ((int)(mGps.getYCoordinate() - CALC_COORDINATES_CASE3_Y) == 0 ))
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			break;
		case 46:
			mGps.calcCoordinates(90.0, 150.0, 720);
			if(((int)(mGps.getXCoordinate() - CALC_COORDINATES_CASE4_X) == 0 )
				&& ((int)(mGps.getYCoordinate() - CALC_COORDINATES_CASE4_Y) == 0 ))
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			break;
		case 47:
			mGps.calcCoordinates(-90.0, 150.0, 720);
			if(((int)(mGps.getXCoordinate() - CALC_COORDINATES_CASE5_X) == 0 )
				&& ((int)(mGps.getYCoordinate() - CALC_COORDINATES_CASE5_Y) == 0 ))
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			break;
		case 48:
			mGps.calcCoordinates(0.0, 50.0, 2160);
			if(((int)(mGps.getXCoordinate() - CALC_COORDINATES_CASE6_X) == 0 )
				&& ((int)(mGps.getYCoordinate() - CALC_COORDINATES_CASE6_Y) == 0 ))
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			break;
		case 49:
			mGps.calcCoordinates(90.0, FLT_MAX*10, FLT_MAX*10);
			if(((int)(mGps.getXCoordinate() - CALC_COORDINATES_CASE7_X) == 0 )
				&& ((int)(mGps.getYCoordinate() - CALC_COORDINATES_CASE7_Y) == 0 ))
			{
				judge(i + 1, true);
				successCount++;
			}
			else
			{
				judge(i + 1, false);
			}
			mGps.setXCoordinate(100.0);
			mGps.setYCoordinate(-100.0);
			break;
		default:
			break;
		}
	}
	cout << "RESULT ------------------------------------------" << endl;
	cout << "CORRECT = " << successCount << "/" << totalNum << endl;

	return 0;
}

void judge(int testNumber, bool result)
{
	if(result)
	{
		cout << "No." << testNumber << ":OK!" << endl;
	}
	else
	{
		cout << "No." << testNumber << ":NG!" << endl;
	}
}

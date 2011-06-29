///////////////////////////////////////////////////////////
//  TestMath.cpp
//  testcode of the math.h
//  Created on:      29-9-2010 9:49:00
//  Original author: nseo
///////////////////////////////////////////////////////////

// g++ TestMath.cpp -lm
#include <math.h>
#include <iostream>
using namespace std;

double myatan2(double y, double x)
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

int main()
{
    float currentX = 2337;
    float currentY = -490;
    float targetX = 0;
    float targetY = 0;
    
    double diffX = targetX - currentX;
    double diffY = targetY - currentY;
    
    cout << atan2(diffY, diffX) << endl;
    cout << myatan2(1.0, 0.0) << endl;
    cout << myatan2(0.0, 1.0) << endl;
    cout << myatan2(0.0, 0.0) << endl;
    cout << myatan2(1.0, 1.0) << endl;
    cout << myatan2(-1.0, 1.0) << endl;
    cout << myatan2(1.0, -1.0) << endl;
    cout << myatan2(-1.0, -1.0) << endl;
}


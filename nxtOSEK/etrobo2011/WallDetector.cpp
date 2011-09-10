///////////////////////////////////////////////////////////
//  WallDetector.cpp
//  Implementation of the Class WallDetector
//  Created on:      14-9-2010 9:22:12
//  Original author: skawasak
///////////////////////////////////////////////////////////

#include "WallDetector.h"
#include "factory.h"

/**
 * 衝突検知する
 *
 * @retval true 検知した
 * @retval false 検知しなかった
 */
bool WallDetector::detect(){
//  float accR = mRightMotorSpeedHistory.calcDifference();
//  float accL = mLeftMotorSpeedHistory.calcDifference();
    float gyro = mGyroHistory.calcDifference();
    
#if 0
    {
        Lcd lcd;
        lcd.clear();
        lcd.putf("dn", (int)mRightMotorHistory.get());
        lcd.putf("dn", (int)mLeftMotorHistory.get());
        lcd.putf("dn", (int)mRightMotorSpeedHistory.get());
        lcd.putf("dn", (int)mLeftMotorSpeedHistory.get());
        lcd.putf("dn", (int)accL);
        lcd.disp();
    }
#endif
    //return (gyro > 60);
    return (gyro > mThreshold);
//    return (accR < WALL_THRESHOLD || accL < WALL_THRESHOLD);
}

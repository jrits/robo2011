#ifndef _GPS_H_
#define _GPS_H_

#include "SensorHistory.hpp"
#include "Direction.hpp"
#include "parameters.hpp"
#include <cmath>

class Gps
{
private:
    SensorHistory &sensorHistory;
    Direction &direction;
    S16 motorCountL, motorCountR;
    S16 previousMotorCountL, previousMotorCountR;
    F32 previousDirection, currentDirection;
    
    Lcd lcd;
    VectorT<double> world;
public:
    Gps(SensorHistory &sh,
        Direction &d):
        sensorHistory(sh),
        direction(d),
        motorCountL(0),
        motorCountR(0),        
        previousMotorCountL(0),
        previousMotorCountR(0),
        previousDirection(0.0),
        currentDirection(0.0),
        world(0.0, 0.0)
        {};
    
    ~Gps(){};

public:
    VectorT<double> getCoordinates() {
        return world;
    }
    void calcCoordinates() {
        motorCountL = sensorHistory.getLeftMotorCountHistory();        
        motorCountR = sensorHistory.getRightMotorCountHistory();

        //前回のエンコーダ値からの差分を計算する。
        S16 dMCL = motorCountL - previousMotorCountL; ///<左モータエンコーダ値の差分。
        S16 dMCR = motorCountR - previousMotorCountR; ///<右モータエンコーダ値の差分。

        //差分の計算のため、１ステップ前のエンコーダ値を保存。
        previousMotorCountL = motorCountL;
        previousMotorCountR = motorCountR;

        //ローカル座標系から、グローバル座標系に変換するために走行体の現在の向きを計算。
        currentDirection = direction.getDirection();

        //ローカル座標系での、座標の変化を計算するために、前回からの進行方向の差を計算する。
        F32 dD = currentDirection - previousDirection;

        //差分計算
        previousDirection = currentDirection;
        
        const S16 STRAIGHT = 0;
        const S16 RIGHT    = 1;
        const S16 LEFT     = 2;
        const S16 TURN     = 3;
		
        S16 advancement = -1;
        if((dMCR * dMCL) < 0){
            advancement = TURN;
        } else if(dMCR == dMCL){
            advancement = STRAIGHT;
        } else if(dMCR < dMCL){
            advancement = RIGHT;
        } else if(dMCR > dMCL){
            advancement = LEFT;
        } else {
            //ここにはこないはず
            advancement = -1;
        }
		
        double RdX = 0.0;//Relativity
        double RdY = 0.0;//Relativity 
        double AdX = 0.0;//Absolutely
        double AdY = 0.0;//Absolutely 
        switch (advancement) {
        case STRAIGHT:
            RdX = 0;
            RdY = (M_PI*dMCL*WHEEL_RADIUS)/180.0;
            break;
        case RIGHT:
            RdX = (((WHEEL_RADIUS * dMCL)/(2 * M_PI * (dMCL-dMCR))) + (WHEEL_RADIUS/2.0))*(cos(dD)-1);
            RdY = (((WHEEL_RADIUS * dMCL)/(2 * M_PI * (dMCL-dMCR))) + (WHEEL_RADIUS/2.0))*(sin(dD));
            break;
        case LEFT:
            RdX = -((((WHEEL_RADIUS * dMCR)/(2 * M_PI * (dMCR-dMCL))) + (WHEEL_RADIUS/2.0))*(cos(dD)-1));
            RdY = -(((WHEEL_RADIUS * dMCL)/(2 * M_PI * (dMCL-dMCR))) + (WHEEL_RADIUS/2.0))*(sin(dD));
            break;
        case TURN:
            //計算式を追加してちょ
            break;	
        default:
            //ここにはこない
            break;
        }
#if 0
        AdX = RdX*cos(currentDirection)+RdY*sin(currentDirection);
        AdY = RdY*cos(currentDirection)+RdX*sin(currentDirection);
#endif
        world += VectorT<double>(AdX, AdY);
    }
};

#endif /* _GPS_H_ */

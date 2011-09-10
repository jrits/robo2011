//
// Gps.h
//
#ifndef GPS_H_
#define GPS_H_

#include "Motor.h"
#include "constants.h"
#include "macro.h"
#include "struct.h"
#include <math.h>
#include <float.h>

//#define PI 3.14159265358979
#define X_LEFT  0    // ロボコンコース上のx座標最左値
#define X_RIGHT 5450 // ロボコンコース上のx座標最右値
#define Y_TOP   0    // ロボコンコース上のy座標最上値
#define Y_DOWN -3600 // ロボコンコース上のy座標最下値
// ex) SIGN(mTargetX - currentX) == PASSTO_LEFT
// ex) SIGN(mTargetX - currentX) == PASSTO_RIGHT
#define PASSTO_LEFT -1 // 左に向かって目標地点を通り過ぎた
#define PASSTO_RIGHT 1 // 右に向かって目標地点を通り過ぎた
#define PASSTO_TOP   1 // 上に向かって目標地点を通り過ぎた
#define PASSTO_DOWN -1 // 下に向かって目標地点を通り過ぎた

using namespace ecrobot;

/**
 * GPS(自己位置推定)クラス
 */
class Gps
{
public:
    /**
     * コース識別子
     *
     * 自動補正対象コースの識別に利用
     */
    enum eCourse {
        IN, //!< インコース
        OUT //!< アウトコース
    };
private:
    float mWheelRadius; //!< 車輪半径(mm)
    float mWheelDistance; //!< 車輪間隔(mm)
    Motor &motorL; //!< 左モータ
    Motor &motorR; //!< 右モータ
    Gps::eCourse mCourse; //!< コース識別子
    float mXCoordinate; //!< X座標
    float mYCoordinate; //!< Y座標
    float mDirection; //!< 向き
    float mDistance; //!< 距離
    float mXOffset; //!< X座標補正値
    float mYOffset; //!< Y座標補正値
    float mDirectionOffset; //!< 向き補正値
    float mDistanceOffset; //!< 距離補正値
    
    //以下補正関係変数
    float mXAverage; //!< X座標平均
    float mYAverage; //!< Y座標平均
    float mDirectionAverage; //!< 向き平均
    int mTimeCounter; //!< 時間カウンタ
public:
    Gps(Motor &aMotorL, Motor &aMotorR, Gps::eCourse aCourse);
    ~Gps(){}
    void update();
    float getXCoordinate();
    float getYCoordinate();
    float getDirection();
    float getDistance();
    void adjustXCoordinate(float trueValue);
    void adjustYCoordinate(float trueValue);
    void adjustDirection(float trueValue);
    void adjustDistance(float trueValue);
public:
    float calcAngle(float el, float er);
    float calcRadius(float encoderLeft, float angle);
    void calcDirection(float angle);
    void calcDistance();
    bool calcCenterCoordinates(float angle, float radius, float *circleX, float *circleY);
    void calcCoordinates(float angle, float distance, float encoderL, float encoderR);
private:
    // 自動補正関数
    void adjustPositionOut(float avgX, float avgY, float avgD);
    void adjustPositionIn(float avgX, float avgY, float avgD);
public:
    // クラスメソッド
    static float marge360(float margeTarget);
    static float marge180(float margeTarget);
    static float radianToDegree(float radian);
    static float degreeToRadian(float degree);
    static double atan2(double y, double x);
    float calcDistanceTo(Point targetCoordinate);
public:
    // obsolete 
    void setYCoordinate(float yCo);
    void setXCoordinate(float xCo);
    void setDirection(float direction);
    void setDistance(float distance);
};

#endif

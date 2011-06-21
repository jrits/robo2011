#ifndef STRUCT_H_
#define STRUCT_H_

#include <math.h>

/**
 * 点
 */
typedef struct _TAGPOINT{
	float X; //!< X座標 単位mm 原点はコース左端 右に行くにつれて+
	float Y; //!< Y座標 単位mm 原点はコース上端 下に行くにつれて-
} Point;

/**
 * GpsVisualizer制御点
 */
typedef struct _TAGCONTROLPOINT {
    float X;              //!< 目標X座標(mm)。座標指定走行しない場合はFP_NANを指定。
    float Y;              //!< 目標Y座標(mm)。座標指定走行しない場合はFP_NANを指定。
    float direction;      //!< 目標向き。方向転換しない場合はFP_NANを指定。
    float forward;        //!< 座標指定走行用。フォワード値。
    float allowableError; //!< 共用。許容誤差(mm)。
    bool  slowdown;       //!< 座標指定走行用。SlowdownSkill を使用するかどうか。
} ControlPoint;

/**
 * 領域
 */
typedef struct _TAGREGION {
    float startX; //!< 始点X(左上)
    float startY; //!< 始点Y(左上)
    float endX;   //!< 終点X(右下)
    float endY;   //!< 終点Y(右下)
} Region;

/**
 * GPS値
 */
typedef struct _TAGGPSVALUE {
    float X;         //!< x座標
    float Y;         //!< y座標
    float direction; //!< 車体向き
    float distance;  //!< 距離
} GpsInfo;

/**
 * 自動補正情報構造体
 */
typedef struct _TAGGPSOFFSET {
    Region region;   //!< この領域に入ったら補正を行う
    GpsInfo gpsInfo; //!< NaNのデータは補正対象外
} GpsOffset;

//=============== Utility 関数(将来的にファイルを分ける可能性あり) ==========

/**
 * Point を作成する
 *
 * @param[in] X x座標(mm)。原点はコース左端。右に行くにつれて+
 * @param[in] Y y座標(mm)。原点はコース上端。下に行くにつれて-
 * @return point 点
 */
Point MakePoint(float X, float Y)
{
    Point point = {X, Y};
    return point;
}

/**
 * Region を作成する
 *
 * @param[in] startX 始点X(左上)
 * @param[in] startY 始点Y(左上)
 * @param[in] endX 終点X(左上)
 * @param[in] endY 終点Y(左上)
 * @return 領域
 */
Region MakeRegion(float startX, float startY, float endX, float endY)
{
    Region region = {startX, startY, endX, endY};
    return region;
}

/**
 * GpsInfo を作成する
 *
 * @param[in] X x座標
 * @param[in] Y y座標
 * @param[in] direction 車体向き
 * @param[in] distance 距離
 * @return GpsInfo
 */
GpsInfo MakeGpsInfo(float X, float Y, float direction, float distance)
{
    GpsInfo gpsInfo = {X, Y, direction, distance};
    return gpsInfo;
}

/**
 * 指定した点が領域内に存在するか判定する
 *
 * @param[in] aRegion 領域
 * @param[in] aPoint  点
 *
 * @retval true 領域内
 * @retval false 領域外
 */
bool inRegion(Region aRegion, Point aPoint)
{
    return (aRegion.startX <= aPoint.X && aPoint.X <= aRegion.endX &&
            aRegion.endY <= aPoint.Y && aPoint.Y <= aRegion.startY);
}

/**
 * ２点間の距離を計算する
 *
 * @param[in] a 点A
 * @param[in] b 点B
 *
 * @return 距離
 */
double calcDistance(Point a, Point b)
{
    return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
}

#endif

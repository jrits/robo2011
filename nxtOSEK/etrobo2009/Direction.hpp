#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "SensorHistory.hpp"
#include "parameters.hpp"
#include <cmath>
#include "Speaker.h"

extern Motor motorL;
extern Motor motorR;

const S16 BUFF_SIZE = 200;
F32 directions[BUFF_SIZE] = {0};///< 角度の移動平均計算用バッファ。
F32 avgDirections[BUFF_SIZE] = {0};///< 微分(差分)計算用バッファ。

#define USE_CURVATURE 1
#if USE_CURVATURE
S16 leftMotorCounts[BUFF_SIZE] = {0}; ///< 本来はセンサヒストリにあるべき
F32 avgLeftMotorCounts[BUFF_SIZE] = {0};
//S16 rightMotorCounts[BUFF_SIZE] = {0}; ///< よくよく考えたら左エンコーダ値だけで充分
//F32 avgRightMotorCounts[BUFF_SIZE] = {0};
#endif

class Direction
{
private:
    SensorHistory &sensorHistory;
    F32 directionOffset;
    const F32 startDirection;

    U16 index; // リングバッファのベース添字。次に書き込む場所。
public:
    Direction(SensorHistory &sh,
        F32 sd = 0.0):
        sensorHistory(sh),
        directionOffset(0),
        startDirection(sd) {
        for(int i = 0; i < BUFF_SIZE; i++) {
            directions[i] = startDirection;
            avgDirections[i] = startDirection;
        }
        index = 0;
    };
    
    ~Direction(){};
    inline F32 getOffset() {
        return this->directionOffset;
    }
// Direction getters
public: 
    /**
     * 向きの取得
     *
     * @param t サンプル時間添字。
     *     t = 0 で最新値を、負数で過去の値を取得(例: t = -1　で直前値)。
     *     また内部でリングバッファを用いているため t = 1 で１周して最古値を取得可能。
     * @return 向き
     */
    inline F32 getDirection(int t = 0) {
        return directions[this->getIndex(t)];
    }
    /**
     * 移動平均向きの取得
     *
     * @param t サンプル時間添字。
     *     t = 0 で最新値を、負数で過去の値を取得(例: t = -1　で直前値)。
     *     また内部でリングバッファを用いているため t = 1 で１周して最古値を取得可能。
     * @return 移動平均向き
     */
    inline F32 getAvgDirection(int t = 0) {
        return avgDirections[this->getIndex(t)];
    }
   /**
     * オフセット値を考慮した向きの取得
     *
     * @param t サンプル時間添字。
     *     t = 0 で最新値を、負数で過去の値を取得(例: t = -1　で直前値)。
     *     また内部でリングバッファを用いているため t = 1 で１周して最古値を取得可能。
     * @return オフセット値を考慮した向き
     */
    inline F32 getDirectionOff(int t = 0) {
        return this->getDirection(t) - this->directionOffset;
    }
   /**
     * オフセット値を考慮した移動平均向きの取得
     *
     * @param t サンプル時間添字。
     *     t = 0 で最新値を、負数で過去の値を取得(例: t = -1　で直前値)。
     *     また内部でリングバッファを用いているため t = 1 で１周して最古値を取得可能。
     * @return オフセット値を考慮した移動平均向き
     */
    inline F32 getAvgDirectionOff(int t = 0) {
        return this->getAvgDirection(t) - this->directionOffset;
    }
    /**
     * 向きの微分値の計算。
     *
     * @param i 微分値の計算に用いるサンプル時間間隔
     * @return 微分値
     */
    inline F32 getDerivativeOfRawDirection(unsigned int i = 0) {
        if (i == 0) i = BUFF_SIZE - 1;
        return (this->getDirection(0) - this->getDirection(-i)) / (F32)i;
    }
    /**
     * 移動平均向きの微分値の計算。
     *
     * @param i 微分値の計算に用いるサンプル時間間隔
     * @return 微分値
     */
    inline F32 getDerivativeOfAvgDirection(unsigned int i = 0) {
        if (i == 0) i = BUFF_SIZE - 1;
        return (this->getAvgDirection(0) - this->getAvgDirection(-i)) / (F32)i;
    }
// Deprecated direction getters
public: 
    inline F32 getNewDirection() {
        return this->getDirection(0);
    }
    inline F32 getOldDirection() {
        return this->getDirection(1);
    }
    inline F32 getNewAvgDirection() {
        return this->getAvgDirection(0);
    }
    inline F32 getOldAvgDirection() {
        return this->getAvgDirection(1);
    }
    inline F32 getNewDirectionOff() {
        return this->getDirectionOff(0);
    }
    inline F32 getOldDirectionOff() {
        return this->getDirectionOff(1);
    }
    inline F32 getNewAvgDirectionOff() {
        return this->getAvgDirectionOff(0);
    }
    inline F32 getOldAvgDirectionOff() {
        return this->getAvgDirectionOff(1);
    }
    /**
     * Get the derivative of direction.
     *
     * This deprecated function does not divide the derivative by the time length.  
     *
     * @deprecated
     */
    inline F32 getDerivativeOfDirection() {
        return this->getAvgDirection(0) - this->getAvgDirection(1);
    }
// Motor count getters (本来はセンサヒストリにあるべき)
public:
#if USE_CURVATURE
    /**
     * 左モータエンコーダ値の取得
     *
     * @param t サンプル時間添字。
     *     t = 0 で最新値を、負数で過去の値を取得(例: t = -1　で直前値)。
     *     また内部でリングバッファを用いているため t = 1 で１周して最古値を取得可能。
     * @return 左モータエンコーダ値
     */
    inline F32 getLeftMotorCount(int t = 0) { 
        return leftMotorCounts[this->getIndex(t)];
    }
    /**
     * 移動平均左モータエンコーダ値の取得
     *
     * @param t サンプル時間添字。
     *     t = 0 で最新値を、負数で過去の値を取得(例: t = -1　で直前値)。
     *     また内部でリングバッファを用いているため t = 1 で１周して最古値を取得可能。
     * @return 移動平均左モータエンコーダ値
     */
    inline F32 getAvgLeftMotorCount(int t = 0) { 
        return avgLeftMotorCounts[this->getIndex(t)];
    }
    /**
     * 左モータエンコーダ値の微分値の計算。
     *
     * @param i 微分値の計算に用いるサンプル時間間隔
     * @return 微分値
     */
    inline F32 getDerivativeOfRawLeftMotorCount(unsigned int i = 0) {
        if (i == 0) i = BUFF_SIZE - 1;
        return (this->getLeftMotorCount(0) - this->getLeftMotorCount(-i)) / (F32)i;
    }
    /**
     * 移動平均左モータエンコーダ値の微分値の計算。
     *
     * @param i 微分値の計算に用いるサンプル時間間隔
     * @return 微分値
     */
    inline F32 getDerivativeOfAvgLeftMotorCount(unsigned int i = 0) {
        if (i == 0) i = BUFF_SIZE - 1;
        return (this->getAvgLeftMotorCount(0) - this->getAvgLeftMotorCount(-i)) / (F32)i;
    }
#endif

// Ring Buffer
public: 
    /**
     * Convert the ring buffer index to the physical memory index.
     *
     * @param i Ring buffer index. 
     *   0 denotes the newest value, and negatives denote the old values.
     *   You may use +1 to denote the oldest value. 
     * @return Physical memory index (index of buffer[] array)
     */
    inline unsigned int getIndex(int i) {
        return (this->index + i + BUFF_SIZE - 1) % BUFF_SIZE;
    }
    /**
     * Forward the newest pointer.
     *
     * @param -
     * @return -
     */
    inline void forwardIndex() {
        this->index = (this->index + 1) % BUFF_SIZE;
    }
    /**
     * リングバッファ履歴の更新
     *
     * @param -
     * @return -
     */
    void renewHistory() {
        // index: 次に書き込む場所
        F32 oldDirection = this->getDirection(1); ///< 最古値を一時的に保存
        F32 newDirection = directions[index] = this->computeDirection();

        avgDirections[index] = this->getAvgDirection(0) 
            + (newDirection - oldDirection)/(F32)BUFF_SIZE;

#if USE_CURVATURE // 本来はセンサヒストリにあるべき
        F32 oldLeftMotorCount = this->getLeftMotorCount(1); ///< 最古値を一時的に保存
        F32 newLeftMotorCount = leftMotorCounts[index] = sensorHistory.getLeftMotorCountHistory();
        
        avgLeftMotorCounts[index] = this->getLeftMotorCount(0)
            + (newLeftMotorCount - oldLeftMotorCount)/(F32)BUFF_SIZE;
#endif

        this->forwardIndex();
    }

// Computation (math equations)
public:
    /**
     * Estimate the absolute direction angle.
     *
     * @return The direction angle (deg).
     */
    F32 computeDirection() {
        S16 degR = sensorHistory.getRightMotorCountHistory();
        S16 degL = sensorHistory.getLeftMotorCountHistory();
        F32 direction = WHEEL_RADIUS * (degR - degL) / WHEEL_DISTANCE;
        return direction + this->startDirection;
    }

#if USE_CURVATURE
    /**
     * Estimate the curvature (1/radius) of running curve. 
     *
     * @param i The sampling time interval to get derivative values.
     * @return The curvature (1/radius). +: left curve. -: right curve.
     *         Be careful that you may receive Inf or -Inf.
     */
     float computeRawCurvature(int i = 0) {
         float dLeftMotorCount = this->getDerivativeOfRawLeftMotorCount(i);
         float dAngle = this->getDerivativeOfRawDirection(i);
        
         float radius = // +: left curve. -: right curve.
             (WHEEL_RADIUS * dLeftMotorCount / dAngle) + WHEEL_DISTANCE / 2;
         float curvature = 1 / radius; 
         if (isnan(curvature)) {
             curvature = 0.0;
         }
         return curvature;
     }

    /**
     * Estimate the moving averaged curvature (1/radius) of running curve. 
     *
     * @param i The sampling time interval to get derivative values.
     * @return The curvature (1/radius). +: left curve. -: right curve.
     *         Be careful that you may receive Inf or -Inf.
     */
     float computeAvgCurvature(int i = 0) {
         float dLeftMotorCount = this->getDerivativeOfAvgLeftMotorCount(i);
         float dAngle = this->getDerivativeOfAvgDirection(i);
        
         float radius = // +: left curve. -: right curve.
             (WHEEL_RADIUS * dLeftMotorCount / dAngle) + WHEEL_DISTANCE / 2;
         float curvature = 1 / radius; 
         if (isnan(curvature)) {
             curvature = 0.0;
         }
         return curvature;
     }
#endif

// Offset trigers
public:
    bool calcDirectionOffset90(){
        static int flagcount = 0;
        static bool done = false;

        if(done) return done;

        F32 derivativeOfDirection = this->getDerivativeOfDirection();
        F32 newAvgDirectionOff = this->getNewAvgDirectionOff();
#ifdef PRINT_CALC_DIRECTION_OFFSET_90
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "calcDirectionOff");
        lcd.putf("sdn", "AVG", (S32)newAvgDirectionOff);
        lcd.putf("sdn", "DRV", (S32)derivativeOfDirection);
        lcd.putf("sdn", "CNT", flagcount);
        lcd.disp();
#endif
        if ((std::abs(derivativeOfDirection) < DIRECTION_DEGREE_OFFSET_90_TRIGER_DEGREE_DERIVATIVE_BOUNDS) 
        	&& newAvgDirectionOff > DIRECTION_DEGREE_OFFSET_90_TRIGER_DEGREE_LOWER) { 
            // 微分値が0付近(直線上)かつ９０度付近(ここではかなりゆるくとっておく)
            flagcount++;
        } else {
            flagcount--;
        }
        if (flagcount < 0) flagcount = 0;
        if (flagcount >= DIRECTION_DEGREE_OFFSET_90_TRIGER_FLAGCOUNT) { // 第二ストレートと認識した
#ifdef BEEP_AT_TRIGER
            beep();
#endif
            done = true;
            this->directionOffset =  // BUFF_SIZE*2 コ分の平均
                (newAvgDirectionOff + this->getOldAvgDirectionOff()) / 2.0; 
            this->directionOffset -= 90; // 90度用オフセットに変更
            
#ifdef PRINT_CALC_DIRECTION_OFFSET_90
            lcd.putf("sdn", "OFF", (S32)this->directionOffset);
#endif
        }
        return done;
    }

    bool calcDirectionOffset180(){
        static int flagcount = 0;
        static bool done = false;

        if(done) return done;
        
        F32 derivativeOfDirection = this->getDerivativeOfDirection();
        F32 newAvgDirectionOff = this->getNewAvgDirectionOff();
        if ((std::abs(derivativeOfDirection) < DIRECTION_DEGREE_OFFSET_180_TRIGER_DEGREE_DERIVATIVE_BOUNDS) 
            && newAvgDirectionOff > DIRECTION_DEGREE_OFFSET_180_TRIGER_DEGREE_LOWER 
        	&& newAvgDirectionOff < DIRECTION_DEGREE_OFFSET_180_TRIGER_DEGREE_UPPER) { 
            flagcount++;
        } else {
            flagcount--;
        }
        if (flagcount < 0) flagcount = 0;
        if (flagcount >= DIRECTION_DEGREE_OFFSET_180_TRIGER_FLAGCOUNT) {
#ifdef BEEP_AT_TRIGER
            beep();
#endif
            done = true;
            this->directionOffset =  // BUFF_SIZE*2 コ分の平均
                (newAvgDirectionOff + this->getOldAvgDirectionOff()) / 2.0; 
            this->directionOffset -= 180; // 180度用オフセットに変更
        }
        return done;
    }

};

#endif /* _DIRECTION_H_ */

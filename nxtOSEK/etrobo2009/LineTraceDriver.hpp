#ifndef LINE_TRACE_DRIVER_H_
#define LINE_TRACE_DRIVER_H_

#include "Driver.hpp"
#include "PidController.hpp"
#include "parameters.hpp"

#include "Color.hpp" // Just for now. ToDo: refactoring
extern Color color;  // Just for now. ToDo: refactoring

class LineTraceDriver : public Driver
{
private:
    float refCurvature;
    float curvY;
    Lcd lcd;
public:
    enum EDGE tracedEdge;
    LineTraceDriver(BalancingRun &b, Distance &d, Direction &g, PidController &lp, PidController &ap, PidController &cp, Clock &c, SensorHistory &sh):
        Driver(b,d,g,lp,ap,cp,c,sh)
        {
            this->initialize();
        };
    ~LineTraceDriver(){};

public:
    void inline initialize(EDGE edge = TRACED_EDGE) {
        tracedEdge = edge;
        this->refCurvature = direction.computeAvgCurvature();
        curvY = 0.0;
    }
    void inline setEdge(EDGE edge) {tracedEdge = edge;};
    void inline setRefCurvature(float rc) {refCurvature = rc;};
    // obsolete
    void inline switchEdge() {
        if(tracedEdge == RIGHT){
            tracedEdge = LEFT;
        } else if(tracedEdge == LEFT){
            tracedEdge = RIGHT;
        }
    }

private:
    float computeVelocityByLight(float refLight = WHITE_GRAY_THRESH)
    {
        F32 curLight = color.getLightSensorValue();
        // 「黒と目標値の偏差」が「白と目標値の偏差」よりも大きいため正規化する。
        curLight = curLight - refLight;
        if (curLight > 0) { // black
            curLight = curLight / (float)(BLACK_MEAN - refLight);
        } else { // white
            curLight = curLight / (float)(refLight - WHITE_MEAN);
        }
        float Y = lightPid.control(curLight, 0.0);
        return Y;
    }
    float computeVelocityByCurvature()
    {
        // 曲率PIDを基準に光PIDで振り幅を決める
        static int state = -1;
        float curv = 0.0, dCurvY = 0.0;
        // 起動時の static Y += dY がなにか怪しいので 2sec 別の処理をしておく
        if (state == -1) {
            time.reset();
            state = 0;
        }
        if (state == 0) {
            if (time.elapsedTime() > 2000) {
                state = 1;
            }
        } else if (state == 1) {
            curv = direction.computeAvgCurvature();
            dCurvY = curvPid.control(curv, this->refCurvature);
            this->refCurvature = curv; // ToDo: 次の目標値を直前の値とするのではなく、マップから先読みする。
            curvY += dCurvY; 
        }

#if PRINT_LINE_TRACE_DRIVER
        lcd.clear();
        lcd.putf("sn", "LineTraceDriver");
        lcd.putf("sdn", "CV ", (S32)(curv * 1000) );
        lcd.putf("sdn", "DCV", (S32)(dCurvY * 1000));
#endif

        return curvY;
    }
public:

    VectorT<S8> computeVelocityWithCurv(float refLight = WHITE_GRAY_THRESH)
    {
        VectorT <S8> velocity;
        float curvY  = this->computeVelocityByCurvature();
        float lightY = this->computeVelocityByLight(refLight);
        float Y = curvY + lightY;
        if (lightY > 0) { // 黒線は細い。白よりも急激に曲がらないとコースアウトしてしまう可能性がある。
            Y *= ENHANCE_Y_FOR_BLACK_COEF; // usually ENHANCE_Y_FOR_BLACK_COEF > 1.0
        }

#if PRINT_LINE_TRACE_DRIVER
        lcd.putf("sdn", "CVY", (S32)(curvY * 1000));
        lcd.putf("sdn", "LTY", (S32)(lightY * 1000));
        lcd.putf("sdn", "  Y", (S32)(Y * 1000));
        lcd.disp();
#endif

        velocity = this->controlVelocity(Y);
        if(tracedEdge == LEFT) velocity.mY *= -1;
        return velocity;
    }

    VectorT<S8> computeVelocity(float refLight = WHITE_GRAY_THRESH)
    {
        VectorT <S8> velocity;
        float curvY  = 0.0;
        float lightY = this->computeVelocityByLight(refLight);
        float Y = curvY + lightY;
        if (lightY > 0) { // 黒線は細い。白よりも急激に曲がらないとコースアウトしてしまう可能性がある。
            Y *= ENHANCE_Y_FOR_BLACK_COEF; // usually ENHANCE_Y_FOR_BLACK_COEF > 1.0
        }

#if PRINT_LINE_TRACE_DRIVER
        lcd.putf("sdn", "CVY", (S32)(curvY * 1000));
        lcd.putf("sdn", "LTY", (S32)(lightY * 1000));
        lcd.putf("sdn", "  Y", (S32)(Y * 1000));
        lcd.disp();
#endif

        velocity = this->controlVelocity(Y);
        if(tracedEdge == LEFT) velocity.mY *= -1;
        return velocity;
    }
};

#endif /* LINE_TRACE_DRIVER_H_ */

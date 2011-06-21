#ifndef CURVATURE_PID_DRIVER_H_
#define CURVATURE_PID_DRIVER_H_

#include "Driver.hpp"
#include "PidController.hpp"
#include "parameters.hpp"

extern PidController curvPid;
extern long DEBUG_DATA_LONG;
extern S8 DEBUG_DATA2;

class CurvaturePidDriver : public Driver
{
private:
public:
    CurvaturePidDriver(BalancingRun &b, Distance &d, Direction &g, PidController &lp, PidController &ap, PidController &cp, Clock &c, SensorHistory &sh):
        Driver(b,d,g,lp,ap,cp,c,sh)
        {
        };
    ~CurvaturePidDriver(){};

public:
    VectorT<S8> computeVelocity(float refCurvature)
    {
        static float Y = 0;
        F32 curv = direction.computeRawCurvature(CURVATURE_PID_DERIVATIVE_INTERVAL);
        // take care of -Inf and Inf.
        curv = (-ALLOWABLE_CURVATURE > curv ? -ALLOWABLE_CURVATURE : 
                (ALLOWABLE_CURVATURE < curv ? ALLOWABLE_CURVATURE : curv));
        float dY = curvPid.control(curv, refCurvature);
        Y += dY;
        VectorT<S8> velocity = this->controlVelocity(Y);
        
#if PRINT_CURVATURE_PID_DRIVER
        DEBUG_DATA2 = velocity.mY;
        DEBUG_DATA_LONG = (S32)direction.getDirectionOff();
        //DEBUG_DATA_LONG = (S32)(direction.getDerivativeOfAvgDirection(CURVATURE_PID_DERIVATIVE_INTERVAL) * 100);
        //DEBUG_DATA_LONG = (S32)(curv * 1000);
        F32 avg = direction.computeAvgCurvature();
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn", "CurvPid * 1000");
        lcd.putf("sdn", "Raw", (S32)(curv * 1000) );
        lcd.putf("sdn", "Avg", (S32)(avg * 1000) );
        lcd.putf("sdn", " dY", (S32)(dY * 1000));
        lcd.putf("sdn", "  Y", (S32)(Y * 1000));
        lcd.putf("sdn", " mX", (S32)velocity.mX);
        lcd.putf("sdn", " mY", (S32)velocity.mY);
        lcd.disp();
#endif
        return velocity;
    }
};

#endif /* CURVATURE_PID_DRIVER_H_ */

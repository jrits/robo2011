#ifndef DIRECTION_PID_DRIVER_H_
#define DIRECTION_PID_DRIVER_H_

#include "Driver.hpp"
#include "PidController.hpp"
#include "parameters.hpp"

extern long DEBUG_DATA_LONG;

class DirectionPidDriver : public Driver
{
private:
public:
    DirectionPidDriver(BalancingRun &b, Distance &d, Direction &g, 
                       PidController &lp, PidController &ap, PidController &cp, 
                       Clock &c, SensorHistory &sh):
        Driver(b,d,g,lp,ap,cp,c,sh)
        {
        };
    ~DirectionPidDriver(){};

public:
    VectorT<S8> computeVelocity(float refDirection)
    {
        VectorT <S8> velocity;
        F32 curDirection = direction.getDirectionOff();

        // Y そのものバージョン
        float Y = anglePid.control(curDirection, refDirection);
        velocity = this->controlVelocity(Y);

        // dY バージョン
        // (全然だめ。むしろ目標値付近でYの値が大きくなる)
        //anglePid.setKp(0.001);
        //anglePid.setKi(0.0);
        //anglePid.setKd(0.0);
        //static float Y = 0.0;
        //float dY = anglePid.control(curDirection, refDirection);
        //Y += dY;
        //velocity = this->controlVelocity(Y);
        
#if PRINT_DIRECTION_PID_DRIVER
        DEBUG_DATA_LONG = (long)curDirection;
        Lcd lcd;
        lcd.clear();
        lcd.putf("sn",  "DirectionPidDriver");
        lcd.putf("sdn", "RDIR", (int)refDirection);
        lcd.putf("sdn", "CDIR", (int)curDirection);
        lcd.putf("sdn", "Raw ", (int)direction.getDirection());
        lcd.putf("sdn", "RawO", (int)direction.getDirectionOff());
        lcd.putf("sdn", "Avg ", (int)direction.getAvgDirection());
        lcd.putf("sdn", "AvgO", (int)direction.getAvgDirectionOff());
        lcd.putf("sdn", "OFF ", (int)direction.getOffset());
        lcd.putf("sdn", "   Y", (int)Y);
        lcd.disp();
#endif
        return velocity;
    }
};

#endif /* DIRECTION_PID_DRIVER_H_ */

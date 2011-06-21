#ifndef DRIVER_H_
#define DRIVER_H_

#include "BalancingRun.hpp"
#include "Distance.hpp"
#include "Direction.hpp"
#include "PidController.hpp"
#include "Time.hpp"
#include "SensorHistory.hpp"

/** 各走行部品のインターフェースを定義するスーパークラス。
 */
class Driver
{
protected:
    BalancingRun  &bRun;
    Distance      &distance;
    Direction     &direction;
    PidController &lightPid;
    PidController &anglePid;
    PidController &curvPid;
    Time          time;///<このメンバは参照型ではないことに注意！！
    SensorHistory &history;

    S8            speed;
public:
    Driver(BalancingRun &b, Distance &d, Direction &g, PidController &lp, PidController &ap, PidController &cp, Clock &c, SensorHistory &sh):
         bRun(b),
         distance(d),
         direction(g),
         lightPid(lp),
         anglePid(ap),
         curvPid(cp),
         time(c),
         history(sh) {
        speed = SPEED;
    };
    ~Driver(){};

public:
    inline void setSpeed(S8 s) {speed = s;};
    inline S8 getSpeed() {return speed;};
public:
    VectorT<S8> computeVelocity(float reference);
    void execute(float reference)
    {
        VectorT<S8> velocity = this->computeVelocity(reference);
        bRun.run(velocity);
    };
protected:
    VectorT<S8> controlVelocity(float Y)
    {
        VectorT<S8> velocity;
        Y = ( Y > 100 ? 100 : ( Y < -100 ? -100 : Y ) );
        velocity.mY = (S8)Y;
        velocity.mX = speed;
        //Lcd lcd;
        //lcd.putf("sdn", "mX", speed);
        //lcd.disp();
        
        /**
           走行体が、その場で旋回するのを防止する。
           
           横方向ベクトルが大きすぎても、最低速度で走行体が前進する。
        */
//     if(velocity.mY > speed){
//         velocity.mX = this->limitedSpeed;
//     }
//     else{
//         velocity.mX = (S8)sqrt(1.0*speed*speed - 1.0*velocity.mY*velocity.mY);
        
//         if(velocity.mX > 100)
//             velocity.mX = 100;
//         else if(velocity.mX < -100)
//             velocity.mX = -100;
//     }
        
        return velocity;
    }
};

#endif /* STRATEGY_H_ */

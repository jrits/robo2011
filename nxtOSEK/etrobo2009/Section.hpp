#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "BalancingRun.hpp"
#include "Distance.hpp"
#include "Direction.hpp"
#include "Driver.hpp"
#include "LineTraceDriver.hpp"
#include "DirectionPidDriver.hpp"
#include "CurvaturePidDriver.hpp"
#include "Time.hpp"
#include "SensorHistory.hpp"

/** 各走行法クラスのインターフェースを定義するスーパークラス。
 */
class Section
{
protected:
    BalancingRun       &bRun;
    Distance           &distance;
    Direction          &direction;
    LineTraceDriver    &lineTraceDriver;
    DirectionPidDriver &directionPidDriver;
    CurvaturePidDriver &curvaturePidDriver;
    Time               time;///<このメンバは参照型ではないことに注意！！
    SensorHistory      &history;
    bool               endFlag;
public:
    Section(BalancingRun &b, Distance &d, Direction &g, LineTraceDriver &lp, DirectionPidDriver &ap, CurvaturePidDriver &dap, Clock &c, SensorHistory &sh):
         bRun(b),
         distance(d),
         direction(g),
         lineTraceDriver(lp),
         directionPidDriver(ap),
         curvaturePidDriver(dap),
         time(c),
         history(sh),
         endFlag(false)
         {};

    Section();
    virtual ~Section(){};
    virtual bool isEnd(){return endFlag;}; // to be obsolete
    virtual bool execute(){return endFlag;};
protected:
    void countFlag(int &flag, bool cond)
    {
        if ( cond ) {
            flag++;
        } else {
            flag--;
        }
        if (flag < 0) flag = 0;
    };
};

#endif /* STRATEGY_H_ */

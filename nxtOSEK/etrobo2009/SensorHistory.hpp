#ifndef _SENSORHISTORY_H_
#define _SENSORHISTORY_H_

#include "Nxt.h"
#include "LightSensor.h"
#include "TouchSensor.h"
#include "Lcd.h"
#include "Bluetooth.h"
#include "GyroSensor.h"
#include "Motor.h"
#include "Clock.h"
#include "Daq.h"

using namespace ecrobot;

class SensorHistory
{
private:
    Nxt         &nxt;
    LightSensor &lightSensor;
    TouchSensor &touchL, &touchR;
    Lcd         &lcd;
    Bluetooth   &bt;
    GyroSensor  &gyro;
    Motor       &motorL, &motorR;

public:
    SensorHistory(Nxt & n,
                  LightSensor &ls,
                  TouchSensor &tL,
                  TouchSensor &tR,
                  Lcd &l,
                  Bluetooth &b,
                  GyroSensor &g,
                  Motor &mL,
                  Motor &mR) :
        nxt(n),
        lightSensor(ls),
        touchL(tL),
        touchR(tR),
        lcd(l),
        bt(b),
        gyro(g),
        motorL(mL),
        motorR(mR)
        {};

    ~SensorHistory(){};

    S16 getLightSensorHistory(S16 index=0) {return lightSensor.get();};
    S16 getBatteryVoltageHistory(S16 index=0) {return nxt.getBattMv();};
    S16 getLeftMotorCountHistory(S16 index=0) {return motorL.getCount();};
    S16 getRightMotorCountHistory(S16 index=0) {return motorR.getCount();};
    S16 getGyroSensorHistory(S16 index=0) {return gyro.get();};

};

#endif /* _SENSORHISTORY_H_ */

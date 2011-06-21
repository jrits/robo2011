// NXTway-GS++ sample.cpp for TOPPERS/ATK(OSEK)

#include <stddef.h>

// ECRobot++ API
#include "TouchSensor.h"
#include "GyroSensor.h"
#include "LightSensor.h"
#include "Motor.h"
#include "Nxt.h"
#include "Clock.h"
#include "Lcd.h"
#include "Bluetooth.h"
#include "Daq.h"
#include "Speaker.h"

using namespace ecrobot;

#include "TouchStarter.h"
#include "NXTway_GS.h"
#include "Calibrator.hpp"
#include "PidController.hpp"
#include "Color.hpp"
#include "SensorHistory.hpp"
#include "BalancingRun.hpp"
#include "Balancer.hpp"
#include "Time.hpp"
#include "Distance.hpp"
#include "Course.hpp"
#include "OutCourse.hpp"
#include "InCourse.hpp"
#include "InOutInCourse.hpp"
#include "InDolphinCourse.hpp"
#include "InTwinloopCourse.hpp"
#include "Direction.hpp"
#include "Gps.hpp"
#include "Driver.hpp"
#include "LineTraceDriver.hpp"
#include "DirectionPidDriver.hpp"
#include "CurvaturePidDriver.hpp"
//#define DEBUG_LINETRACE

//=============================================================================
// Device objects
Nxt         nxt;
LightSensor light(PORT_3);
TouchSensor touchL(PORT_1);
TouchSensor touchR(PORT_4);
Lcd         lcd;
Bluetooth   bt;
GyroSensor  gyro(PORT_2);
Motor       motorL(PORT_C);
Motor       motorR(PORT_B);
Clock       clock;

SensorHistory history(nxt,light,touchL,touchR,lcd,bt,gyro,motorL,motorR);
Color         color(history);
Distance      distance(history);
Balancer      balancer(history);
BalancingRun  balancingRun(balancer,motorL,motorR);
Time          time(clock);
//F32 defaultDirection = 180;
F32 defaultDirection = 0;
Direction     direction(history, defaultDirection);
Gps gps(history,direction);

// 走行部品
//LightPidController lightPid(color, KP, KI, KD);
//LightLinePidController lightPid(color, direction, KP, KI, KD);
PidController lightPid(KP, KI, KD, 10);
PidController anglePid(DIRECTION_PID_KP, DIRECTION_PID_KI, DIRECTION_PID_KD, DIRECTION_PID_SIZE);
PidController curvPid(CURVATURE_PID_KP, CURVATURE_PID_KI, CURVATURE_PID_KD, CURVATURE_PID_SIZE);
LineTraceDriver lineTraceDriver(balancingRun, distance, direction, lightPid, anglePid, curvPid, clock, history);
DirectionPidDriver directionPidDriver(balancingRun, distance, direction, lightPid, anglePid, curvPid, clock, history);
CurvaturePidDriver curvaturePidDriver(balancingRun, distance, direction, lightPid, anglePid, curvPid, clock, history);

// 走行法クラス。
// グローバルオブジェクトとして宣言しないと、メモリー破壊の可能性があるため、ここで宣言。
NormalLineSection   normalLineSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
GoalStopSection     goalStopSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
DashedLineSection   dashedLineSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
TreasureHuntSection treasureHuntSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
//DolphinSection      dolphinSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
//TwinloopSection     twinloopSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
//TestSection         testSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
NewDashedLineSection newDashedLineSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
NewTreasureHuntSection newTreasureHuntSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);
NewGoalStopSection newGoalStopSection(balancingRun, distance, direction, lineTraceDriver, directionPidDriver, curvaturePidDriver, clock, history);

// コース。走行法決定。
//InCourse        course(direction,distance,time,InCourse::START);
//OutCourse       course(direction,distance,time,OutCourse::START); // dashed and treasurehunt
InOutInCourse course(direction,distance,time,InOutInCourse::START);
//InDolphinCourse        course(direction,distance,time, InDolphinCourse::START);
//InTwinloopCourse        course(direction,distance,time, InTwinloopCourse::START);

S8 DEBUG_DATA1 = 0, DEBUG_DATA2 = 0;
S32 DEBUG_DATA_LONG = 0;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

    static void connect_bt(Lcd &lcd);

    //=============================================================================
    // TOPPERS/ATK declarations
    DeclareCounter(SysTimerCnt);
    DeclareTask(TaskDrive);
    DeclareTask(TaskMain);
    DeclareAlarm(Alarm4msec);
    DeclareEvent(EventDrive);

    //=============================================================================
    // 1msec timer interrupt hook
    void user_1ms_isr_type2(void)
    {
        (void)SignalCounter(SysTimerCnt); // Alarm counter

        SleeperMonitor(); // Need for Nxt and I2C device classes

        if (nxt.getButtons() == Nxt::ENTR_ON && !bt.isConnected())
        {
            bt.cancelWaitForConnection(); // Cancel Bluetooth connection process
        }
    }

    //=============================================================================
    // Drive Task
    TASK(TaskDrive)
    {
        Section *st;
        WaitEvent(EventDrive); // waiting for "Ready Go"
        
        while(1)
        {
#if PRINT_THREAD_TIME
            U32 now = clock.now();
#endif
            // ヒストリ更新
            direction.renewHistory();

            st = course.detect();
            //st = &normalLineSection;
            //st = &testSection;

            //PID制御走行
            st->execute();

#if PRINT_THREAD_TIME
            lcd.clear();
            lcd.putf("sdn", "CLK", clock.now() - now);
            lcd.putf("sdn", "MTL", motorL.getCount());
            lcd.putf("sdn", "MTR", motorR.getCount());
            lcd.putf("sdn", "DIR", (S32)direction.getDirectionOff());
            lcd.disp();
#endif

            ClearEvent(EventDrive); // Clear the drive event
            WaitEvent(EventDrive);  // Wait for drive event
        }
    }


    //=============================================================================
    // Main Task
    TASK(TaskMain)
    {
        Daq daq(bt);
        //Calibrator cal(lcd,touchL,touchR,light,color);

        //connect_bt(lcd); // Set up Blutooth connection with data logger.

//    	cal.calibration(pid);
        
//         lcd.clear();
//         lcd.putf("sn", "OUT == 3");
//         lcd.putf("sn", "and SPEED == 50?");
//         lcd.putf("sn", "IN == 0");
//         lcd.putf("sn", "and SPEED == 60?");
//         lcd.putf("sn", "CourseId");
//         lcd.putf("dn", course.getCourseId());
//         lcd.putf("sn", "SPEED");
//         lcd.putf("dn", SPEED);
//         lcd.disp();
        TouchStarter(touchL).waitForReadyGo();  // Touch sensor is used to start driving
        lcd.clear(true);

        SetRelAlarm(Alarm4msec, 1, 4); // Set 4msec periodical Alarm for the drive event

        while(1)
        {
            S8 data[2];
            S32 longData;

            data[0] = DEBUG_DATA1;
            data[1] = DEBUG_DATA2;
            longData = DEBUG_DATA_LONG;

            daq.send(data,longData);
            gps.calcCoordinates();

            clock.sleep(5); // Sleep this Task for 5msec
        }
    }

    //=============================================================================
    // Sub functions
    static void connect_bt(Lcd &lcd)
    {
        CHAR  name[16] = "Kachi";
        U8 address[7];

        lcd.clear();

        if (bt.getDeviceAddress(address)) // get the device address
        {
            lcd.putf("sn", "BD_ADDR:");
            for (SINT i=0; i<7; i++) lcd.putf("x", address[i],2);

            bt.setFriendlyName(name); // set the friendly device name
            if (bt.getFriendlyName(name)) // display the friendly device name
            {
                lcd.putf("nssn", "BD_NAME: ", name);
            }

            lcd.putf("nsn", "Connecting BT...");
            lcd.putf("sn",  "ENTR to cancel.");
            lcd.disp();

            if (bt.waitForConnection("1234", 0)) // wait forever
            {
                lcd.putf("s", "Connected.");
            }
        }
        else
        {
            lcd.putf("s", "BT Failed.");
        }

        lcd.putf("ns", "Press Touch.");
        lcd.disp();
    }

};

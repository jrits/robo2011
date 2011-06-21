#include "NormalLineSection.hpp"
#include "Lcd.h"

bool NormalLineSection::execute()
{
    VectorT<S8> velocity;

    if (state == -1) { // initialization state
        time.reset();
        state = 0;
        lineTraceDriver.initialize(bootEdge);
    }
    if (state == 0) {
        velocity = lineTraceDriver.computeVelocity();
        velocity.mX = bootSpeed;
        if (time.elapsedTime() > bootTime) {
            state = 1;
            lineTraceDriver.initialize(edge);
        }
    } 
    else if (state == 1) {
        velocity = lineTraceDriver.computeVelocityWithCurv();
        velocity.mX = speed;
    }
#if PRINT_DRIVER
    Lcd lcd;
    lcd.putf("sdn", "mX", velocity.mX);
    lcd.disp();
#endif
#if TORQUE_MOTOR
    bRun.run(velocity);
#endif
    return false;
}


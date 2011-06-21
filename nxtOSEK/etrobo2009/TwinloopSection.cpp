#include "TwinloopSection.hpp"

#define TWINLOOP 2

bool TwinloopSection::execute(){
    static int state = -1;
    static int flagCount = 0;
    VectorT<S8> velocity;
    static F32 myOffset;

#if PRINT_STATE
    Lcd lcd;
    lcd.clear();
    lcd.putf("sn","TwinloopSection");
    lcd.putf("sdn","SID", state);
    lcd.putf("sdn","Cnt", flagCount);
    lcd.putf("sdn","DST", (S32)distance.computeDistance());
    lcd.disp();
#endif
    
    if (state == -1) { // Initialization state
        state = 0;
        distance.reset();
        this->endFlag = false;
        flagCount = 0;
        myOffset = direction.getAvgDirectionOff() - 180;
    }
    if (state == 0) { //突入
        velocity = directionPidDriver.computeVelocity(TWINLOOP_STRATEGY_FORWARD_REFERENCE_DEGREE - myOffset);
        velocity.mX = TWINLOOP_STRATEGY_FORWARD_VELOCITY_X;
        if (distance.computeDistance() > TWINLOOP_STRATEGY_FORWARD_DISTANCE) {
            state = 1;
            distance.reset();
            flagCount = 0;
        }
    } else if(state == 1) { //バック
        velocity = directionPidDriver.computeVelocity(TWINLOOP_STRATEGY_BACKWARD_REFERENCE_DEGREE - myOffset);
        velocity.mX = TWINLOOP_STRATEGY_BACKWARD_VELOCITY_X;
        if (distance.computeDistance() < TWINLOOP_STRATEGY_BACKWARD_DISTANCE) {
            state = 2;
            distance.reset();
            flagCount = 0;
        }
    } else if(state == 2) { // 2nd Foward
        velocity = directionPidDriver.computeVelocity(TWINLOOP_STRATEGY_2ND_FORWARD_REFERENCE_DEGREE - myOffset);
        velocity.mX = TWINLOOP_STRATEGY_2ND_FORWARD_VELOCITY_X;
        if (distance.computeDistance() > TWINLOOP_STRATEGY_2ND_FORWARD_DISTANCE) {
            state = 3;
            distance.reset();
            flagCount = 0;
        }
#if TWINLOOP == 1
    } else if(state == 3) {//着地
        velocity = directionPidDriver.computeVelocity(TWINLOOP_STRATEGY_LANDING_REFERENCE_DEGREE - myOffset);
        velocity.mX = TWINLOOP_STRATEGY_LANDING_VELOCITY_X;
        
        countFlag(flagCount, history.getLightSensorHistory() > WHITE_GRAY_THRESH);
        if(flagCount > 1) {
            this->endFlag = true;
        }
    }
#elif TWINLOOP == 2
    } else if(state == 3) {//2nd ループへの横移動
    velocity = directionPidDriver.computeVelocity(360 - myOffset);
        velocity.mX = 80;

        if (distance.computeDistance() > 100) {
            state = 4;
            distance.reset();
            flagCount = 0;
        }
    } else if(state == 4) {
    velocity = directionPidDriver.computeVelocity(260 - myOffset);
        velocity.mX = -80;

        if (distance.computeDistance() < -10) {
            state = 5;
            distance.reset();
            flagCount = 0;
        }
    } else if(state == 5) {
        velocity = directionPidDriver.computeVelocity(260 - myOffset);
        velocity.mX = 25;

        if (distance.computeDistance() > 2) {
            state = 6;
            distance.reset();
            flagCount = 0;
        }
    } else if(state == 6) {
        velocity = directionPidDriver.computeVelocity(225 - myOffset);
        velocity.mX = 60;

        if (distance.computeDistance() > 40) { // 40 // 65
            state = 7;
            distance.reset();
            flagCount = 0;
        }
    } else if(state == 7) {
        velocity = directionPidDriver.computeVelocity(225 - myOffset); // 225 // 250
        velocity.mX = 20;

        countFlag(flagCount, history.getLightSensorHistory() > BLACK_GRAY_THRESH); // BLACK_GRAY_THRESH // WHITE_GRAY_THRESH
        if(flagCount > 1) {
            this->endFlag = true;
        }
    }
#endif

#if TORQUE_MOTOR
    bRun.run(velocity);
#endif
    if (this->isEnd()) {
        return true;
    } else {
        return false;
    }
}

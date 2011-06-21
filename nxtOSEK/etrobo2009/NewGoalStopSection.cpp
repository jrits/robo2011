#include "NewGoalStopSection.hpp"

VectorT<S8> NewGoalStopSection::computeRunningVelocity()
{
    VectorT<S8> velocity;
    velocity = lineTraceDriver.computeVelocity();
    return velocity;
}

VectorT<S8> NewGoalStopSection::computeStopVelocity(F32 refDirection)
{
    VectorT<S8> velocity;
    velocity = directionPidDriver.computeVelocity(refDirection);
    velocity.mX = GOAL_STOP_STRATEGY_STOP_VELOCITY_X;
    //velocity.mY = GOAL_STOP_STRATEGY_STOP_VELOCITY_Y;
    return velocity;
}

VectorT<S8> NewGoalStopSection::computeRebootVelocity(F32 refDirection)
{
    VectorT<S8> velocity;
    velocity = directionPidDriver.computeVelocity(refDirection);
    velocity.mX = 30;
    return velocity;
}

bool NewGoalStopSection::execute()
{
    Lcd lcd;
    static int state = -1;
    VectorT<S8> velocity;
    static F32 refDirection;

    lcd.putf("n");
    lcd.putf("sn", "NewGoalState ");
    lcd.putf("dn", state);
    lcd.disp();

    if (state == -1) { // initialization state
        this->distance.reset();
        state = 0;
    }
    if (state == 0) {
        velocity = this->computeRunningVelocity();
        
        if(this->distance.computeDistance() > distanceToGoal){
            state = 11;
            this->time.reset();
            this->distance.reset();
            refDirection = direction.getAvgDirectionOff();
        }
    } else if(state == 11) {
        velocity = directionPidDriver.computeVelocity(refDirection+90);
        velocity.mX = 80;
        beep();
        if(this->distance.computeDistance() > 30){
            state = 1;
            this->time.reset();
            this->distance.reset();
        }
    }
    else if (state == 1) { // 停止状態
        velocity = this->computeStopVelocity(refDirection);
        if (this->time.elapsedTime() > 7000/*GOAL_STOP_STRATEGY_FIRST_STOP_TIME*/) {
            state = 2;
            this->distance.reset();
        }
    } else if (state == 2) { // ライントレース
        velocity = this->computeRebootVelocity(refDirection);
        if(this->distance.computeDistance() > GOAL_STOP_STRATEGY_REPEATED_DISTANCE){
            state = 3;
            this->time.reset();
            this->distance.reset();
        }
    } else if (state == 3) { // 停止状態
        velocity = this->computeStopVelocity(refDirection);
        if (this->time.elapsedTime() > GOAL_STOP_STRATEGY_SECOND_STOP_TIME) {
            state = 4;
            this->distance.reset();
        }
    } else if (state == 4) { // ライントレース
        velocity = this->computeRebootVelocity(refDirection);
        if(this->distance.computeDistance() > GOAL_STOP_STRATEGY_REPEATED_DISTANCE){
            state = 5;
            this->time.reset();
            this->distance.reset();
        }
    } else if (state == 5) { // 停止状態
        velocity = this->computeStopVelocity(refDirection);
    }
#if TORQUE_MOTOR
    bRun.run(velocity);
#endif
    return false;
}
















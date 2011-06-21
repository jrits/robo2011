#include "NewTreasureHuntSection.hpp"

bool NewTreasureHuntSection::execute(){
    static int state = -1;
    static int state4Flagcount = 0;
//    static int state3Flagcount = 0;
    VectorT<S8> velocity;
    static F32 ref;

    Lcd lcd;
    lcd.clear();
    lcd.putf("sn","state");
    lcd.putf("dn",state);
    //lcd.disp();

    if (state == -1) // Initialization state
    {
        distance.reset();
        state = 0;
        ref = direction.getAvgDirectionOff();
    }
    if (state == 0) //先端突入
    {
        velocity = directionPidDriver.computeVelocity(ref);
        velocity.mX = TREASURE_HUNT_STRATEGY_FORWARD_VELOCITY_X;
        lcd.putf("dn", (S32)distance.computeDistance());
        lcd.disp();
        if (distance.computeDistance() > TREASURE_HUNT_STRATEGY_FORWARD_DISTANCE) {
            state = 1;
            distance.reset();
        }
    } else if(state == 1) { //バック
        velocity = directionPidDriver.computeVelocity(ref+TREASURE_HUNT_STRATEGY_BACKWARD_DIRECTION_DIFF);
        velocity.mX = TREASURE_HUNT_STRATEGY_BACKWARD_VELOCITY_X;
        lcd.putf("dn", (S32)distance.computeDistance());
        lcd.disp();
        if (distance.computeDistance() < TREASURE_HUNT_STRATEGY_BACKWARD_DISTANCE) {
            state = 3;
            distance.reset();
        }
    } else if(state == 3) {
        velocity = directionPidDriver.computeVelocity(ref+TREASURE_HUNT_STRATEGY_LANDING_DIRECTION_DIFF);
        velocity.mX = 60;

        if(distance.computeDistance() > NEW_TREASURE_HUNT_STRATEGY_STATE_3_DISTANCE) {
            state = 4;
            distance.reset();
        }
    } else if(state == 4) {//着地
        velocity = directionPidDriver.computeVelocity(ref+NEW_TREASURE_HUNT_STRATEGY_LANDING_DIRECTION);
        velocity.mX = 40;
        // TREASURE_HUNT_STRATEGY_LANDING_VELOCITY_X;

        countFlag(state4Flagcount, history.getLightSensorHistory() > WHITE_GRAY_THRESH);
        if(state4Flagcount > 1
           && distance.computeDistance() > NEW_TREASURE_HUNT_STRATEGY_LANDING_DISTANCE)
        {
            this->endFlag = true;
        }
    }

    if (this->isEnd()) {
        return true;
    } else {
#if TORQUE_MOTOR
        bRun.run(velocity);
#endif
        return false;
    }
}

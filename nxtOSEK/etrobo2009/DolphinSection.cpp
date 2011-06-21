#include "DolphinSection.hpp"

bool DolphinSection::execute() {
    static int state = -1;
    static int flagCount = 0;
    VectorT<S8> velocity;

#if PRINT_STATE
    Lcd lcd;
    lcd.clear();
    lcd.putf("sn",  "DolphinSection");
    lcd.putf("sdn", "SID", state);
    lcd.putf("sdn", "Cnt", flagCount);
    lcd.disp();
#endif
    
    if (state == -1) { // initialization state
        distance.reset();
        state = 0;
        flagCount = 0;
    }
    if (state == 0) {
        velocity = directionPidDriver.computeVelocity(DOLPHIN_STRATEGY_STRAIGHT_DIRECTION_TO_GO);
    	velocity.mX = DOLPHIN_STRATEGY_STRAIGHT_DIRECTION_TO_GO_SPEED;
        
        if(distance.computeDistance() > DOLPHIN_STRATEGY_DISTANCE_TO_LAND){
            state = 1;
            flagCount = 0;
            distance.reset();
        }
    } else if (state == 1) {
        velocity = directionPidDriver.computeVelocity(DOLPHIN_STRATEGY_STRAIGHT_DIRECTION_TO_LAND);
    	velocity.mX = DOLPHIN_STRATEGY_STRAIGHT_DIRECTION_TO_LAND_SPEED;
        countFlag(flagCount, history.getLightSensorHistory() > WHITE_GRAY_THRESH);
        if(flagCount > 1) {
            this->endFlag = true;
        }
    }

#if TORQUE_MOTOR
    bRun.run(velocity);
#endif
    if (this->isEnd()) {
        return true;
    } else {
        return false;
    }
};

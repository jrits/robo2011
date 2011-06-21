#include "NewDashedLineSection.hpp"

bool NewDashedLineSection::execute() {
    static int state = -1;
    static int state1Flagcount = 0;
    VectorT<S8> velocity;

#if PRINT_STATE
    Lcd lcd;
    lcd.clear();
    lcd.putf("sn",  "NewDashedState");
    lcd.putf("sdn", "SSID", state);
    lcd.disp();
#endif

    if (state == -1) { // initialization state
        distance.reset();
        state = 0;
    }
    if (state == 0) {
        velocity = directionPidDriver.computeVelocity(NEW_DASHED_LINE_SECTION_STRAIGHT_DIRECTION_TO_GO);        
    	velocity.mX = NEW_DASHED_LINE_SECTION_STRAIGHT_DIRECTION_TO_GO_SPEED;

        if(distance.computeDistance() > NEW_DASHED_LINE_SECTION_DISTANCE_TO_LAND){
            state = 1;
        }
    } else if (state == 1) {
        velocity = directionPidDriver.computeVelocity(NEW_DASHED_LINE_SECTION_STRAIGHT_DIRECTION_TO_LAND);        
    	velocity.mX = NEW_DASHED_LINE_SECTION_STRAIGHT_DIRECTION_TO_LAND_SPEED;
        countFlag(state1Flagcount, history.getLightSensorHistory() > WHITE_GRAY_THRESH);
        if(state1Flagcount > 1) {
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
};

#include "Calibrator.hpp"
#include <cmath>

void Calibrator::calibration(PidController &pid){
    // extern F32 K_PHIDOT, K_THETADOT;
    // S16 whiteGrayThresh;		/* 閾値の値 */
    // extern S16 BLACK_MEAN;	/* 黒の値 */
    // S8 touch_flag = WHITE_FLAG;	/* WHITE_FLAG = 1, BLACK_FLAG = 2 */
    // S8 flag_count = 0;

    // myLcd.clear(true);
    // myLcd.putf("sn","Read white.");
    // myLcd.disp();
	
    // while(true){
    //     if(myTouchL.isPressed() && touch_flag == WHITE_FLAG){
				
    //         /* 閾値を設定 */
    //         whiteGrayThresh += color.setWhiteGrayThresh((S16)mLight.get());
			
    //         flag_count++;
    //         if(flag_count > 10){
    //             whiteGrayThresh /= flag_count;
    //             color.setWhiteGrayThresh((S16)whiteGrayThresh);
				
    //             myLcd.clear(true);
    //             myLcd.putf("sn","Read Black.");
    //             myLcd.disp();
				
    //             systick_wait_ms(1000);	/* 1秒以上右ボタンを押すと次の状態に遷移するので注意 */
    //             flag_count = 0;
    //             touch_flag = BLACK_FLAG;
    //         }
    //     }else if(myTouchL.isPressed() && touch_flag == BLACK_FLAG){
				
    //         /* 黒の値を設定 */
    //         BLACK_MEAN += (S16)mLight.get();
			
    //         flag_count++;
    //         if(flag_count > 10){
    //             BLACK_MEAN /= flag_count;
    //             /* 設定値をLCDに表示 */
    //             displayParameter("KP",KP,true);
    //             displayParameter("KI",KI,false);
    //             displayParameter("KD",KD,false);
    //             displayParameter("K_PHIDOT",K_PHIDOT,false);
    //             displayParameter("K_THETADOT",K_THETADOT,false);
    //             displayParameter("SPEED",SPEED,false);
    //             displayParameter("THRESH",whiteGrayThresh,false);
    //             displayParameter("BLACK", BLACK_MEAN,false);
    //             pid.setKp(KP);
    //             pid.setKi(KI);
    //             pid.setKd(KD);
    //             pid.setSpeed(SPEED);
    //             systick_wait_ms(1000);	/* 1秒以上右ボタンを押すと次の状態に遷移するので注意 */
    //             break;
    //         }
    //     }
    // }
    // return;
	
}

void Calibrator::setParameters(PidController &pid){

    extern F32 K_PHIDOT, K_THETADOT;

    currentStatus = EXIT;
    myLcd.clear(true);
    myLcd.putf("sn","Set parameters.");
    myLcd.disp();

    while(true)
    {
        if(myTouchL.isPressed()){
            switch(currentStatus){
            case EXIT:
                displaySingleLine("set KP");
                currentStatus = SET_KP;
                break;
            case SET_KP:
                displaySingleLine("set KI");
                currentStatus = SET_KI;
                break;
            case SET_KI:
                displaySingleLine("set KD");
                currentStatus = SET_KD;
                break;
            case SET_KD:
                displaySingleLine("set speed");
                currentStatus = SET_SPEED;
                break;
            case SET_SPEED:
                displaySingleLine("switch edge");
                currentStatus = SET_EDGE;
                break;
            case SET_EDGE:
                displaySingleLine("set K_PHIDOT");
                currentStatus = SET_K_PHIDOT;
                break;
            case SET_K_PHIDOT:
                displaySingleLine("set K_THETADOT");
                currentStatus = SET_K_THETADOT;
                break;
            case SET_K_THETADOT:
                displaySingleLine("exit");
                currentStatus = EXIT;
                break;
            default:
                break;
            }
        }

        if(myTouchR.isPressed()){
            switch(currentStatus){
            case EXIT:
                displayParameter("KP",KP,true);
                displayParameter("KI",KI,false);
                displayParameter("KD",KD,false);
                displayParameter("K_PHIDOT",K_PHIDOT,false);
                displayParameter("K_THETADOT",K_THETADOT,false);
                displayParameter("SPEED",SPEED,false);
                if(TRACED_EDGE == LEFT)
                    displaySingleLine("left edge",false);
                else if(TRACED_EDGE == RIGHT)
                    displaySingleLine("right edge",false);
                pid.setKp(KP);
                pid.setKi(KI);
                pid.setKd(KD);
                //pid.setSpeed(SPEED);
                return;
                break;
            case SET_KP:
                chooseParameter("KP",KP);
                displaySingleLine("set KP");
                systick_wait_ms(waitingTime);
                break;
            case SET_KI:
                chooseParameter("KI",KI);
                displaySingleLine("set KI");
                systick_wait_ms(waitingTime);
                break;
            case SET_KD:
                chooseParameter("KD",KD);
                displaySingleLine("set KD");
                systick_wait_ms(waitingTime);
                break;
            case SET_SPEED:
//                chooseParameter("forwardSpeed",FORWARD_SPEED);
                chooseParameter("speed",SPEED);
                displaySingleLine("set speed");
                systick_wait_ms(waitingTime);
                break;
            case SET_EDGE:
                switchEdge();
                displaySingleLine("switch edge");
                systick_wait_ms(waitingTime);
                break;
            case SET_K_PHIDOT:
                chooseParameterCoarse("K_PHIDOT",K_PHIDOT);
                displaySingleLine("set K_PHIDOT");
                break;
            case SET_K_THETADOT:
                chooseParameterCoarse("K_THETADOT",K_THETADOT);
                displaySingleLine("set K_THETADOT");
                break;
            default:
                break;
            }
        }
        systick_wait_ms(200);
    }

};

void Calibrator::chooseParameter(char *name, S8 &param){
    displayParameter(name,param);
    systick_wait_ms(waitingTime);
    while(!(myTouchL.isPressed() && myTouchR.isPressed())){
        if(myTouchL.isPressed()){
            param -= 1;
            displayParameter(name, param);
        }
        if(myTouchR.isPressed()){
            param += 1;
            displayParameter(name, param);
        }

        systick_wait_ms(200);
    }

};

void Calibrator::chooseParameter(char *name, F32 &param){
    displayParameter(name,param);
    systick_wait_ms(waitingTime);
    while(!(myTouchL.isPressed() && myTouchR.isPressed())){
        if(myTouchL.isPressed()){
            param -= 0.1;
            displayParameter(name, param);
        }
        if(myTouchR.isPressed()){
            param += 0.1;
            displayParameter(name, param);
        }

        systick_wait_ms(200);
    }

};

void Calibrator::chooseParameterCoarse(char *name, F32 &param){
    displayParameter(name,param);
    systick_wait_ms(waitingTime);
    while(!(myTouchL.isPressed() && myTouchR.isPressed())){
        if(myTouchL.isPressed()){
            param -= 1.0;
            displayParameter(name, param);
        }
        if(myTouchR.isPressed()){
            param += 1.0;
            displayParameter(name, param);
        }

        systick_wait_ms(200);
    }

};

void Calibrator::displaySingleLine(char *line, bool clearDisplay){
    if(clearDisplay)
        myLcd.clear(clearDisplay);
    
    myLcd.putf("sn",line);
    myLcd.disp();
};

void Calibrator::displayParameter(char *name, S8 &param, bool clearDisplay){
    if(clearDisplay)
        myLcd.clear(clearDisplay);

    myLcd.putf("s",name);
    myLcd.putf("s",": ");
    myLcd.putf("dn",param,1);
    myLcd.disp();
};

void Calibrator::displayParameter(char *name, S16 &param, bool clearDisplay){
    if(clearDisplay)
        myLcd.clear(clearDisplay);

    myLcd.putf("s",name);
    myLcd.putf("s",": ");
    myLcd.putf("dn",param,1);
    myLcd.disp();
};

void Calibrator::displayParameter(char *name, F32 &param, bool clearDisplay){
    S8 FirstDigit, SecondDigit;

    FirstDigit = (S16) param;
    SecondDigit = (S16) (param*10)%10;

    if(clearDisplay)
        myLcd.clear(clearDisplay);

    myLcd.putf("s",name);
    myLcd.putf("s",": ");
    if(param < 0){
        myLcd.putf("s","-");
        FirstDigit *= -1;
        SecondDigit *= -1;
    }
    else{
        myLcd.putf("s"," ");
    }

    myLcd.putf("d",FirstDigit,1);
    myLcd.putf("s",".");
    myLcd.putf("dn",SecondDigit,1);
    myLcd.disp();
};

void Calibrator::switchEdge(){

    while(!(myTouchL.isPressed() && myTouchR.isPressed())){
        if(myTouchR.isPressed()){
            if (TRACED_EDGE == LEFT){
                TRACED_EDGE = RIGHT;
                displaySingleLine("right edge");
            } else {
                TRACED_EDGE = LEFT;
                displaySingleLine("left edge");
            }
        }
        systick_wait_ms(200);
    }
};


#ifndef CALIBRATOR_H_
#define CALIBRATOR_H_

#include <stddef.h>


#include "Lcd.h"
#include "TouchSensor.h"
#include "parameters.hpp"
#include "light_param.h"
#include "LightSensor.h"

using namespace ecrobot;
#include "PidController.hpp"

/** パラメータをセットするためのUIクラス。
 */
class Calibrator
{

private:
    Lcd &myLcd;///< 各種メニュー画面を表示するためのLcdクラス。
    const TouchSensor &myTouchL,///< 左タッチセンサをコントロールするクラス。
        &myTouchR;///< 右タッチセンサをコントロールするクラス。

    void chooseParameter(char *name, S8 &param);
    void chooseParameter(char *name, F32 &param);
    void chooseParameterCoarse(char *name, F32 &param);
    void displaySingleLine(char *line, bool clearDisplay = true);
    void displayParameter(char *name, S8 &param, bool clearDisplay = true);
	void displayParameter(char *name, S16 &param, bool clearDisplay = true);
    void displayParameter(char *name, F32 &param, bool clearDisplay = true);
    void switchEdge();
    static const U32 waitingTime = 1000;// ms
	LightSensor& mLight;
	Color &color;
	static const S8 WHITE_FLAG = 1;
	static const S8 BLACK_FLAG = 2;

	

public:
    Calibrator(Lcd &lcd,
               const TouchSensor &touchL,
               const TouchSensor &touchR,
    		   LightSensor& light,
    		   Color& c) :
        myLcd(lcd),
        myTouchL(touchL),
        myTouchR(touchR),
		mLight(light),
		color(c)
        {};
    ~Calibrator(){};

    /** 設定しているパラメータを表すenum。
     */
    enum Status{
        EXIT,///< 終了。
        SET_KP,///< kpを設定。
        SET_KI,///< kiを設定。
        SET_KD,///< kdを設定。
        SET_K_PHIDOT,
        SET_K_THETADOT,
        SET_SPEED,///< 前進スピードを設定。
        SET_EDGE///< トレースするエッジを設定。
    };

    enum Status currentStatus;///< 現在設定しているパラメータ。
	
    void setParameters(PidController &pid);
	void calibration(PidController &pid);
	
};

#endif

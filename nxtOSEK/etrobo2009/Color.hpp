#ifndef _COLOR_H_
#define _COLOR_H_

#include "SensorHistory.hpp"
#include "parameters.hpp"

class Color
{
private:
    SensorHistory &history;
    S16 whiteGrayThresh, blackMean;

public:
    Color(SensorHistory &sh) :
        history(sh)
        {
            whiteGrayThresh = WHITE_GRAY_THRESH;///<パラメータの調整をする必要がある。
            blackMean = BLACK_MEAN;///<パラメータの調整をする必要がある。
        };
    
    ~Color(){};

    S16 getLightSensorValue(){return history.getLightSensorHistory(0);};

    S16 getDeviation(){return getLightSensorValue() - whiteGrayThresh;};
	
    S16 setWhiteGrayThresh(S16 mLightGet){return whiteGrayThresh = mLightGet + 40;}

};


#endif /* _COLOR_H_ */

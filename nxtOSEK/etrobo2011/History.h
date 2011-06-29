///////////////////////////////////////////////////////////
//  History.h
//  Implementation of the Class Histrory
//  Created on:      10-9-2010 19:42:49
//  Original author: yhayama
///////////////////////////////////////////////////////////

#if !defined HISTORY_H
#define HISTORY_H

#define LATEST 0
#define OLDEST 1
#define PREVIOUS -1

/**
 * Historyクラス
 */
class History
{
private :
    float *mBuffer; //!< バッファとして使う配列を指すポインタ
    int mSize; //!< バッファサイズ
    int mIndex; //!< バッファの現在インデックス
public :
    History(float* buffer, int size);
    virtual ~History();
    int getSize();
    float calcAverage();
    float calcDifference();
    float get(int time = 0);
    void update(float data);
};
#endif // HISTORY_H

//
// LineDetector.h
//

#ifndef LineDetector_H_
#define LineDetector_H_

#include "Detector.h"

/**
 * ライン検知
 *
 * ライン復帰に使う
 */
class LineDetector : public Detector
{
public:
    /**
     * ライン検知のパターンを示す
     */
    enum ePattern {
        ON_WHITE,       //!< 白=>白(ライン外)
        WHITE_TO_BLACK, //!< 白=>黒
        BLACK_TO_WHITE, //!< 黒=>白
        ON_BLACK,       //!< 黒=>黒
    };
public:
    LineDetector(){}
    ~LineDetector(){}
    LineDetector::ePattern detect();
};

#endif

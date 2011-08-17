#ifndef __COORDINATES_H__
#define __COORDINATES_H__

#include "struct.h"

extern float GPS_COURSE_START_X;
extern float GPS_COURSE_START_Y;
extern float GPS_COURSE_START_DIRECTION;

extern float GPS_ENIGMA_START_X;
extern float GPS_ENIGMA_START_Y;
extern float GPS_ENIGMA_START_DIRECTION;

extern float GPS_MYSTERY_START_X;
extern float GPS_MYSTERY_START_Y;
extern float GPS_MYSTERY_START_DIRECTION;

extern float GPS_SEESAW_START_X;
extern float GPS_SEESAW_START_Y;
extern float GPS_SEESAW_START_DIRECTION;

extern float GPS_STAIRWAY_START_X;
extern float GPS_STAIRWAY_START_Y;
extern float GPS_STAIRWAY_START_DIRECTION;

/* ETロボコン2011 追記*/
// ET相撲開始座標
extern float GPS_ETSUMO_START_X; // ＠todo要再設定
extern float GPS_ETSUMO_START_Y; // ＠todo要再設定
extern float GPS_ETSUMO_START_DIRECTION; // ＠todo要再設定
extern float GPS_ETSUMO_SEARCH_X; // ＠todo要再設定
extern float GPS_ETSUMO_SEARCH_Y; // ＠todo要再設定
extern float GPS_ETSUMO_SEARCH_DIRECTION; // ＠todo要再設定
/* ETロボコン2011 追記ここまで */

extern float GPS_GARAGEIN_START_X;
extern float GPS_GARAGEIN_START_Y;
extern float GPS_GARAGEIN_START_DIRECTION;

extern float GPS_GARAGE_X;
extern float GPS_GARAGE_Y;
extern float GPS_GARAGE_DIRECTION;

extern Region GPS_ENIGMA_START;
extern Region GPS_MYSTERY_START;
extern Region GPS_SEESAW_START;
extern Region GPS_STAIRWAY_START;
extern Region GPS_GARAGEIN_START;

#endif // __COORDINATES_H__

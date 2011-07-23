#include "constants.h"
#include "macro.h"
#include <math.h>

#define KACHIROBO
//#define KACHIICHI
//#define KACHINEW

#ifdef KACHIROBO
char BT_NAME[16] = "KACHIROBO";
int USER_GYRO_OFFSET = 605; // この値は適宜調整すること
//竹田家 W:550 G:617 B:720
int WHITE = 550; // 白色の光センサ値 //小さめにしてね
int BLACK = 720; // 黒色の光センサ値 //大きめにしてね
int LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK -  TRIPOD_WHITE)/4;
int TRIPOD_WHITE = 607; // 白色の光センサ値 //小さめにしてね
int TRIPOD_BLACK = 710; // 黒色の光センサ値 //大きめにしてね
int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
#endif

#ifdef KACHIICHI
char BT_NAME[16] = "KACHIICHI";
int USER_GYRO_OFFSET = 585; // この値は適宜調整すること
//W:570 B:670
int WHITE = 560; // 白色の光センサ値 //小さめにしてね
int BLACK = 680; // 黒色の光センサ値 //大きめにしてね
int LINE_THRESHOLD = 600; // ライン端の色を示す閾値。白と灰色の間の値
#endif

#ifdef KACHINEW
char BT_NAME[16] = "KACHINEW";
int USER_GYRO_OFFSET = 608; // この値は適宜調整すること
//3F居室
int WHITE = 540; // 白色の光センサ値
int BLACK = 730; // 黒色の光センサ値
int LINE_THRESHOLD = 600; // ライン端の色を示す閾値。白と灰色の間の値
#endif

// GPS
// float WHEEL_RADIUS = 38.5; // 車輪半径(mm)
// float WHEEL_DISTANCE = 168.0; // 車輪間隔(mm)
float WHEEL_RADIUS = 40.5; // 車輪半径(mm)
float WHEEL_DISTANCE = 162.0; // 車輪間隔(mm)

// LineTrace
int INIT_FORWARD = 50;    // 初期前進速度
int INIT_SAMPLECOUNT = 200;  // 初期前進継続カウンタ期間
int FORWARD = 100;        // 前進速度
float LIGHT_PID_KP = 150; // K_PHIDOT = 25.0F
float LIGHT_PID_KI = 0;   // K_PHIDOT = 25.0F
float LIGHT_PID_KD = 300; // K_PHIDOT = 25.0F
float LIGHT_ONOFF_K = 40; // K_PHIDOT = 25.0F
// float LIGHT_PID_KP = 40; // K_PHIDOT = 25.0F*2.5F
// float LIGHT_PID_KI = 0;   // K_PHIDOT = 25.0F*2.5F
// float LIGHT_PID_KD = 120; // K_PHIDOT = 25.0F*2.5F

//AngleTrace
int TARGETANGLE = 180;//(360*2);  //目標角度
float ANGLE_PID_KP = 10.0; // K_PHIDOT = 25.0F
float ANGLE_PID_KI = 0.0;  // K_PHIDOT = 25.0F
float ANGLE_PID_KD = 30.0; // K_PHIDOT = 25.0F
// float ANGLE_PID_KP = 4.0; // K_PHIDOT = 25.0F*2.5F
// float ANGLE_PID_KI = 0.0;  // K_PHIDOT = 25.0F*2.5F
// float ANGLE_PID_KD = 12.0; // K_PHIDOT = 25.0F*2.5F

//SlowdownSkill
float SLOWDOWN_PID_KP = 1.0; // 100mm手前でフォワード値100
float SLOWDOWN_PID_KI = 0;
float SLOWDOWN_PID_KD = 0;

//CoordinateStop
float STOP_PID_KP = 0.01; 
float STOP_PID_KI = 0.0001;
float STOP_PID_KD = 0.001;
//float STOP_PID_KP = 0.0625;
//float STOP_PID_KI = 0.001;
//float STOP_PID_KD = 0.015;

//転倒検出
int FAILSAFE_SAMPLECOUNT = 200; // 転倒検出のサンプル数とする(*4ms)
int FAILSAFE_THRESHOLD = 600; //FAILSAFE_SAMPLECOUNT当たり両輪がこれ以上回転したら転倒とみなす

//検知系
float WALL_THRESHOLD = -10;
float DOWNSLOPE_THRESHOLD = 5;

//Bluetoothログ送信
int LOGGER_SEND = 0;
//0: 送らない
//1: void send(S8 dataS08[2], S32 dataS32)
//2: void send(S8 dataS08[2], U16 dataU16, S16 dataS16[4], S32 dataS32[4])
S8  LOGGER_DATAS08[2] = {0, 0};
U16 LOGGER_DATAU16 = 0;
S16 LOGGER_DATAS16[4] = {0,0,0,0};
S32 LOGGER_DATAS32[4] = {0,0,0,0};

//Activator
bool DESK_DEBUG = false; // モータを回さない

//LineTraceフラグ
bool gLineTrace = false;
bool gTripodLineTrace = false;
//ブレーキ走行(実験中)
int PWMR = 0;
int PWML = 0;

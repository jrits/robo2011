#include "define.h"
#include "constants.h"
#include "macro.h"
#include <math.h>

#ifdef KACHIROBO
char BT_NAME[16] = "KACHIROBO";
int USER_GYRO_OFFSET = 600; // この値は適宜調整すること
// オージス総研
int WHITE = 530; // 白色の光センサ値 //小さめにしてね
int BLACK = 670; // 黒色の光センサ値 //大きめにしてね
int LINE_THRESHOLD = WHITE + (BLACK - WHITE)/4; // 白と黒の間からちょっと白より
int GRAY  = (WHITE + BLACK)/2; // 灰色の光センサ値
int MARKER_THRESHOLD = GRAY + (BLACK - GRAY)/4; // 灰と黒の間からちょっと灰より
int TRIPOD_WHITE = 560; // 白色の光センサ値 //小さめにしてね
int TRIPOD_BLACK = 650; // 黒色の光センサ値 //大きめにしてね
int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
int TRIPOD_GRAY  = (TRIPOD_WHITE + TRIPOD_BLACK)/2; // 灰色の光センサ値
int TRIPOD_MARKER_THRESHOLD = TRIPOD_GRAY + (TRIPOD_BLACK - TRIPOD_GRAY)/4; // 灰と黒の間からちょっと灰より
// //竹田家 W:550 G:617 B:720
// int WHITE = 550; // 白色の光センサ値 //小さめにしてね
// int BLACK = 720; // 黒色の光センサ値 //大きめにしてね
// int LINE_THRESHOLD = WHITE + (BLACK - WHITE)/4; // 白と黒の間からちょっと白より
// int GRAY  = (WHITE + BLACK)/2; // 灰色の光センサ値
// int MARKER_THRESHOLD = GRAY + (BLACK - GRAY)/4; // 灰と黒の間からちょっと灰より
// int TRIPOD_WHITE = 607; // 白色の光センサ値 //小さめにしてね
// int TRIPOD_BLACK = 710; // 黒色の光センサ値 //大きめにしてね
// int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
// int TRIPOD_GRAY  = (TRIPOD_WHITE + TRIPOD_BLACK)/2; // 灰色の光センサ値
// int TRIPOD_MARKER_THRESHOLD = TRIPOD_GRAY + (TRIPOD_BLACK - TRIPOD_GRAY)/4; // 灰と黒の間からちょっと灰より
#endif

#ifdef KACHIICHI
char BT_NAME[16] = "KACHIICHI";
int USER_GYRO_OFFSET = 611; // この値は適宜調整すること

#if 1 //オージス
int WHITE = 513; // 白色の光センサ値 //小さめにしてね
int BLACK = 665; // 黒色の光センサ値 //大きめにしてね
int LINE_THRESHOLD = WHITE + (BLACK - WHITE)/4;
int GRAY  = (WHITE + BLACK)/2; // 灰色の光センサ値
int MARKER_THRESHOLD = GRAY + (BLACK - GRAY)/4; // 灰と黒の間からちょっと灰より
int TRIPOD_WHITE = 562; // 白色の光センサ値 //小さめにしてね
int TRIPOD_BLACK = 672; // 黒色の光センサ値 //大きめにしてね
int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
int TRIPOD_GRAY  = (TRIPOD_WHITE + TRIPOD_BLACK)/2; // 灰色の光センサ値
int TRIPOD_MARKER_THRESHOLD = TRIPOD_GRAY + (TRIPOD_BLACK - TRIPOD_GRAY)/4; // 灰と黒の間からちょっと灰より#endif //オージス
#endif

#if 0 //高橋家
int WHITE = 540; // 白色の光センサ値 //小さめにしてね
int BLACK = 710; // 黒色の光センサ値 //大きめにしてね
int LINE_THRESHOLD = WHITE + (BLACK - WHITE)/4;
int GRAY  = (WHITE + BLACK)/2; // 灰色の光センサ値
int MARKER_THRESHOLD = GRAY + (BLACK - GRAY)/4; // 灰と黒の間からちょっと灰より
int TRIPOD_WHITE = 562; // 白色の光センサ値 //小さめにしてね
int TRIPOD_BLACK = 672; // 黒色の光センサ値 //大きめにしてね
int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
int TRIPOD_GRAY  = (TRIPOD_WHITE + TRIPOD_BLACK)/2; // 灰色の光センサ値
int TRIPOD_MARKER_THRESHOLD = TRIPOD_GRAY + (TRIPOD_BLACK - TRIPOD_GRAY)/4; // 灰と黒の間からちょっと灰より
#endif //高橋家

// 瀬尾家
// int WHITE = 500; // 白色の光センサ値 //小さめにしてね
// int BLACK = 710; // 黒色の光センサ値 //大きめにしてね
// int LINE_THRESHOLD = WHITE + (BLACK - WHITE)/4; // 白と黒の間からちょっと白より
// int GRAY  = (WHITE + BLACK)/2; // 灰色の光センサ値
// int MARKER_THRESHOLD = GRAY + (BLACK - GRAY)/4; // 灰と黒の間からちょっと灰より
// int TRIPOD_WHITE = 580; // 白色の光センサ値 //小さめにしてね
// int TRIPOD_BLACK = 710; // 黒色の光センサ値 //大きめにしてね
// int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
// int TRIPOD_GRAY  = (TRIPOD_WHITE + TRIPOD_BLACK)/2; // 灰色の光センサ値
// int TRIPOD_MARKER_THRESHOLD = TRIPOD_GRAY + (TRIPOD_BLACK - TRIPOD_GRAY)/4; // 灰と黒の間からちょっと灰より
#endif

#ifdef KACHINEW
char BT_NAME[16] = "KACHINEW";
int USER_GYRO_OFFSET = 582; // この値は適宜調整すること
// オージス総研
int WHITE = 530; // 白色の光センサ値 //小さめにしてね
int BLACK = 670; // 黒色の光センサ値 //大きめにしてね
int LINE_THRESHOLD = WHITE + (BLACK - WHITE)/4; // 白と黒の間からちょっと白より
int GRAY  = (WHITE + BLACK)/2; // 灰色の光センサ値
int MARKER_THRESHOLD = GRAY + (BLACK - GRAY)/4; // 灰と黒の間からちょっと灰より
int TRIPOD_WHITE = 560; // 白色の光センサ値 //小さめにしてね
int TRIPOD_BLACK = 650; // 黒色の光センサ値 //大きめにしてね
int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
int TRIPOD_GRAY  = (TRIPOD_WHITE + TRIPOD_BLACK)/2; // 灰色の光センサ値
int TRIPOD_MARKER_THRESHOLD = TRIPOD_GRAY + (TRIPOD_BLACK - TRIPOD_GRAY)/4; // 灰と黒の間からちょっと灰より
// //竹田家 W:550 G:617 B:720
// int WHITE = 550; // 白色の光センサ値 //小さめにしてね
// int BLACK = 720; // 黒色の光センサ値 //大きめにしてね
// int LINE_THRESHOLD = WHITE + (BLACK - WHITE)/4; // 白と黒の間からちょっと白より
// int GRAY  = (WHITE + BLACK)/2; // 灰色の光センサ値
// int MARKER_THRESHOLD = GRAY + (BLACK - GRAY)/4; // 灰と黒の間からちょっと灰より
// int TRIPOD_WHITE = 607; // 白色の光センサ値 //小さめにしてね
// int TRIPOD_BLACK = 710; // 黒色の光センサ値 //大きめにしてね
// int TRIPOD_LINE_THRESHOLD = TRIPOD_WHITE + (TRIPOD_BLACK - TRIPOD_WHITE)/4;
// int TRIPOD_GRAY  = (TRIPOD_WHITE + TRIPOD_BLACK)/2; // 灰色の光センサ値
// int TRIPOD_MARKER_THRESHOLD = TRIPOD_GRAY + (TRIPOD_BLACK - TRIPOD_GRAY)/4; // 灰と黒の間からちょっと灰より
#endif

// まいまい式
// 新横浜
//float MAIMAI_WHITE = 1.0F; // 白色のMAIMAIセンサ値 //大きめにしてね
//float MAIMAI_BLACK = 0.4F; // 黒色のMAIMAIセンサ値 //小さめにしてね
// ogis
float MAIMAI_WHITE = 0.8F; // 白色のMAIMAIセンサ値 //大きめにしてね
float MAIMAI_BLACK = 0.2F; // 黒色のMAIMAIセンサ値 //小さめにしてね
float MAIMAI_LINE_THRESHOLD = MAIMAI_WHITE - (MAIMAI_WHITE - MAIMAI_BLACK)/4.0; // ライン端の色を示す閾値。白と灰の間の値
float MAIMAI_GRAY  = (MAIMAI_WHITE + MAIMAI_BLACK)/2.0; // 灰色の光センサ値
float MAIMAI_MARKER_THRESHOLD = (MAIMAI_GRAY + MAIMAI_BLACK)/2.0; // 灰と黒の間の値
#if 0 // 瀬尾家
float MAIMAI_TRIPOD_WHITE = 0.75F; // 白色のMAIMAIセンサ値 //大きめにしてね
float MAIMAI_TRIPOD_BLACK = 0.30F; // 黒色のMAIMAIセンサ値 //小さめにしてね
#endif
#if 1 // 新横浜
float MAIMAI_TRIPOD_WHITE = 0.5F; // 白色のMAIMAIセンサ値 //大きめにしてね
float MAIMAI_TRIPOD_BLACK = 0.2F; // 黒色のMAIMAIセンサ値 //小さめにしてね
#endif
float MAIMAI_TRIPOD_LINE_THRESHOLD = MAIMAI_TRIPOD_WHITE - (MAIMAI_TRIPOD_WHITE - MAIMAI_TRIPOD_BLACK)/4.0; // ライン端の色を示す閾値。白と灰色の間の値
float MAIMAI_TRIPOD_GRAY  = (MAIMAI_TRIPOD_WHITE + MAIMAI_TRIPOD_BLACK)/2.0; // 灰色の光センサ値
float MAIMAI_TRIPOD_MARKER_THRESHOLD = (MAIMAI_TRIPOD_GRAY + MAIMAI_TRIPOD_BLACK)/2.0; // 灰と黒の間の値
int   MAIMAI_PERIOD = 10; /* まいまい式の実行周期(ms)。8msでもイケる？*/

// GPS
// float WHEEL_RADIUS = 38.5; // 車輪半径(mm)
// float WHEEL_DISTANCE = 168.0; // 車輪間隔(mm)
float WHEEL_RADIUS = 40.5; // 車輪半径(mm)
float WHEEL_DISTANCE = 162.0; // 車輪間隔(mm)

// LineTrace
int INIT_FORWARD = 50;    // 初期前進速度
int INIT_SAMPLECOUNT = 200;  // 初期前進継続カウンタ期間
int FORWARD = 100;        // 前進速度
float LIGHT_PID_KP = 60; // K_PHIDOT = 25.0F
float LIGHT_PID_KI = 0;   // K_PHIDOT = 25.0F
float LIGHT_PID_KD = 180; // K_PHIDOT = 25.0F
float LIGHT_ONOFF_TURN = 80; // ONOFF制御の旋回量
float LIGHT_ONOFF_K_PHIDOT = 25.0F*2.5F;
float LIGHT_ONOFF_K_THETADOT = 7.5F;

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

//tail_control
int   TAIL_ANGLE_STAND_UP = 108; /* 完全停止時の角度[度] */
int   TAIL_ANGLE_DRIVE = 3; /* バランス走行時の角度[度] */
int   TAIL_ANGLE_TRIPOD_DRIVE = 70; /* ３点走行時の角度[度] */
float TAIL_P_GAIN = 2.5F; /* 完全停止用モータ制御比例係数 */
int   TAIL_PWM_ABS_MAX = 60; /* 完全停止用モータ制御PWM絶対最大値 */

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


/**
 *******************************************************************************
 **	FILE NAME : balancer_param.c
 **
 **	ABSTRUCT  : NXTway-GS balance control parameters
 **
 ** NOTE: These parameters definitely affect the balance control of NXTway-GS.
 *******************************************************************************
 **/

#include "ecrobot_interface.h"

/*============================================================================
 * DATA DEFINITIONS
 *===========================================================================*/
/* low pass filter gain for motors average count */
F32 A_D = 0.8F; //0.8F
/* low pass filter gain for motors target count */
F32 A_R = 0.996F;      

/* 
 * NOTE: When NXT standard tires are used for NXTway-GS, a compiler macro (NXT_STD_TIRE)
 *       has to be added to USER_DEF macro definition in user Makefile
 *       E.g. USER_DEF = NXT_STD_TIRE
 */
/* #ifdef NXT_STD_TIRE */
/* 	/\* servo control state feedback gain for NXT standard tire *\/ */
/* 	/\* F32 K_F[4] = {-0.834434F, -38.1749F, -1.0985F, -3.55477F}; *\/ */
/*         F32 K_F[4] = {   -0.843338F, -39.3947F, -1.1126F, -4.11782F}; */
/* #else */
/* 	/\* servo control state feedback gain for RCX Motorcycle tire *\/ */
/* 	F32 K_F[4] = {-0.870303F, -31.9978F, -1.1566F, -2.78873F}; */
/* #endif */

/* 
 * 他のパラメータはnxtway_gs/param_plant内で定義されている。
 * RRはファイルnxtway_gs/param_controller.mの中で定義されている。
 */
/* m = 0.03;                        % wheel weight [kg]     */
/* R = 0.0408;                      % wheel radius [m] 0.04 */
/* M = 0.67;                        % body weight [kg] 0.6  */
/* W = 0.065;                       % body width [m] 0.14   */
/* D = 0.047;                       % body depth [m] 0.04   */
/* H = 0.18;                        % body height [m] 0.144 */

/* RR = 1.5e2*eye(2);                                       */
/* F32 K_F[4] = {-1.6704, -62.2796, -1.4630, -4.7635};      */
/* F32 K_I = -1.1547;                                       */

/* RR = 1e2*eye(2);                                         */
/* F32 K_F[4] = {-1.9816, -72.5038, -1.6158, -5.2759};      */
/* F32 K_I = -1.4142;                                       */

/* RR = 1e3*eye(2);                                         */
/* F32 K_F[4] = {-0.83830, -37.59739, -1.10463, -3.55765};  */
/* F32 K_I = -0.44721;                                      */

/* RR = 1e4*eye(2);                                         */
/* F32 KF[4] = {-0.42564, -30.48672, -0.98157, -3.15133};   */
/* F32 K_I = -0.14142;                                      */

/* RR = 1e5*eye(2);                                         */
/* F32 KF = {-0.22825F, -28.78509F, -0.93233F, -2.99480F};  */
/* F32 K_I = -0.44721;                                      */

/* 電池用 M = 0.7                                           */
/* RR = 1e3*eye(2);                                         */
F32 K_F[4] ={-0.83830,  -37.74796,   -1.10463,   -3.56874};
F32 K_I = -0.44721;

/* 充電バッテリー用                                         */

//{-0.83986F, -39.15738F, -1.10712F, -3.96924F};
/* m = 0.03;                        % wheel weight [kg]     */
/* R = 0.0408;                      % wheel radius [m] 0.04 */
/* M = 0.6;                        % body weight [kg] 0.6   */
/* W = 0.065;                       % body width [m] 0.14   */
/* D = 0.047;                       % body depth [m] 0.04   */
/* H = 0.18;                        % body height [m] 0.144 */

F32 K_PHIDOT = 30.0F;//30.0  /* turn target speed gain */
F32 K_THETADOT = 10.0F;//7.5F /* forward target speed gain */

const F32 BATTERY_GAIN = 0.001089F;	/* battery voltage gain for motor PWM outputs */
const F32 BATTERY_OFFSET = 0.625F;	/* battery voltage offset for motor PWM outputs */
/******************************** END OF FILE ********************************/

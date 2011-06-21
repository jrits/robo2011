#include "PidController.hpp"
#include "parameters.hpp"
#include <cmath>

float PidController::control(float current, float reference)
{
    static float buf[MAX_SIZE] = {0};
    static unsigned int index = 0;
    static float P, I = 0, D, PP = 0;

    // Proportional
    P = current - reference;

    // Nothing but buffering (Ring bufffer)
    PP = buf[index]; // oldest value
    buf[index] = P;  // newest value
    index = (index + 1) % size;
    
    // Integral
    //I += P; // バッファオーバフローの危険性
    I = 0;
    for (int i = 0; i < size; i++) {
        I += buf[i];
    }

    //Derivative
    D = P - PP;

    float Y = ((kp*P) + (ki*I) + (kd*D));
#if PRINT_PID_CONTROLLER
    Lcd lcd;
    lcd.clear();
    lcd.putf("sn", "pid");
    lcd.putf("dn", (S32)(kp * 1000));
    lcd.putf("dn", (S32)(ki * 1000));
    lcd.putf("dn", (S32)(kd * 1000));
    lcd.putf("dn", (S32)(kp*P * 1000));
    lcd.putf("dn", (S32)(ki*I * 1000));
    lcd.putf("dn", (S32)(kd*D * 1000));
    lcd.putf("dn", (S32)(Y * 1000));
    lcd.disp();
#endif
    return Y;
};


#include "Distance.hpp"
#include <cmath>

void Distance::reset()
{
    S32 currentCountL = history.getLeftMotorCountHistory();
    S32 currentCountR = history.getRightMotorCountHistory();
    this->resetCount = (currentCountL + currentCountR) / 2.0;
}

F32 Distance::computeDistance()
{
    S32 currentCountL = history.getLeftMotorCountHistory();
    S32 currentCountR = history.getRightMotorCountHistory();
    F32 currentCount;
    F32 distance;

    currentCount = (currentCountL + currentCountR) / 2.0;
    distance = (currentCount - this->resetCount) * WHEEL_RADIUS * M_PI / 180.0;

    return distance;
}

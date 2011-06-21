#include "Balancer.hpp"

void Balancer::balance(VectorT<S8> &direction, S8 &pwmL, S8 &pwmR){

    balance_control(
  		  (F32)direction.mX, // forward command
  		  (F32)direction.mY, // turn command
  		  (F32)history.getGyroSensorHistory(),
  		  (F32)USER_GYRO_OFFSET,
  		  (F32)history.getLeftMotorCountHistory(),
  		  (F32)history.getRightMotorCountHistory(),
  		  (F32)history.getBatteryVoltageHistory(),
  		  &pwmL,
  		  &pwmR);
};

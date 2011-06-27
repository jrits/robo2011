#include "Activator.h"

Activator::Activator(MotorIF &leftMotor, 
                     MotorIF &rightMotor, 
                     GyroSensorIF &gyroSensor, 
                     NxtIF &nxt)
    : mLeftMotor(leftMotor),
      mRightMotor(rightMotor),
      mGyroSensor(gyroSensor),
      mNxt(nxt) {
}

void Activator::run(VectorT<F32> command){
}

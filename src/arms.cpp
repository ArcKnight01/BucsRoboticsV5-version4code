#include "arms.h"

Arms::Arms(){
  armSpeed = 80;
}

Arms::Arms(motor_group _armMotors, double _armSpeed){
  armMotors = _armMotors;
  armSpeed = _armSpeed;
}


void Arms::raise(){
  armMotors.spin(directionType::fwd, armSpeed, velocityUnits::rpm);
}

void Arms::raise(double speed){
  armMotors.spin(directionType::fwd, speed, velocityUnits::rpm);
}

void Arms::lower(double speed){
  armMotors.spin(directionType::fwd, speed, velocityUnits::rpm);
}

void Arms::lower(){
  armMotors.spin(directionType::fwd, armSpeed, velocityUnits::rpm);
}

void Arms::stop(){
  armMotors.stop(brakeType::hold);
}

void Arms::rotateDegree(double rotationAmount, double speed){
    ArmMotors.rotateFor(rotationAmount, rotationUnits::rev, speed, velocityUnits::rpm);
    stop();
}
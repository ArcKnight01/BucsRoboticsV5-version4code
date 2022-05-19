
#include "arms.h"

#include "vex.h"
using namespace vex;

Arms::Arms(){
  armSpeed = 80;
}

Arms::Arms(double _armSpeed){
  armSpeed = _armSpeed;
}


void Arms::raise(){
  ArmMotors.spin(directionType::fwd, armSpeed, velocityUnits::rpm);
}

void Arms::raise(double speed){
  ArmMotors.spin(directionType::fwd, speed, velocityUnits::rpm);
}

void Arms::lower(double speed){
  ArmMotors.spin(directionType::fwd, speed, velocityUnits::rpm);
}

void Arms::lower(){
  ArmMotors.spin(directionType::fwd, armSpeed, velocityUnits::rpm);
}

void Arms::stop(){
  ArmMotors.stop(brakeType::hold);
}

void Arms::rotateDegree(double rotationAmount, double speed){
    ArmMotors.rotateFor(rotationAmount, rotationUnits::rev, speed, velocityUnits::rpm);
    stop();
}
#include "claw.h"
#include "vex.h"
using namespace vex;

Claw::Claw(){
  initialize();
}

void Claw::initialize(){
  
    ClawMotor.resetPosition();
    ClawMotor.resetRotation();
    ClawMotor.setVelocity(100, percent);
    ClawMotor.setTimeout(5, timeUnits::sec);
    ClawMotor.setBrake(brakeType::hold);

}
void Claw::unclamp(double rotationDegrees){
    rotationDegrees *= gearRatio;
    if (clawState != opened && clawState == closed) {
        wait(5,msec);
        ClawMotor.setTimeout(5, seconds);
        ClawMotor.spinFor(directionType::rev, rotationDegrees, rotationUnits::deg); 
        ClawMotor.setBrake(brakeType::hold);
    }
    else if (clawState == opened){
      Controller1.rumble("..");
    }
    clawState = opened;
}

void Claw::clamp(double rotationDegrees){
  rotationDegrees *= gearRatio;
  if(clawState!=closed && clawState == opened){
    wait(5, msec);
    ClawMotor.setTimeout(5, seconds);
    ClawMotor.spinFor(directionType::fwd, rotationDegrees, degrees); 
    ClawMotor.setBrake(brakeType::hold);
  }
  else if (clawState == closed){
      Controller1.rumble("..");
    }
  clawState = closed;
}


void Claw::changeState(){
  switch(clawState){
      case opened:
        clamp(360);
        
        break;

      case closed:
        unclamp(360);
        
        break;
    }
}

CLAWSTATE Claw::getMode(){
  return clawState;
}
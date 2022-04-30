#include "claw.h"

Claw::Claw(){
  initialize();
}

Claw::Claw(motor _clawMotor){
  clawMotor = _clawMotor;
  initialize();
  
}

void Claw::initialize(){
  if(clawMotor.installed()){
    installed = true;
  }
  if(installed){
    clawMotor.resetPosition();
    clawMotor.resetRotation();
    clawMotor.setVelocity(100, percent);
    clawMotor.setTimeout(5, timeUnits::sec);
    clawMotor.setBrake(brakeType::hold);
  }
  

}
void Claw::unclamp(){
    double rotationDegrees = 360;
    if (clawState == opened) {
      //claw is opened already
    } else if (clawState == closed) {
      //claw is currently closed
      if(installed){
        clawMotor.setTimeout(3000, timeUnits::msec);
        clawMotor.spinFor(directionType::rev, rotationDegrees, degrees); 
        clawMotor.setBrake(brakeType::hold);
      }
     }
}

void Claw::clamp(){
  double rotationDegrees = 360;
  if(clawState == opened){
    //claw is opem
  }
  else if(clawState == closed){
    if(installed){
      clawMotor.setTimeout(3000, timeUnits::msec);
      clawMotor.spinFor(directionType::rev, rotationDegrees, degrees); 
      clawMotor.setBrake(brakeType::hold);
    }
  }
}


void Claw::changeState(){
  switch(clawState){
      case opened:
        clamp();
        clawState = closed;
        break;

      case closed:
        unclamp();
        clawState = opened;
        break;
    }
}
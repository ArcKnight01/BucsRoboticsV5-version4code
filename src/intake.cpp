#include "intake.h"

Intake::Intake(){
  if(intakeMotor.installed()){
    installed = true;
  }
}

Intake::Intake(motor _intakeMotor, double _intakeSpeed){
  intakeSpeed = _intakeSpeed;
  if(IntakeMotor.installed()){
    installed = true;
  }
}

void Intake::setSpeed(double speed){
  if(installed)
    IntakeMotor.spin(directionType::fwd, speed, velocityUnits::rpm);
  
}

void Intake::setSpeed(double speed, directionType direction){
  if(installed)
    IntakeMotor.spin(direction, speed, velocityUnits::rpm);
}

int Intake::getMode(){
  return(intakeMode);
}

double Intake::getIntakeToDriveRatio(){
  return(intakeToDriveRatio);
}
void Intake::start(){
  if(intakeMode == automatic) {
      setSpeed(intakeSpeed);
  }
}

void Intake::reverse(){
  if(intakeMode == automatic) {
     setSpeed(intakeSpeed, directionType::rev);   
  }
}

void Intake::stop(){
   if(intakeMode == automatic) {
        IntakeMotor.stop(brakeType::hold);
    }
}
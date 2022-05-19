#include "vex.h"
using namespace vex;

enum INTAKEMODE {
  automatic = 0, 
  manual = 1
};

class Intake{
  private:
    bool installed = true;
    
    INTAKEMODE intakeMode = manual;
    double intakeSpeed = 100;
    const double intakeToDriveRatio = 1;
    bool autoStateOn = false;
  public:
    Intake(double intakeSpeed){
      this->intakeSpeed = intakeSpeed;
    }
    
    void setSpeed(double speed);
    void setSpeed(double speed, directionType dir);

    void start();
    void reverse();
    void stop();
    bool getAutoState(){
      return autoStateOn;
    }
    void changeAutoState(){
      autoStateOn = (autoStateOn)? false : true;
    }
    INTAKEMODE getMode();
    void switchMode(){
      setSpeed(0);
      intakeMode = intakeMode == automatic ? manual : automatic;
    }
    double getIntakeToDriveRatio();
};



// Intake::Intake(motor _intakeMotor, double _intakeSpeed){
//   intakeSpeed = _intakeSpeed;
//   if(IntakeMotor.installed()){
//     installed = true;
//   }
// }

void Intake::setSpeed(double speed){
  IntakeMotor.spin(directionType::fwd, speed, velocityUnits::rpm);
}

void Intake::setSpeed(double speed, directionType direction){
  IntakeMotor.spin(direction, speed, velocityUnits::rpm);
}

INTAKEMODE Intake::getMode(){
  return(intakeMode);
}

double Intake::getIntakeToDriveRatio(){
  return(intakeToDriveRatio);
}
void Intake::start(){
  setSpeed(intakeSpeed);  
}

void Intake::reverse(){
  setSpeed(intakeSpeed, directionType::rev);    
}

void Intake::stop(){
   IntakeMotor.stop(brakeType::hold);
}
#pragma once
#include "vex.h"
using namespace vex;

class Arms{
  private:
    
    bool installed = true;
    // motor_group armMotors = ArmMotors;
    
    double armSpeed = 80;
    double armDegree = 0;

  public:

    Arms();
    Arms(double _armSpeed);

    void raise(double speed);
    void raise();
    void lower(double speed);
    void lower();
    void calculateArmDegree(){
      double a = ArmMotors.rotation(degrees);
      armDegree = a;
    }

    void rotateDegree(double rotationAmount, double speed);
    void stop();

};
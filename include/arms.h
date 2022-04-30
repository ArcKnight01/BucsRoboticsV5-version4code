#include "vex.h"
using namespace vex;

class Arms{
  private:
    bool installed = false;
    motor_group armMotors = ArmMotors;
    
    double armSpeed;
    double armDegree;

  public:

    Arms();
    Arms(motor_group _armMotors, double _armSpeed);

    void raise(double speed);
    void raise();
    void lower(double speed);
    void lower();
    void rotateDegree(double rotationAmount, double speed);
    void stop();

};
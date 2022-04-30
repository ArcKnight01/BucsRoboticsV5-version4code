#include "vex.h"
using namespace vex;

class Wrist{
  private:
    motor wristMotor = WristMotor;
    double wristSpeed;
    double wristPosition;

  public: 
    Wrist();
    Wrist(motor _wristMotor, double _wristSpeed);

    void rotateUp();
    void rotateDown();

    

};
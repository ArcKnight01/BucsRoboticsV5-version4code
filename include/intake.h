#include "vex.h"
using namespace vex;

enum INTAKEMODE {
  automatic = 0, 
  manual = 1
};

class Intake{
  private:
    bool installed = false;
    motor intakeMotor = IntakeMotor;
    INTAKEMODE intakeMode = automatic;
    double intakeSpeed;
    const double intakeToDriveRatio = 1;

  public:
    Intake();
    Intake(motor _intakeMotor, double _intakeSpeed);

    void setSpeed(double speed);
    void setSpeed(double speed, directionType dir);

    void start();
    void reverse();
    void stop();

    int getMode();
    double getIntakeToDriveRatio();
};
#include "vex.h"
using namespace vex;

class Claw {
  private:
    bool installed = false;
    bool pneumaticMode = false;
    enum ClawState {opened = 0, closed = 1};
    motor clawMotor = ClawMotor; 
    double startingPosition;
    double closedPosition;
    double openedPosition;

  public:
    Claw();
    Claw(motor _clawMotor);
    ClawState clawState = opened;   
    void clamp();
    void unclamp();
    void initialize();
    void update();
    void changeState();
};
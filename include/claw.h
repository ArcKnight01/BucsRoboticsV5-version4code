#pragma once
#include "vex.h"
using namespace vex;
enum CLAWSTATE {opened = 0, closed = 1};

class Claw {
  private:
    bool installed = true;
    
    double startingPosition;
    double closedPosition;
    double openedPosition;

    double gearRatio = 5*5;
  public:
    Claw();
    // Claw(motor _clawMotor);
    CLAWSTATE clawState = opened;  
    
    CLAWSTATE getMode();

    void clamp(double rotationDegrees);
    void unclamp(double rotationDegrees);

    void initialize();
    void update();
    void changeState();
    
};
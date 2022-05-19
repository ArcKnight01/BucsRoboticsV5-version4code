#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
motor LeftFrontMotor = motor(PORT13, ratio18_1, false);
motor RightFrontMotor = motor(PORT12, ratio18_1, true);
motor LeftBackMotor = motor(PORT14, ratio18_1, false);
motor RightBackMotor = motor(PORT11, ratio18_1, true);
motor IntakeMotor = motor(PORT17, ratio18_1, false);
motor ArmMotorsMotorA = motor(PORT15, ratio18_1, false);
motor ArmMotorsMotorB = motor(PORT16, ratio18_1, true);
motor_group ArmMotors = motor_group(ArmMotorsMotorA, ArmMotorsMotorB);
motor ClawMotor = motor(PORT20, ratio18_1, false);
motor WristMotor = motor(PORT9, ratio18_1, false);
inertial Inertial = inertial(PORT5);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}
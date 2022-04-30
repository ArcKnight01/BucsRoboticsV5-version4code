#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
motor LeftFrontMotor = motor(PORT1, ratio18_1, false);
motor RightFrontMotor = motor(PORT2, ratio18_1, false);
motor LeftBackMotor = motor(PORT3, ratio18_1, false);
motor RightBackMotor = motor(PORT4, ratio18_1, false);
motor IntakeMotor = motor(PORT5, ratio18_1, false);
motor ArmMotorsMotorA = motor(PORT6, ratio18_1, false);
motor ArmMotorsMotorB = motor(PORT7, ratio18_1, true);
motor_group ArmMotors = motor_group(ArmMotorsMotorA, ArmMotorsMotorB);
motor ClawMotor = motor(PORT8, ratio18_1, false);
motor WristMotor = motor(PORT9, ratio18_1, false);

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
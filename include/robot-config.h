using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern controller Controller2;
extern motor LeftFrontMotor;
extern motor RightFrontMotor;
extern motor LeftBackMotor;
extern motor RightBackMotor;
extern motor IntakeMotor;
extern motor_group ArmMotors;
extern motor ClawMotor;
extern motor WristMotor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );
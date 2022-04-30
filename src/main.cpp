/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\arcar                                            */
/*    Created:      Thu Feb 17 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Controller2          controller                    
// LeftFrontMotor       motor         1               
// RightFrontMotor      motor         2               
// LeftBackMotor        motor         3               
// RightBackMotor       motor         4               
// IntakeMotor          motor         5               
// ArmMotors            motor_group   6, 7            
// ClawMotor            motor         8               
// WristMotor           motor         9               
// ---- END VEXCODE CONFIGURED DEVICES ----

//includes
#include "vex.h"
#include "intake.h"
#include "arms.h"
#include "claw.h"
#include "wrist.h"

using namespace vex;

competition Competition;

enum DRIVESPEED {crawlDrive = 60, slowDrive = 120, fastDrive = 360, maxDrive = 600}; //drivespeed is in RPM
DRIVESPEED driveSpeed = slowDrive;

Intake intake(IntakeMotor, 100);
Arms arms(ArmMotors, 80);
Claw claw();

int driveMode = 0;

/*temporary values*/
//double leftDriveVelocity = 0;
//double rightDriveVelocity = 0;
double counter = 0;
double tmpCounter = counter;
double whichDrive = 0;
double tmpBatterypct = 99;
double tmpDriveSpeed = driveSpeed;

/*BUTTONS*/
class Button{
  private:
    //nothing for now...
  public:
    Button();
    int heldThresh = 10;
    bool isPressing = false;
    bool isHeld = false;
    bool longPress = false;
    int heldTimer = 0;
    bool newPress = false;
    bool newRelease = false;
    int doublePressCheckTimer = 0;
    bool isDoublePress = false;

};

//letter buttons 
Button buttonB = Button();
Button buttonX = Button();
Button buttonY = Button();
Button buttonA = Button();

//arrow buttons
Button buttonDown = Button();
Button buttonUp = Button();
Button buttonLeft = Button();
Button buttonRight = Button();

//trigger buttons
Button buttonR1 = Button();
Button buttonR2 = Button();
Button buttonL1 = Button();
Button buttonL2 = Button();

// void setIntakeSpeed(double speed);
void sleep50();
double abso(double k);
void switchDriveMode();

void sleep50(){
    wait(50, msec);
    // sleepMs(50); //disable if not RobotMeshStudio
    
}

double abso(double k) {
    if(k < 0) return -k;
    else return k;
}

//button functions 


void switchDriveMode(){
  //stop all motors to prevent possible issues
  LeftBackMotor.stop();
  RightBackMotor.stop();
  LeftFrontMotor.stop();
  RightFrontMotor.stop();

  if (driveMode == 0){
    driveMode = 1;
  }
  else if (driveMode == 1){
    driveMode = 0;
  }
  //pause to prevent drive mode from switching back immediately
  wait(5, msec);
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // checkTouch();
  //initialize claw
  /*
  if(ClawMotor.installed()){
    ClawMotor.resetPosition();
    ClawMotor.resetRotation();
    ClawMotor.setVelocity(100, percent);
    ClawMotor.setTimeout(5, timeUnits::sec);
    ClawMotor.setBrake(brakeType::hold);
  }
  */

  // displayWhichAuto();
  wait(2000, msec);
  
  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

void drive(){
  this_thread::setPriority(1);
  while(true){
    //get the position of the joystick from a range of -100 to 100
    double leftPosition = Controller1.Axis3.position();
    double rightPosition = Controller1.Axis2.position();
    
    //set max position to be greatest of 2 positions (used for custom intake speed)
    double maxPosition = leftPosition;
    if(rightPosition > maxPosition) maxPosition = rightPosition;

    if(intake.getMode() == manual){
        double customIntake = maxPosition * intake.getIntakeToDriveRatio() * driveSpeed / 100;
        intake.setSpeed(abs(customIntake));
    }
    else if (intake.getMode() == automatic){

    }

    if(driveMode == 0){
      //normal driving, moves forward with arms in front
      LeftFrontMotor.spin(directionType::fwd, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      LeftBackMotor.spin(directionType::fwd, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      RightFrontMotor.spin(directionType::fwd, rightPosition * driveSpeed/100, velocityUnits::rpm);
      RightBackMotor.spin(directionType::fwd, rightPosition * driveSpeed/100, velocityUnits::rpm);
    }
    else if (driveMode == 1){
      //reverse driving mode, moves forward with intake in front
      LeftFrontMotor.spin(directionType::rev, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      LeftBackMotor.spin(directionType::rev, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      RightFrontMotor.spin(directionType::rev, rightPosition * driveSpeed/100, velocityUnits::rpm);
      RightBackMotor.spin(directionType::rev, rightPosition * driveSpeed/100, velocityUnits::rpm);
    }
    this_thread::sleep_for(10);

  }
}

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  
  
} 

void buttonPressedUpdate(Button button){
  if(!button.isPressing){
        button.newPress = true;
      }
      else{
        button.newPress = false;
        button.isHeld = true;
      }

      if(button.isHeld){
        if(button.heldTimer > button.heldThresh){
          button.longPress = true;
        }
        button.heldTimer += 1;
      } else {
        button.heldTimer = 0;
        button.longPress = false;
      }
      button.isPressing = true;
}

void buttonReleasedUpdate(Button button){
  if(button.isPressing){
    button.newRelease = true;
  }
  button.isPressing = false;
  button.isHeld = false;
}
void updateButtonStates(){
  if(Controller1.ButtonA.pressing()){
    buttonPressedUpdate(buttonA);
  }
  else {
    buttonReleasedUpdate(buttonA);
  }

  if(Controller1.ButtonB.pressing()){
    buttonPressedUpdate(buttonB);
  }
  else {
    buttonReleasedUpdate(buttonB);
  }

  if(Controller1.ButtonY.pressing()){
    buttonPressedUpdate(buttonY);
  }
  else {
    buttonReleasedUpdate(buttonY);
  }

  if(Controller1.ButtonX.pressing()){
    buttonPressedUpdate(buttonX);
  }
  else {
    buttonReleasedUpdate(buttonX);
  }
  
  if(Controller1.ButtonUp.pressing()){
    buttonPressedUpdate(buttonUp);
  }
  else {
    buttonReleasedUpdate(buttonUp);
  }

  if(Controller1.ButtonDown.pressing()){
    buttonPressedUpdate(buttonDown);
  }
  else {
    buttonReleasedUpdate(buttonDown);
  }
  
  if(Controller1.ButtonLeft.pressing()){
    buttonPressedUpdate(buttonLeft);
  }
  else {
    buttonReleasedUpdate(buttonLeft);
  }
  
  if(Controller1.ButtonRight.pressing()){
    buttonPressedUpdate(buttonRight);
  }
  else {
    buttonReleasedUpdate(buttonRight);
  }

  if(Controller1.ButtonL1.pressing()){
    buttonPressedUpdate(buttonL1);
  }
  else {
    buttonReleasedUpdate(buttonL1);
  }

  if(Controller1.ButtonL2.pressing()){
    buttonPressedUpdate(buttonL2);
  }
  else {
    buttonReleasedUpdate(buttonL2);
  }

  if(Controller1.ButtonR1.pressing()){
    buttonPressedUpdate(buttonR1);
  }
  else {
    buttonReleasedUpdate(buttonR1);
  }

  if(Controller1.ButtonR2.pressing()){
    buttonPressedUpdate(buttonR2);
  }
  else {
    buttonReleasedUpdate(buttonR2);
  }
}

void usercontrol(void){
  // Place drive control code here, inside the loop
  Brain.Screen.setFont(fontType::prop60);

  //Start all of the threads
    thread drivethread = thread(drive);
    // thread con_displaythread = thread(updateControllerScreen);
    // thread t_turnthread = thread(rotateTask);
    // thread brainthread = thread(checkTouch);
  
  while (true) {
    //region FRONT CONTROLLER BUTTONS//
    updateButtonStates();

    /*button A*/
    if(buttonA.isPressing){
      if(buttonA.newPress){
        //single new press, normal intake automatically
        if(intake.getMode()==automatic)
          intake.start();
      }
      else if (buttonA.longPress){
        //intake is held for a while, reverse intake automatically
        if(intake.getMode()==automatic){ 
          intake.reverse();
        }
      }
    }
    
    /*button B*/
    if(buttonB.isPressing){
      if(buttonB.newPress){
        
      }
      else if (buttonB.longPress){

      }
    }

    /*button X*/
    if(buttonX.isPressing){
      if(buttonX.newPress){
        //pressed once briefly
        switchDriveMode();
      }
      else if (buttonX.longPress){
        //do nothing
      }
    }

    /*button Y*/
    if(buttonY.isPressing){
      if(buttonY.newPress){

      }
      else if (buttonY.longPress){

      }
    }

    /*Arrow Buttons*/
    if(buttonUp.isPressing){
        //unused currently
        
    }

    if(buttonDown.isPressing){
        //unused currently
    }

    if(buttonLeft.isPressing){
      //change driveSpeed
      //opposite of button right
    }

    if(buttonRight.isPressing){
      //change driveSpeed
      if(buttonRight.newPress){}
        if(driveSpeed == crawlDrive){
            driveSpeed = fastDrive;
        }
        else if(driveSpeed == fastDrive){
            driveSpeed = slowDrive;
        }
        else if(driveSpeed == slowDrive){
            driveSpeed = crawlDrive;
        }
      }
      while(buttonRight.isHeld){
        sleep50();
      }
    }
    
    if(buttonL1.isPressing){
      //raise arms
      arms.raise();
    }

    if(buttonL2.isPressing){
      //lower arms
      arms.lower();
    }
    if(!(buttonL1.isPressing || buttonL1.isPressing)){
      //hold arms in place
      arms.stop();
    }
    if(buttonR1.isPressing){
      if(intake.getMode() == automatic)
        intake.start();
    }
    if(buttonR2.isPressing){
      if(intake.getMode() == automatic)
        intake.reverse();
    }
    if(!(buttonR1.isPressing || buttonR1.isPressing)){
      if(intake.getMode() == automatic)
        intake.stop();
    }

  }
}

int main(){
  Brain.Screen.clearScreen();
  // checkTouch();
  
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();
  
  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
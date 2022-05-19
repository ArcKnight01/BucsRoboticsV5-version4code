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
// LeftFrontMotor       motor         13              
// RightFrontMotor      motor         12              
// LeftBackMotor        motor         14              
// RightBackMotor       motor         11              
// IntakeMotor          motor         17              
// ArmMotors            motor_group   15, 16          
// ClawMotor            motor         20              
// WristMotor           motor         9               
// Inertial             inertial      5               
// ---- END VEXCODE CONFIGURED DEVICES ----

//includes
#include "vex.h"
#include "intake.h"
#include "arms.h"
#include "claw.h"
// #include "wrist.h"
// #include "inertial.h"
// #include "setter-functions.h"
// #include "robot.h"
#include "autonomousController.h"
// #include "vector2.h"
using namespace vex;
using namespace std;

competition Competition;

//speeds
DRIVESPEED driveSpeed = slowDrive;

double intakeSpeed = 100; //rpm
double armSpeed = 80; //rpm


Intake intake = Intake(intakeSpeed);
Arms arms = Arms(armSpeed);
Claw claw = Claw();

AutonomousController autonomousController;


enum DRIVEMODE { normal = 0, inverted = 1 }; 
DRIVEMODE driveMode = normal;

string debugString = "(>_<)";
string buttonString = "O";
int whichAuto = 0;

//used values below
double counter = 0;
double tmpCounter = counter;
double whichDrive = 0;
double tmpBatterypct = 99;
double tmpDriveSpeed = driveSpeed;

bool batteryWarning = false;


bool controller2affectsDrive = false;
bool doCustomIntake = false;

class Button{
  private:
    std::string name = "null";
    
  public:
    Button(string name){
      this->name = name;
    };

    string getName(){
      return(name);
    }

    int heldThresh = 10000000;
    bool isPressing = false;
    bool isHeld = false;
    bool longPress = false;
    int heldTimer = 0;
    bool newPress = false;
    bool newRelease = false;
    int doublePressCheckTimer = 0;
    bool isDoublePress = false;

    void pressedUpdate(){
      /*
        This code runs inside a if button pressed, does the necessary functions to update a button state
      */
      if(!isPressing){
        newPress = true;
      }
      else{
        newPress = false;
        isHeld = true;
      }

      if(isHeld){
        if(heldTimer > heldThresh){
          longPress = true;
        }
        heldTimer += 1;
      } else {
        heldTimer = 0;
        longPress = false;
      }
    
      isPressing = true;

      if(isPressing){
        // Controller1.rumble(". .");
        buttonString = getName();
        // sleep50();
      }

    }

    void releasedUpdate(){
      if(isPressing){
        newRelease = true;
      }
      isPressing = false;
      isHeld = false;
      longPress = false;

      if(isPressing) Controller1.rumble(".");
    }
};

class Test{
  private:
    int something;
  public:
    Test ();
    void doSomething();
};
Test::Test(){
  something = 100;
}

void Test::doSomething(){
  IntakeMotor.spin(directionType::fwd, something, velocityUnits::rpm);
}

Test test = Test();
/*BUTTONS*/
//letter buttons 
Button buttonB = Button("B");
Button buttonX = Button("X");
Button buttonY = Button("Y");
Button buttonA = Button("A");

//arrow buttons
Button buttonDown = Button("(v)");
Button buttonUp = Button("(^)");
Button buttonLeft = Button("(<)");
Button buttonRight = Button("(>)");

//trigger buttons
Button buttonR1 = Button("R1");
Button buttonR2 = Button("R2");
Button buttonL1 = Button("L1");
Button buttonL2 = Button("L2");

void switchDriveMode();
void updateControllerScreen();

//failsafes
void setIntakeSpeed(double speed);
void startIntake();
void reverseIntake();
void stopIntake();
vector2 getTouchPosition();

/***/
void pre_auton(void);

void switchDriveMode(){
  //stop all motors to prevent possible issues
  LeftBackMotor.stop();
  RightBackMotor.stop();
  LeftFrontMotor.stop();
  RightFrontMotor.stop();

  driveMode = (driveMode == normal) ? driveMode = inverted : driveMode = normal;
  
  //pause to prevent drive mode from switching back immediately
  wait(5, msec);
}

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // checkTouch();
  
  if(autonomousController.robotInertial.installed){
    autonomousController.robotInertial.calibrateSensor();
    autonomousController.robotInertial.setHeadingDegrees(0);
    autonomousController.robotInertial.setRotationDegrees(0);
  }

  if(ClawMotor.installed()){
    ClawMotor.resetPosition();
    ClawMotor.resetRotation();
    ClawMotor.setVelocity(100, percent);
    ClawMotor.setTimeout(5, timeUnits::sec);
    ClawMotor.setBrake(brakeType::hold);
  }

  // displayWhichAuto();
  wait(2000, msec);

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}


void drive(){
  //thread has priority
  this_thread::setPriority(1);
  
  while(true){

    //get the position of the joysticks from a range of -100 to 100
    double leftPosition = Controller1.Axis3.position();
    double rightPosition = Controller1.Axis2.position();
    
    // setIntakeSpeed(forward, 100);
    // intake.start();
    if(Controller2.installed() && controller2affectsDrive) {
        //access positions of controller axises
        double leftPositionC2 = Controller2.Axis3.position();
        double rightPositionC2 = Controller2.Axis2.position();
        
        //controller2 overwrites drive
        if(abso(leftPositionC2) > abso(leftPosition)) leftPosition = leftPositionC2;
        if(abso(rightPositionC2) > abso(rightPosition)) rightPosition = rightPositionC2;
      
    }

    //set max position to be greatest of 2 positions (used for custom intake speed)
    if(doCustomIntake){ 
      double maxPosition = leftPosition;
      if(rightPosition > maxPosition) maxPosition = rightPosition;
      // double customIntake = maxPosition * intakeToDriveRatio * driveSpeed / 100;
    }

    if(intake.getMode() == automatic){
      // setIntakeSpeed(600);
    }

    if(driveMode == normal){
      //normal driving, moves forward with arms in front
      LeftFrontMotor.spin(directionType::fwd, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      LeftBackMotor.spin(directionType::fwd, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      RightFrontMotor.spin(directionType::fwd, rightPosition * driveSpeed/100, velocityUnits::rpm);
      RightBackMotor.spin(directionType::fwd, rightPosition * driveSpeed/100, velocityUnits::rpm);
    }
    else if (driveMode == inverted){
      //reverse driving mode, moves forward with intake in front
      LeftFrontMotor.spin(directionType::rev, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      LeftBackMotor.spin(directionType::rev, leftPosition * driveSpeed/100, velocityUnits::rpm);        //motor spins at a speed defined by the axes, reported as a percent 
      RightFrontMotor.spin(directionType::rev, rightPosition * driveSpeed/100, velocityUnits::rpm);
      RightBackMotor.spin(directionType::rev, rightPosition * driveSpeed/100, velocityUnits::rpm);
    }

    //remove before competition!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // if(Controller1.ButtonUp.pressing()){
    //   Controller1.Screen.clearScreen();
    //   Controller1.Screen.setCursor(0, 1);
    //   Controller1.Screen.print("TEST TAKE THIS OUT ON COMPETITION BEFORE USING");
    //   Controller1.Screen.newLine();
    //   Controller1.Screen.print("IN DRIVE FUNCTION");
    //   sleep50();
    //   test.doSomething();
    //   Controller1.rumble("--.");
    // }
    this_thread::sleep_for(10);
  }
}

void updateBrain(){
  autonomousController.robotInertial.update();
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  Brain.Screen.setPenColor(color::yellow);
  Brain.Screen.print("Location: (%d, %d, %d)", autonomousController.robotInertial.getPosition().x, autonomousController.robotInertial.getPosition().y, autonomousController.robotInertial.getPosition().z);
  Brain.Screen.newLine();
  Brain.Screen.print("Angle: %d, Heading %d.", autonomousController.robotInertial.getRotation(), autonomousController.robotInertial.getHeading());
  Brain.Screen.newLine();
  Brain.Screen.setPenColor(color::cyan);
  Brain.Screen.print(whichAuto);
  Brain.Screen.setPenColor(color::blue);
  Brain.Screen.print(" driveMode: %s ",((driveMode==normal)?"Normal":"Inverted"));
  Brain.Screen.setPenColor(color::green);
  Brain.Screen.print(" intakeMode %s ",(intake.getMode() == automatic)?"AUTO":"MANUAL");
  Brain.Screen.newLine();
  Brain.Screen.setPenColor(color::red);
  Brain.Screen.print("BATTERY STATUS: %d% | %s ", Brain.Battery.capacity(pct), (batteryWarning)?"(!)":"(o_o)");
  this_thread::sleep_for(10);
}

void updateButtonStates(){
  if(Controller1.ButtonA.pressing()){
    buttonA.pressedUpdate();
    if(buttonA.isPressing == false){
      debugString = "bttnA error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonA.releasedUpdate();
    if(buttonA.isPressing == true){
      debugString = "bttnA error!";
      Controller1.rumble("..");
    }
  }

  if(Controller1.ButtonB.pressing()){
    buttonB.pressedUpdate();
    if(buttonB.isPressing == false){
      debugString = "bttnB error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonB.releasedUpdate();
    if(buttonB.isPressing == true){
      debugString = "bttnB error!";
      Controller1.rumble("..");
    }
  }

  if(Controller1.ButtonY.pressing()){
    buttonY.pressedUpdate();
    if(buttonY.isPressing == false){
      debugString = "bttnY error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonY.releasedUpdate();
    if(buttonY.isPressing == true){
      debugString = "bttnY error!";
      Controller1.rumble("..");
    }
  }

  if(Controller1.ButtonX.pressing()){
    buttonX.pressedUpdate();
    if(buttonX.isPressing == false){
      debugString = "bttnX error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonX.releasedUpdate();
    if(buttonX.isPressing == true){
      debugString = "bttnX error!";
      Controller1.rumble("..");
    }
  }
  
  if(Controller1.ButtonUp.pressing()){
    buttonUp.pressedUpdate();
    if(buttonUp.isPressing == false){
      debugString = "bttnUp error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonUp.releasedUpdate();
    if(buttonUp.isPressing == true){
      debugString = "bttnUp error!";
      Controller1.rumble("..");
    }
  }

  if(Controller1.ButtonDown.pressing()){
    buttonDown.pressedUpdate();
    if(buttonDown.isPressing == false){
      debugString = "bttnDwn error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonDown.releasedUpdate();
    if(buttonDown.isPressing == true){
      debugString = "bttnDwn error!";
      Controller1.rumble("..");
    }
  }
  
  if(Controller1.ButtonLeft.pressing()){
    buttonLeft.pressedUpdate();
    if(buttonLeft.isPressing == false){
      debugString = "bttnLft error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonLeft.releasedUpdate();
    if(buttonLeft.isPressing == true){
      debugString = "bttnLft error!";
      Controller1.rumble("..");
    }
  }
  
  if(Controller1.ButtonRight.pressing()){
    buttonRight.pressedUpdate();
    if(buttonRight.isPressing == false){
      debugString = "bttnRgt error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonRight.releasedUpdate();
    if(buttonRight.isPressing == true){
      debugString = "bttnRgt error!";
      Controller1.rumble("..");
    }
  }

  if(Controller1.ButtonL1.pressing()){
    buttonL1.pressedUpdate();
    if(buttonL1.isPressing == false){
      debugString = "bttnL1 error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonL1.releasedUpdate();
    if(buttonL1.isPressing == true){
      Controller1.rumble("..");
      debugString = "bttnL1 error!";
    }
  }

  if(Controller1.ButtonL2.pressing()){
    buttonL2.pressedUpdate();
    if(buttonL2.isPressing == false){
      debugString = "bttnL2 error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonL2.releasedUpdate();
    if(buttonL2.isPressing == true){
      debugString = "bttnL2 error!";
      Controller1.rumble("..");
    }
  }

  if(Controller1.ButtonR1.pressing()){
    buttonR1.pressedUpdate();
    if(buttonR1.isPressing == false){
      debugString = "bttnR1 error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonR1.releasedUpdate();
    if(buttonR1.isPressing == true){
      debugString = "bttnR1 error!";
      Controller1.rumble("..");
    }
  }

  if(Controller1.ButtonR2.pressing()){
    buttonR2.pressedUpdate();
    if(buttonR2.isPressing == false){
      debugString = "bttnR2 error!";
      Controller1.rumble("..");
    }
  }
  else {
    buttonR2.releasedUpdate();
    if(buttonR2.isPressing == true){
      debugString = "bttnR2 error!";
      Controller1.rumble("..");
    }
  }

}

void autonomous(void) {
      // ..........................................................................
      // Insert autonomous user code here.
      // ..........................................................................
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1, 1);
      Controller1.Screen.print("-    AUTO TEST    -");
      claw.unclamp(90);
      wait(250, msec);
      claw.clamp(90);
      intake.setSpeed(slowDrive);
      wait(5000, msec);
      intake.stop();
      arms.rotateDegree(90, 100);
      arms.stop();
      arms.rotateDegree(-90, 100);
      arms.stop();
      autonomousController.rotateDegree(180);
      
      switch(whichAuto){
        case 1:
        //TODO
        break;

        case 2:
        //TODO
        break;
        
      }
  
} 

void usercontrol(void){
  // Place drive control code here, inside the loop
  Brain.Screen.setFont(fontType::prop60);

  //Start all of the threads
  thread drivethread = thread(drive);
  thread con_displaythread = thread(updateControllerScreen);
  thread inertial_updatethread = thread(updateBrain);
  
  while (true) {
    
    updateButtonStates();
    ////region FRONT CONTROLLER BUTTONS//

    if(Controller1.ButtonA.pressing()){
      if(buttonA.newPress){
        if(intake.getMode() == automatic){
          if(intake.getAutoState() != true){
            //turn on intake motor to continuously run
            startIntake();
            intake.changeAutoState();
            // Controller1.rumble(".");
          }
          else if (intake.getAutoState() == true){
            //stop intake motor from continuously running
            stopIntake();
            intake.changeAutoState();
            Controller1.rumble(".");
          }
        }
        else if(intake.getMode() == manual){
          intake.switchMode();
          if(intake.getAutoState() == false) intake.changeAutoState();
          // intake.start();
          startIntake();
        }
        else{
          Controller1.rumble("-----");
          
          batteryWarning = true;
        }
      }
    } else {
      buttonA.releasedUpdate();
    }
        





    
    /*button B*/
    if(Controller1.ButtonB.pressing() && buttonB.newPress){
      setIntakeSpeed(0);
      intake.switchMode();
      buttonB.isPressing = true;
      if(intake.getAutoState() == true) intake.changeAutoState();
    }
    else if (!(Controller1.ButtonB.pressing())){
      buttonB.isPressing = false;
    }

    /*button X*/
    if(buttonX.isPressing && buttonX.newPress){
      /*can confirm this works*/
      
      switchDriveMode();

      while(Controller1.ButtonX.pressing()){
        sleep50();
      }
      
      wait(100, msec);
      
    }
    else if(Controller1.ButtonX.pressing() && buttonX.newPress){
      /*just in case this is a failsafe*/

      switchDriveMode();

      while(Controller1.ButtonX.pressing()){
        sleep50();
      }
      
      wait(100, msec);
      
    }

    /*button Y*/
    if(buttonY.isPressing && buttonY.newPress){
      claw.changeState();
      while (Controller1.ButtonY.pressing()){
        sleep50();
      }
    }
    //if above doesnt work:

    /*Arrow Buttons*/
    if(buttonUp.isPressing){/*TODO: Implement Button Up Functionality*/}

    if(buttonDown.isPressing){/*TODO: Implement Button Up Functionality*/}

    if(Controller1.ButtonLeft.pressing()){
      //change driveSpeed
      if(driveSpeed == crawlDrive){
          driveSpeed = slowDrive;
      }
      else if(driveSpeed == slowDrive){
          driveSpeed = fastDrive;
      }
      else if(driveSpeed == fastDrive){
          driveSpeed = crawlDrive;
      }

      while(Controller1.ButtonLeft.pressing()){
        sleep50();
      }

      wait(100, msec);

    }

    if(Controller1.ButtonRight.pressing()){
      //change driveSpeed
      if(driveSpeed == crawlDrive){
          driveSpeed = fastDrive;
      }
      else if(driveSpeed == fastDrive){
          driveSpeed = slowDrive;
      }
      else if(driveSpeed == slowDrive){
          driveSpeed = crawlDrive;
      }
      
      while(Controller1.ButtonRight.pressing()){
        sleep50();
      }

      wait(100, msec);
    }
    
    if(Controller1.ButtonL1.pressing() || (Controller2.installed() && Controller2.ButtonL1.pressing())){
      //raise arms
      // arms.raise();
      ArmMotors.spin(directionType::fwd, armSpeed, velocityUnits::rpm);
    }

    if(Controller1.ButtonL2.pressing() || (Controller2.installed() && Controller2.ButtonL2.pressing())){
      //lower arms
      // arms.lower();
      ArmMotors.spin(directionType::rev, armSpeed, velocityUnits::rpm);
      
    }
    if(!(Controller1.ButtonL2.pressing() || Controller1.ButtonL1.pressing()) || (Controller2.installed() && !(Controller2.ButtonL2.pressing() || Controller2.ButtonL1.pressing())))
    {
      //hold arms in place
      // arms.stop();
      ArmMotors.stop(brakeType::hold);
    }

    //todo fix all this
    if(intake.getMode() == automatic){
      if(intake.getAutoState() == true){
        intake.start();
        if(Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing()){
          
          if(intake.getAutoState() ==true) intake.changeAutoState();
          intake.switchMode();
          stopIntake();

        }
      }
    }



    if(Controller1.ButtonR1.pressing()){
      if(intake.getMode() == manual){
        if(intake.getAutoState() ==true) intake.changeAutoState();
        startIntake(); 
        IntakeMotor.spin(directionType::fwd, intakeSpeed, velocityUnits::rpm);
      }
    }

    if(Controller1.ButtonR2.pressing()){
      if(intake.getMode() == manual){
        if(intake.getAutoState() ==true) intake.changeAutoState();
        reverseIntake();
      } 
    }

    if(!(Controller1.ButtonR2.pressing() || Controller1.ButtonR1.pressing())){
      if(intake.getMode() == manual){
        stopIntake();
        if(intake.getAutoState() == true) intake.changeAutoState();
      }

    }
    
    
    //motor power checks (remanent of old code)
    //use to check for overheating?
    if(LeftFrontMotor.power(powerUnits::watt) > 17 || LeftBackMotor.power(powerUnits::watt) > 17 || RightFrontMotor.power(powerUnits::watt) > 17 || RightBackMotor.power(powerUnits::watt) > 17 ){
      Controller1.rumble(".");
      batteryWarning = true;
    }
    else if(Brain.Battery.capacity(pct) <= 25){
      batteryWarning = true;
    }
    else{
      batteryWarning = false;
    }
  }
}


int main(){

  Brain.Screen.clearScreen();

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();
  
  // Prevent main from exiting with an infinite loop.
  while (true) {wait(100, msec);}
}

void updateControllerScreen(){
    //initial variable declarations
    string driveSpeedText = "";

    /*Controller has 3 lines available to use*/
    
    //initialize!
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1,0);
    
    
    while(true){
        Controller1.Screen.clearScreen();
        
        /* UPDATE LINE 1*/
        if(tmpDriveSpeed != driveSpeed || tmpBatterypct != Brain.Battery.capacity(pct)){            // Line 1 updates, if percent or speed change
            sleep50(); 
            // tmpDriveSpeed = driveSpeed;
            // tmpBatterypct = Brain.Battery.capacity(pct);
            Controller1.Screen.setCursor(1,0);
            
            switch(driveSpeed){
              case fastDrive:
                driveSpeedText = "Fast ";
                break;
              
              case slowDrive:
                driveSpeedText = "Slow ";
                break;

              case crawlDrive:
                driveSpeedText = "Crawl";
                break;

              case maxDrive:
                driveSpeedText = "Max! ";
                break;

              default:
                driveSpeedText = "Error";
                break;
            }

            string warningString = "";
            if(batteryWarning){
              warningString = "(!)";
              if(Brain.Battery.capacity(pct) < 5) warningString = "(!!)";
            }else{
              warningString = " ";
            }
            Controller1.Screen.print("%s|Btt:%d%|%s", driveSpeedText.c_str(), Brain.Battery.capacity(pct), warningString.c_str());
            sleep50(); 
        }
        


        Controller1.Screen.setCursor(2,1); // Line 2 Updates the intake mode
        
        string intakeModeString = (intake.getMode() == automatic)? "A" : "M";
        string driveModeString = (driveMode == 0)? "N" : "R";
        
        Controller1.Screen.print("intk:[%s]|btn:[%s]|drv:[%s]", intakeModeString.c_str(), buttonString.c_str(), driveModeString.c_str());
        
        
        Controller1.Screen.setCursor(3, 1); //Line 3 updates the clawstate
        string clawStateString = (claw.getMode() == opened)? "Open ":"Closed";
        Controller1.Screen.print("claw:%s|%s", clawStateString.c_str(), debugString.c_str());
        
        this_thread::sleep_for(250);
    }
}


vector2 getTouchPosition(){
  vector2 position = vector2(Brain.Screen.xPosition(), Brain.Screen.yPosition());
  return position;
}

void setIntakeSpeed(double speed) {
    IntakeMotor.spin(directionType::fwd, speed, velocityUnits::rpm);
}

void setIntakeSpeed(double speed, directionType dir) {
    IntakeMotor.spin(dir, speed, velocityUnits::rpm);
}

void startIntake(){
  setIntakeSpeed(intakeSpeed, directionType::fwd);
}

void reverseIntake(){
  setIntakeSpeed(intakeSpeed, directionType::rev);
}

void stopIntake(){
  IntakeMotor.stop(brakeType::hold);
}
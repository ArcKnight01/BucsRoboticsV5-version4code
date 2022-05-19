// #include "vex.h"
// #include "setter-functions.h"

// using namespace std;
// using namespace vex;

// class ControllerUpdater {
//   private:
//     controller userController = Controller1;
//     const int numRows = 3;
//     const int numCols = 18;
//     double counter = 0;
//     double tmpCounter = counter;
//     double whichDrive = 0;
//     double tmpBatterypct = 99;
//     // DRIVESPEED driveSpeed = slowDrive;
//     // double tmpDriveSpeed = driveSpeed;
//   public:
//     ControllerUpdater();
//     void updateScreen();
//     void doErrorCode();
    
//   void displayCounter(){
//       Controller1.Screen.clearScreen();
//       sleep50();
//       Controller1.Screen.setCursor(1,0);
//       sleep50();
//       Controller1.Screen.print("Counter: %.f ", counter);
//       sleep50();
//   }
//   double getBatteryPct(){return(Brain.Battery.capacity(pct));}
  
// };
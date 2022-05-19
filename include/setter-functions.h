#include "vex.h"
using namespace vex;

const double pi = 3.14159;
const double wheelDiameter = 3; //inches
// const double wheelCircumference = pi * wheelDiameter;
const double wheelCircumference = 13; // inches , 3 * pi calculates to be 12.56 but measured is 13 for some reason
const double turnRatio = 18/4;      // diameter of robot's turn (diagonal wheels) / wheel diameter
const double intakeToDriveRatio = 1;
enum DRIVESPEED {crawlDrive = 60, slowDrive = 120, fastDrive = 360, maxDrive = 600}; //drivespeed is in RPM

void sleep50(){
    wait(50, msec);
    // sleepMs(50); //disable if not RobotMeshStudio 
}

double abso(double k) {
    if(k < 0) return -k;
    else return k;
}

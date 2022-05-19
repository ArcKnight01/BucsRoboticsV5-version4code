#include "inertial.h"
#include "vex.h"
using namespace vex;

InertialSensor::InertialSensor(){
  if(inertialSensor.installed()){
    installed = true;
  }
  else{
    installed = false;
  }
}

void InertialSensor::updateAcceleration(){
  vector3 linear_acceleration = vector3(inertialSensor.acceleration(xaxis), inertialSensor.acceleration(yaxis), inertialSensor.acceleration(zaxis));
  acceleration = vector3(
    linear_acceleration.x*cos(heading) + linear_acceleration.y*sin(heading), 
    linear_acceleration.y*cos(heading) - linear_acceleration.x*sin(heading),
    linear_acceleration.z
  );
  
}

vector3 InertialSensor::getAcceleration(){
  return(acceleration);
}
vector3 InertialSensor::getVelocity(){
  return(velocity);
}
vector3 InertialSensor::getPosition(){
  return(position);
}
double InertialSensor::calculateHeading(){
  heading = inertialSensor.heading();
  return(heading);
}

double InertialSensor::getHeading(){
  return(heading);
}

double InertialSensor::getRotation(){
  return(rotationDegrees);
}
double InertialSensor::calculateRotationRate(){
  rotationRate = inertialSensor.gyroRate(yaxis,  velocityUnits::dps);
  return (rotationRate);
}
double InertialSensor::calculateRotation(){
  rotationDegrees = inertialSensor.rotation();
  return(rotationDegrees);
}

double InertialSensor::calculateOrientation(){
  orientationDegrees = inertialSensor.orientation(pitch, degrees);
  return(orientationDegrees);
}

void InertialSensor::update(){
  heading = calculateHeading();
  rotationDegrees = calculateRotation();
  rotationRate = calculateRotationRate();
  orientationDegrees = calculateOrientation();
  updateAcceleration();
  velocity.add(acceleration);
  position.add(velocity);
  
  
}

void InertialSensor::calibrateSensor(){
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain Gyro
  wait(200, msec);
  Brain.Screen.print("Calibrating Gyro for Drivetrain");
  inertialSensor.calibrate();
  while(inertialSensor.isCalibrating()){
    wait(25, msec);
  }
  //reset screen
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1,1);
  wait(50, msec);
  Brain.Screen.clearScreen();
}
void InertialSensor::setRotationDegrees(double degrees){
    inertialSensor.setRotation(degrees, rotationUnits::deg);
}

void InertialSensor::setHeadingDegrees(double degrees){
    inertialSensor.setHeading(degrees, rotationUnits::deg);
}
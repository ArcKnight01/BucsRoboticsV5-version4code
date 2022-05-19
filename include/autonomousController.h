#include "vex.h"
#include "setter-functions.h"
#include "inertial.h"

using namespace vex;
class AutonomousController{
    private:
      int whichAuto = 0;
      
      float dist = 0;
      // float reference = 20;//target value
      float error; //
      float driveSpeed = fastDrive;
      float prev_error = 0;
      float diff_error;
      float Kp = 0.5;// OR 5
      float Kd = 0.5;
      float sum_error = 0;
      float Ki = 0.1;

    public:
      InertialSensor robotInertial = InertialSensor();
      double calculateDistance(vector3 pos0, vector3 pos1){
        double deltaX = pos1.x - pos0.x;
        double deltaY = pos1.y - pos0.y;
        double deltaR = sqrt(deltaX*deltaX + deltaY*deltaY);
        return(deltaR);
      }
      void driveForInches(double target_distance, double speed){
        bool negativeDist = (target_distance < 0) ? true : false;
        double rotations = target_distance / wheelCircumference;
        vector3 initialPos = robotInertial.getPosition();
        LeftFrontMotor.startRotateFor(rotations, rotationUnits::rev, speed, velocityUnits::rpm);
        LeftBackMotor.startRotateFor(rotations, rotationUnits::rev, speed, velocityUnits::rpm);
        RightBackMotor.startRotateFor(rotations, rotationUnits::rev, speed, velocityUnits::rpm);
        RightBackMotor.rotateFor(rotations, rotationUnits::rev, speed, velocityUnits::rpm);
        sleep50();
        double calculated_distance = calculateDistance(initialPos, robotInertial.getPosition());
        if(negativeDist) calculated_distance *= -1;
        if((target_distance - calculated_distance)>10)
          fixDriveDistance(target_distance, calculated_distance);
      }
      
      void rotateDegree(double degree){ 
        //positive degrees cause the robot to rotate right (clockwise)
        //negative degrees cause the robot to rotate left (counterclockwise)
        LeftFrontMotor.startRotateFor(degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm); 
        LeftBackMotor.startRotateFor(degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm);    
        RightBackMotor.startRotateFor(-degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm);
        RightBackMotor.rotateFor(-degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm);
      }

      // void rotateDegree(double degree, double speed){ 
      //   //positive degrees cause the robot to rotate right (clockwise)
      //   //negative degrees cause the robot to rotate left (counterclockwise)
      //   LeftFrontMotor.startRotateFor(degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm); 
      //   LeftBackMotor.startRotateFor(degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm);    
      //   RightBackMotor.startRotateFor(-degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm);
      //   RightBackMotor.rotateFor(-degree*turnRatio, rotationUnits::deg, slowDrive, velocityUnits::rpm);
      // }

      // void fixTurnDegree(double targetDegree, double currentDegree){
      //   prev_error = error;
      //   error = dist - reference;
      //   sum_error += error;
      //   diff_error = error - prev_error;
      //   driveSpeed = Kp * error + Ki*sum_error + Kd * diff_error;
      //   rotateDegree(error*-1, driveSpeed);
      // }
      
      void fixDriveDistance(double targetDistance, double currentDistance){
        
          dist = currentDistance;
          prev_error = error;
          error = dist - targetDistance;
          sum_error += error;
          diff_error = error - prev_error;
          driveSpeed = Kp * error + Ki*sum_error + Kd * diff_error;
          driveForInches(error*-1, driveSpeed);
        
        
      }
};


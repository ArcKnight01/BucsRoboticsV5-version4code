#include "vex.h"
#include "vector2.h"
using namespace std;
using namespace vex;

class InertialSensor {
  private:
    inertial inertialSensor = Inertial;

    vector3 position = vector3(0,0,0);
    vector3 velocity = vector3(0,0,0);
    vector3 acceleration;
    double heading;
    double rotationRate;
    double rotationDegrees;
    double orientationDegrees;
    

  public:
      InertialSensor();
      //setter functions
      bool installed = false;
      void setRotationDegrees(double degrees);
      void setHeadingDegrees(double degrees);
      void update();

      double getHeading();
      double getRotationRate();
      double getOrientation();
      double getRotation();

      vector3 getAcceleration();
      vector3 getVelocity();
      vector3 getPosition();

      double calculateRotation();
      double calculateHeading();
      double calculateRotationRate();
      double calculateOrientation();
      void updateAcceleration();
      void calibrateSensor();
};
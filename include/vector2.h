struct vector2{
  double x;
  double y;

  vector2(){
    this->x = 0;
    this->y = 0;
  }

  vector2(double x, double y){
    this->x = x;
    this->y = y;
  }
};

struct vector3{
  double x;
  double y;
  double z;

  vector3(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }

  vector3(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
  }

  vector3 add(vector3 &otherVector){
    this->x += otherVector.x;
    this->y += otherVector.y;
    this->z += otherVector.z;
    
    return(vector3(this->x, this->y, this->z));
  }
};
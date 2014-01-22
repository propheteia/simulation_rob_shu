#ifndef OBJECT_WM_H
#define OBJECT_WM_H
#include "vector.h"
using namespace salt;
class Object
{
public:
  	salt::Vector3f pos;
	salt::Vector3f pos_local;
	double distance;
	double angle;
public:
	void setDistance(double distance){distance = distance;}
	double GetDistanceToSelf(){return distance;}
	
	void setLocalPos(Vector3f l_pos){pos_local = l_pos;}

};
#endif
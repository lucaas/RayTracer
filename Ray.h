#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray
{
public:
	Ray() : direction(0), origin(0), depth(0) {}
	cbh::vec3 direction;
	cbh::vec3 origin;	
	unsigned int depth;
private:
};


#endif
#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

#define EPSILON 10e-4

class ImplicitObject;

class Ray
{
public:
	Ray() : direction(0), origin(0), depth(0), t(0), currentObject(NULL) {}
	Ray(const Ray & r) : direction(r.direction), origin(r.origin), depth(r.depth), t(r.t), currentObject(r.currentObject) {}

	cbh::vec3 direction;
	cbh::vec3 origin;	
	unsigned int depth;   
	ImplicitObject * currentObject;
	double t;    
	inline cbh::vec3 getOffset() {
		return t*EPSILON*direction;
	}

};


#endif
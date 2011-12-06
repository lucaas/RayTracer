#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

#define EPSILON 10e-5

class ImplicitObject;

class Ray
{
public:
	Ray() : direction(0), origin(0), diffuseDepth(0),reflectionDepth(0),refractionDepth(0), t(0),n(1), currentObject(NULL), normal(0) {}
	Ray(const Ray & r) : 
	n(1.0), direction(r.direction), origin(r.origin), diffuseDepth(r.diffuseDepth), reflectionDepth(r.reflectionDepth),refractionDepth(r.refractionDepth),
		t(r.t), currentObject(r.currentObject), normal(r.normal) {}

	void reset()
	{
		diffuseDepth = reflectionDepth = refractionDepth = t = 0;
		n = 1;
		direction[0] = direction[1] = direction[2] = origin[0] = origin[1] = origin[2] = normal[0] = normal[1] = normal[2] = 0;
		currentObject = NULL;
	}

	cbh::vec3 direction;
	cbh::vec3 origin;	
	cbh::vec3 normal;
	
	unsigned int diffuseDepth, reflectionDepth, refractionDepth;
	ImplicitObject * currentObject;
	double t,n;    
	void Offset() {
		origin += (t*EPSILON)*direction;
	}

};


#endif
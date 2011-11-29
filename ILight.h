#ifndef ILIGHT
#define ILIGHT

#include "Vector3.h"
#include "Ray.h"
#include "ImplicitObject.h"

class ILight
{
public:
	virtual void getIntersection(Ray &ray) = 0;
	virtual cbh::vec3 brdf(cbh::vec3 &incoming) = 0;
	virtual cbh::vec3 getPosition() = 0;
	virtual cbh::vec3 getRandomPosition() = 0;
	virtual ImplicitObject * getObject() = 0;
};





#endif
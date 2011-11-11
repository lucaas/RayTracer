#ifndef ILIGHT
#define ILIGHT

#include "Vector3.h"
#include "Ray.h"

class ILight
{
public:
	virtual void getIntersection(const Ray &ray) = 0;
	virtual cbh::vec3 brdf(cbh::vec3 &incoming) = 0;
};





#endif
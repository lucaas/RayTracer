#ifndef IMPLICIT_PLANE
#define IMPLICIT_PLANE

#include "SimpleMaterial.h"
#include "ImplicitObject.h"
#include "Vector3.h"
#include "Ray.h"

class ImplicitPlane : public ImplicitObject
{
public: 
	ImplicitPlane(float distance, cbh::vec3 normal) : distance(distance), normal(normal) {}

	bool intersects(Ray &ray) const
	{
		ray.t = -1.0f * (ray.origin.dot(normal) + distance)/(ray.direction.dot(normal));
		
		
		
		//if t > 0 we intersect the plane
		//Due to numerical errors we make sure that we are on
		//the side of the plane we shoot the ray from
		return (ray.t > 0);
	}

	cbh::vec3 getNormal(cbh::vec3 intersection) const
	{
		return normal;
	}

	cbh::vec3 getPosition() const
	{
		return cbh::vec3(0);
	}

	cbh::vec3 getRandomPosition() {
		return cbh::vec3(0);
	}

private:
	float distance;
	cbh::vec3 normal;
};





#endif
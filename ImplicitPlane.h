#ifndef IMPLICIT_PLANE
#define IMPLICIT_PLANE

#include "Ray.h"
#include "SimpleMaterial.h"
#include "ImplicitObject.h"
#include "Vector3.h"

class ImplicitPlane : public ImplicitObject
{
public: 
	ImplicitPlane(float distance, cbh::vec3 normal) : distance(distance), normal(normal) 
	{
		material = new SimpleMaterial(0,1,0, cbh::vec3(1.0f, 0.0f, 0.0f));
	}

	bool intersects(Ray &ray) const
	{
		ray.t = -1.0f * (ray.origin.dot(normal) + distance)/(ray.direction*normal);
		//if t > 0 we intersect the plane
		//Due to numerical errors we make sure that we are on
		//the side of the plane we shoot the ray from
		return (ray.t > 10e-6);
	}

	cbh::vec3 getNormal(cbh::vec3 intersection) const
	{
		return normal;
	}

	const SimpleMaterial & getMaterial() const
	{
		return *material;
	}

private:
	float distance;
	cbh::vec3 normal;
};





#endif
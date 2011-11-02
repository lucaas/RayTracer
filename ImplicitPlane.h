#ifndef IMPLICIT_PLANE
#define IMPLICIT_PLANE


#include "glm.hpp"
#include "Ray.h"
#include "Material.h"
#include "ImplicitObject.h"

class ImplicitPlane : public ImplicitObject
{
public: 
	ImplicitPlane(float distance, glm::vec3 normal) : distance(distance), normal(normal) 
	{
		material = Material(0.1f, 1.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	bool intersects(const Ray &ray,float &t) const
	{
		t = -1.0f * (glm::dot(ray.origin, normal) + distance)/(glm::dot(ray.direction,normal));
		return (t > 0);
	}

	glm::vec3 getNormal(glm::vec3 intersection) const
	{
		return normal;
	}

	Material getMaterial() const
	{
		return material;
	}

	Material material;
private:
	float distance;
	glm::vec3 normal;
};





#endif
#ifndef IMPLICIT_SPHERE
#define IMPLICIT_SPHERE


#include "glm.hpp"
#include "Ray.h"
#include "Material.h"
#include "ImplicitObject.h"

class ImplicitSphere : public ImplicitObject
{
public: 
	ImplicitSphere(float radius,glm::vec3 position) : position(position), radius(radius) {
		material = Material(0.1f, 1.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	bool intersects(const Ray &ray,float &t) const
	{
		float a = glm::dot(ray.direction,ray.direction);
		glm::vec3 dist = ray.origin - position;
		glm::vec3 raydir2 = ray.direction*2.0f;
		float b = glm::dot(raydir2 , dist);
		float c = glm::dot(dist,dist) - radius * radius;
		
		//Find discriminant
		float disc = b * b - 4 * a * c;

		// if discriminant is negative there are no real roots, so return 
		// false as ray misses sphere
		if (disc < 0)
			return false;

		// compute q as described above
		float distSqrt = sqrtf(disc);
		float q;
		if (b < 0)
			q = (-b - distSqrt)/2.0f;
		else
			q = (-b + distSqrt)/2.0f;

		// compute t0 and t1
		float t0 = q / a;
		float t1 = c / q;

		// make sure t0 is smaller than t1
		if (t0 > t1)
		{
			// if t0 is bigger than t1 swap them around
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}

		// if t1 is less than zero, the object is in the ray's negative direction
		// and consequently the ray misses the sphere
		if (t1 < 0)
			return false;

		// if t0 is less than zero, the intersection point is at t1
		// camera is inside the sphere
		if (t0 < 0)
		{
			t = t1;
			return true;
		}
		// else the intersection point is at t0
		else
		{
			t = t0;
			return true;
		}
	}

	glm::vec3 getNormal(glm::vec3 intersection) const
	{
		glm::vec3 normal = intersection - position;
		return normal / radius;
	}

	Material getMaterial() const
	{
		return material;
	}

	Material material;
private:
	glm::vec3 position;
	float radius;
};





#endif
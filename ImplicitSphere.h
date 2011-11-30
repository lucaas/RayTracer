#ifndef IMPLICIT_SPHERE
#define IMPLICIT_SPHERE

#include "IMaterial.h"

#define M_PI 3.14159265358979323846

class ImplicitSphere : public ImplicitObject
{
public: 
	ImplicitSphere() {}
	ImplicitSphere(float radius,cbh::vec3 position) : position(position), radius(radius) {}
	
	bool intersects(Ray &ray) const
	{
		double a = ray.direction.squareNorm();
		cbh::vec3 dist = ray.origin - position;
		double b = 2.0*dist.dot(ray.direction);
		double c = dist.squareNorm() - radius * radius;
		
		//Find discriminant
		double disc = b * b - 4 * a * c;

		// if discriminant is negative there are no real roots, so return 
		// false as ray misses sphere
		if (disc < 0)
			return false;

		// compute q as described above
		double distSqrt = sqrt(disc);
		double q;
		if (b < 0)
			q = (-b - distSqrt)/2.0;
		else
			q = (-b + distSqrt)/2.0;

		// compute t0 and t1
		double t0 = q / a;
		double t1 = c / q;

		// make sure t0 is smaller than t1
		if (t0 > t1)
		{
			// if t0 is bigger than t1 swap them around
			double temp = t0;
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
			ray.t = t1;
            ray.normal = (ray.origin + ray.t*ray.direction - position).normalize();
			return true;
		}
		// else the intersection point is at t0
		else
		{
			ray.t = t0;
            ray.normal = (ray.origin + ray.t*ray.direction - position).normalize();
			return true;
		}
	}

	cbh::vec3 getNormal(cbh::vec3 intersection) const
	{
		return (intersection - position).normalize();
	}

	cbh::vec3 getPosition() const
	{
		return position;
	}

	cbh::vec3 getRandomPosition() {
		double rad = radius;
		double theta = ((double)rand()/(double)(RAND_MAX+1)) * M_PI;
		double phi = ((double)rand()/(double)(RAND_MAX+1)) * 2 * M_PI;

		cbh::vec3 offset(rad*sin(theta)*cos(phi), rad*sin(theta)*sin(phi), rad*cos(theta));
		return position + offset;
	}

	cbh::vec3 position;
	double radius;

private:

};





#endif
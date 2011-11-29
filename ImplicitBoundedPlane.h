#ifndef IMPLICIT_BOUNDED_PLANE
#define IMPLICIT_BOUNDED_PLANE

#include "SimpleMaterial.h"
#include "ImplicitObject.h"
#include "Vector3.h"
#include "Ray.h"

class ImplicitBoundedPlane : public ImplicitObject
{
public:
	//Position is the center of the plane
	ImplicitBoundedPlane(cbh::vec3 position, double width, double height, cbh::vec3 normal) 
		: position(position), width(width), height(height), halfwidth(0.5*width), halfheight(height*0.5), normal(normal) {}

	bool intersects(Ray &ray) const
	{
		ray.t = -1.0f * (ray.origin.dot(normal) + position[1])/(ray.direction.dot(normal));
		//if t > 0 we intersect the plane
		//Due to numerical errors we make sure that we are on
		//the side of the plane we shoot the ray from
		
		//Do we hit the plane
		if(ray.t < 0)
		 return false;

		//Where do we hit?
		cbh::vec3 hitPos = ray.origin + ray.t*ray.direction;

		//Check X and Z value is inside bounds
		if(hitPos[0] > position[0] - halfwidth &&  hitPos[0] < position[0] + halfwidth &&
			hitPos[2] > position[2] - halfheight &&  hitPos[2] < position[2] + halfheight)
			return true;
		else
			return false;

		
		
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
		double rW = halfheight*2.0*((double)rand()/(double)(RAND_MAX) - 0.5); // r [-halfheight,halfheight];
		double rH = halfwidth*2.0*((double)rand()/(double)(RAND_MAX) - 0.5); // r [-halfwidth,halfwidth];

		return position + cbh::vec3(rW,0,rH);
	}

private:
	cbh::vec3 position, normal;
	double width,height,halfwidth,halfheight;

};





#endif
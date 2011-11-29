#ifndef PLANE_AREA_LIGHT
#define PLANE_AREA_LIGHT


#include "ILight.h"
#include "ImplicitBoundedPlane.h"

class PlaneAreaLight : public ILight 
{
public:
	PlaneAreaLight() : color(cbh::vec3(1)),intensity(0.2)
	{
		
	}

	PlaneAreaLight(cbh::vec3 position, cbh::vec3 color, double intensity) : color(cbh::vec3(1)),intensity(0.2)
	{
		plane = new ImplicitBoundedPlane(position, 2, 1.5, cbh::vec3 (0,-1,0));
	}

	cbh::vec3 getPosition() {
		return plane->getPosition();
	}


	ImplicitObject * getObject()
	{
		return plane;
	}

	// Returns random position somewhere on the sphere surface
	cbh::vec3 getRandomPosition() {
		return plane->getRandomPosition();
	}

	void getIntersection(Ray &ray)
	{
		plane->intersects(ray);
	}

	cbh::vec3 brdf(cbh::vec3 &incoming)
	{
		return cbh::vec3(intensity);
	}


	double intensity;

private:
	cbh::vec3 color;
	ImplicitBoundedPlane *plane;
};



#endif
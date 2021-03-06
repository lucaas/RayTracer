#ifndef UNIFORM_AREA_LIGHT
#define UNIFORM_AREA_LIGHT

#define USE_MATH_DEFINES


#include "ILight.h"
#include "ImplicitSphere.h"

class UniformAreaLight : public ILight 
{
public:
	UniformAreaLight() : color(cbh::vec3(1)),intensity(0.2)
	{
		sphere = new ImplicitSphere(1,cbh::vec3(0));
	}

	UniformAreaLight(double radius, cbh::vec3 position) : color(cbh::vec3(1)),intensity(0.2)
	{
		sphere = new ImplicitSphere(radius,position);
	}

	cbh::vec3 getPosition() {
		return sphere->position;
	}


	ImplicitObject * getObject()
	{
		return sphere;
	}

	// Returns random position somewhere on the sphere surface
	cbh::vec3 getRandomPosition() {

		

		double rad = sphere->radius + 0.2;
		double theta = ((double)rand()/(double)(RAND_MAX+1)) * M_PI;
		double phi = ((double)rand()/(double)(RAND_MAX+1)) * 2 * M_PI;

		cbh::vec3 offset(rad*sin(theta)*cos(phi), rad*sin(theta)*sin(phi), rad*cos(theta));
		return sphere->position + offset;
	}

	void getIntersection(Ray &ray)
	{
		sphere->intersects(ray);
	}

	cbh::vec3 brdf(cbh::vec3 &incoming)
	{
		return cbh::vec3(intensity);
	}


	double intensity;
private:
	cbh::vec3 color;
	ImplicitSphere *sphere;
};



#endif
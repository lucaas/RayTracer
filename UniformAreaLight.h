#ifndef UNIFORM_AREA_LIGHT
#define UNIFORM_AREA_LIGHT

#include "ILight.h"
#include "ImplicitSphere.h"

class UniformAreaLight : public ILight 
{
public:
	UniformAreaLight() : color(cbh::vec3(1)),intensity(1)
	{
		sphere = ImplicitSphere(1,cbh::vec3(0));
	}

	UniformAreaLight(cbh::vec3 position,double radius) : color(cbh::vec3(1)),intensity(1)
	{
		sphere = ImplicitSphere(radius,position);
	}


	void getIntersection(Ray &ray)
	{
		sphere.intersects(ray);
	}
	cbh::vec3 brdf(cbh::vec3 &incoming)
	{
		return 1;
	}
private:
	cbh::vec3 color;
	double intensity;
	ImplicitSphere sphere;
};



#endif
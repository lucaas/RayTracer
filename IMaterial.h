#ifndef IMATERIAL
#define IMATERIAL

#include "Vector3.h"

class IMaterial
{
public:
	// Diffuse material
	IMaterial(cbh::vec3 color, double kd) : color(color), kd(kd), ks(0), specularPower(20), rIndex(1) {}
	
	// Normal/Specular
	IMaterial(cbh::vec3 color, double kd, double ks, double specularPower) : color(color), kd(kd), ks(ks), specularPower(specularPower), rIndex(1) {}
	
	// With rIndex
	IMaterial(cbh::vec3 color, double kd, double ks, double specularPower, double rIndex) : color(color), kd(kd), ks(ks), specularPower(specularPower), rIndex(rIndex) {}
	
	
	virtual cbh::vec3 brdf(const cbh::vec3 & PerfectReflection, const cbh::vec3 & outgoing) const = 0;
	virtual cbh::vec3 sampleHemisphere(const cbh::vec3 & normal, double & pdf) const = 0;
	
	double kd,ks;
	double rIndex;
	double specularPower;
	cbh::vec3 color;
};


#endif
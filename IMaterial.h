#ifndef IMATERIAL
#define IMATERIAL

#include "Vector3.h"

class IMaterial
{
public:
	// Light source material
	IMaterial(cbh::vec3 emittance, bool isLightSource) : color(1), kd(1), kr(0), kt(0), specularPower(20), rIndex(1), emittance(emittance), isLightSource(isLightSource) {}

	// Diffuse material
	IMaterial(cbh::vec3 color, double kd) : color(color), kd(kd), kr(0), kt(0), specularPower(20), rIndex(1), emittance(0,0,0), isLightSource(false) {}
	
	// Normal/Specular
	IMaterial(cbh::vec3 color, double kd, double kr, double kt, double specularPower) : color(color), kd(kd), kr(kr), kt(kt), specularPower(specularPower), rIndex(1), emittance(0,0,0), isLightSource(false) {}
	
	// With rIndex
	IMaterial(cbh::vec3 color, double kd, double kr, double kt, double specularPower, double rIndex) : color(color), kd(kd), kr(kr),kt(kt), specularPower(specularPower), rIndex(rIndex), emittance(0,0,0), isLightSource(false) {}
	
	
	virtual cbh::vec3 brdf(const cbh::vec3 & PerfectReflection, const cbh::vec3 & outgoing) const = 0;
	virtual cbh::vec3 sampleHemisphere(const cbh::vec3 & normal, double & pdf) const = 0;
	
	//kd(Diffuse), kr(Reflectance), kt(Transmittance)
	double kd,kr,kt;
	double rIndex;
	double specularPower;
	cbh::vec3 color;
	cbh::vec3 emittance;
	bool isLightSource;
};


#endif
#ifndef RayTracer_LightMaterial_h
#define RayTracer_LightMaterial_h

#include "IMaterial.h"

class LightMaterial : public IMaterial {
public:
	
	// Light Material
	LightMaterial(cbh::vec3 emittance, bool isLightSource) : IMaterial(emittance, isLightSource) {}

	cbh::vec3 brdf(const cbh::vec3 & PerfectReflection, const cbh::vec3 & outgoing) const 
	{
		// Modified Blinn-Phong
		//cbh::vec3 halfVec = (-incident + outgoing).normalize();
		//halfVec = halfVec.normalize();
		
		// kr+kt since refractive materials should reflect due to fresnel's equation
		return color * ((kr+kt) * pow(PerfectReflection.dot(outgoing), specularPower) + kd);		

	}


	//Importance sampling of hemisohere according to phong brdf
	cbh::vec3 sampleHemisphere(const cbh::vec3 & normal, double & pdf) const
	{
		double r1((double)rand() / ((double)RAND_MAX + 1));
		double r2((double)rand() / ((double)RAND_MAX + 1));
		
		r2 = pow(r2,1.0/(specularPower+1));
	
		cbh::vec3 d(cos(2.0 * M_PI * r1)*sqrt(1-r2), sin(2.0 * M_PI * r1)*sin(sqrt(1-r2)), r2 );

		double theta = -acos(normal.getZ());
		double phi = -atan2(normal.getY(),normal.getX());

		/* Ry * Rz * [x; y; z]
		y*sin(phi) + x*cos(phi)*cos(theta) - z*cos(phi)*sin(theta)
		y*cos(phi) - x*cos(theta)*sin(phi) + z*sin(phi)*sin(theta)
		z*cos(theta) + x*sin(theta)
		*/	
		cbh::vec3 d2(d); //Temporary direction vector;
		d.setX(d2.getY()*sin(phi) + d2.getX()*cos(phi)*cos(theta) - d2.getZ()*cos(phi)*sin(theta));
		d.setY(d2.getY()*cos(phi) - d2.getX()*cos(theta)*sin(phi) + d2.getZ()*sin(phi)*sin(theta));
		d.setZ(d2.getZ()*cos(theta) + d2.getX()*sin(theta));
	

		pdf = (specularPower + 1.0)*pow(r2,specularPower) / 2*M_PI;

		return d.normalize();


	}

	
};

#endif

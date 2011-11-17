#ifndef RayTracer_DiffuseMAterial_h
#define RayTracer_DiffuseMAterial_h

#include "IMaterial.h"

class DiffuseMaterial : public IMaterial {
public:
	
	// Diffuse material
	DiffuseMaterial(cbh::vec3 color, double kd) : IMaterial(color, kd, 0,0,0) {}
		
	cbh::vec3 brdf(const cbh::vec3 & PerfectReflection, const cbh::vec3 & outgoing) const 
	{
		return color * kd;
	}


	//Importance sampling of hemisohere according to phong brdf
	cbh::vec3 sampleHemisphere(const cbh::vec3 & normal, double & pdf) const
	{
		double r1((double)rand() / ((double)RAND_MAX + 1));
		double r2((double)rand() / ((double)RAND_MAX + 1));

		while(r2 < 10e-4)
		{
			r2 = ((double)rand() / ((double)RAND_MAX + 1));
		}
	
		cbh::vec3 d(cos(2.0 * M_PI * r1)*sqrt(1-r2), sin(2.0 * M_PI * r1)*sin(sqrt(1-r2)), sqrt(r2) );

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
	

		pdf = sqrt(r2) / M_PI;

		return d.normalize();


	}

};

#endif

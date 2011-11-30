#ifndef RayTracer_DiffuseMAterial_h
#define RayTracer_DiffuseMAterial_h

#include "IMaterial.h"

class WardMaterial : public IMaterial {
public:
	
	// Diffuse material
	WardMaterial(cbh::vec3 color, double kd) : IMaterial(color, kd, 0,0,0) {}
    
	cbh::vec3 brdf(const cbh::vec3 & incomming, const cbh::vec3 & normal,const cbh::vec3 & outgoing) const 
	{
        cbh::vec3 half = (incomming + outgoing).normalize();
        
        // Implementation of equation 4 in Notes on the Ward BRDF, Walter.
        // http://www.graphics.cornell.edu/~bjw/wardnotes.pdf
        
        double ax = 4;
        double ay = 4;
        
        // First part of equation
        double fr = 4*M_PI*ax*ay;
        fr *= sqrt( incomming.dot(normal) * outgoing.dot(normal) );
        
        
        // Second part of equation
        double spec = pow(half.dot(xvec) / ax, 2);
        spec += pow(half.dot(yvec) / ay, 2);
        spec /= pow(half.dot(normal), 2);
        spec = exp(-spec);
        
        fr *= spec;
        
        double lambertian = 
        
        return color * fr;      
	
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
        
		cbh::vec3 d2(d); //Temporary direction vector;
		d.setX(d2.getY()*sin(phi) + d2.getX()*cos(phi)*cos(theta) - d2.getZ()*cos(phi)*sin(theta));
		d.setY(d2.getY()*cos(phi) - d2.getX()*cos(theta)*sin(phi) + d2.getZ()*sin(phi)*sin(theta));
		d.setZ(d2.getZ()*cos(theta) + d2.getX()*sin(theta));
        
        
		pdf = sqrt(r2) / M_PI;
        
		return d.normalize();
        
	}
    
};

#endif

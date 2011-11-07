#ifndef UTILITY
#define UTILITY

#include <cmath>
#include "Vector3.h"
#include "Ray.h"
#include "SimpleMaterial.h"


//Converts a float color vector [0,1] into 8 bit [0,255]
//Assumes input to be in the range [0,1]
inline cbh::vec3uc float2rgb (cbh::vec3 color) {

	// clamp colors here too?
	color = color.clamp(0,1);
	return cbh::vec3uc((unsigned char)(255*color.getX()),(unsigned char)(255*color.getY()),(unsigned char)(255*color.getZ()));
}

// Blinn-Phong Local lighting
// All vectors point outwards from the surface
cbh::vec3 phong(const SimpleMaterial &material, const cbh::vec3 &position, const PointLight &light, const cbh::vec3 &viewDir, const cbh::vec3 &normal) {
	cbh::vec3 lightDir = (light.position - position).normalize();
	cbh::vec3 halfVec = lightDir + viewDir;
	halfVec = halfVec.normalize();
	
	double diffuseIntensity = lightDir.dot(normal);
	double specularIntensity = pow(normal.dot(halfVec),material.specular);

	// ambient
	cbh::vec3 result = material.ka * material.acolor;

	// Diffuse
	result += material.kd * material.dcolor * diffuseIntensity;

	// Specular
	result += material.ks * material.scolor * specularIntensity;

	result = light.intensity * result.mtimes(light.color);


	// Not sure this is correct, limits each channel to [0,1]
	return result.clamp(0,1);
}




#endif
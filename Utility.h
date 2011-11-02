#ifndef UTILITY
#define UTILITY

#include "ColorRGB.h"
#include "glm.hpp"

//Converts a float color vector [0,1] into 8 bit [0,255]
//Assumes input to be in the range [0,1]
inline ColorRGB float2rgb (glm::vec3 color) {
    
	return ColorRGB((unsigned char)(255*color.r),(unsigned char)(255*color.g),(unsigned char)(255*color.b));
}

// Blinn-Phong Local lighting
// All vectors point outwards from the surface
glm::vec3 phong(const Material &material, const glm::vec3 &position, const PointLight &light, const glm::vec3 &viewDir, const glm::vec3 &normal) {
	glm::vec3 lightDir = glm::normalize(light.position - position);
	glm::vec3 halfVec = lightDir + viewDir;
	halfVec = glm::normalize(halfVec);

	float diffuseIntensity = glm::clamp(glm::dot(lightDir,normal), 0.0f, 1.0f);
	float specularIntensity = pow(glm::clamp(glm::dot(normal, halfVec), 0.0f, 1.0f), material.specular);

	// ambient
	glm::vec3 result = material.ka * material.acolor;

	// Diffuse
	result += material.kd * material.dcolor * diffuseIntensity;

	// Specular
	result += material.ks * material.scolor * specularIntensity;

	result *= light.intensity * light.color;


	// Not sure this is correct, limits each channel to [0,1]
	return glm::clamp(result,0,1);
}




#endif
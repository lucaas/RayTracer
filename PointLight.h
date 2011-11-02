#ifndef POINT_LIGHT
#define POINT_LIGHT

#include "glm.hpp"

class PointLight
{
public:
	PointLight(glm::vec3 position,glm::vec3 color,float intensity):position(position),color(color),intensity(intensity)	{}

	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};


#endif
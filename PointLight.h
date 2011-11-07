#ifndef POINT_LIGHT
#define POINT_LIGHT

#include "Vector3.h"

class PointLight
{
public:
	PointLight(cbh::vec3 position,cbh::vec3 color,float intensity):position(position),color(color),intensity(intensity)	{}

	cbh::vec3 position;
	cbh::vec3 color;
	float intensity;
};


#endif
#ifndef RAY_H
#define RAY_H

#include "glm.hpp"

class Ray
{
public:
	Ray() : direction(0), origin(0), depth(0) {}
	glm::vec3 direction;
	glm::vec3 origin;	
	unsigned int depth;
private:
};


#endif
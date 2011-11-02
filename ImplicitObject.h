#ifndef IMPLICIT_OBJECT
#define IMPLICIT_OBJECT

#include "Ray.h"
#include "glm.hpp"
#include "Material.h"

class ImplicitObject
{
public:
	virtual bool intersects(const Ray &ray, float &t) const = 0;
	virtual glm::vec3 getNormal(glm::vec3 intersection) const = 0;
	virtual Material getMaterial() const = 0;
private:
	Material material;
};



#endif
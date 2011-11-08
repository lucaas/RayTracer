#ifndef IMPLICIT_OBJECT
#define IMPLICIT_OBJECT

#include "Ray.h"
#include "SimpleMaterial.h"
#include "Vector3.h"

class ImplicitObject
{
public:
	virtual ~ImplicitObject() { delete material; }
	virtual bool intersects(Ray & ray) const = 0;
	virtual cbh::vec3 getNormal(cbh::vec3 intersection) const = 0;
	virtual const SimpleMaterial & getMaterial() const = 0;
	void setMaterial(SimpleMaterial * mat) { material = mat; };

protected:
	SimpleMaterial * material;
};



#endif
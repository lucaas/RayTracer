#ifndef IMPLICIT_OBJECT
#define IMPLICIT_OBJECT

#include "Ray.h"
#include "SimpleMaterial.h"
#include "Vector3.h"
#include "IMaterial.h"

class ImplicitObject
{
public:
	ImplicitObject() { material = NULL; }
	
	~ImplicitObject() { 
		if (material != NULL) 
			delete material; 
	}

	// Interface
	virtual bool intersects(Ray & ray) const = 0;
	virtual cbh::vec3 getNormal(cbh::vec3 intersection) const = 0;
	virtual cbh::vec3 getPosition() const = 0;
	virtual cbh::vec3 getRandomPosition() = 0;

	// Materials
	virtual const IMaterial & getMaterial() const {	return *material; }
	virtual void setMaterial(IMaterial * mat) { material = mat; }

protected:
	IMaterial * material;
};



#endif
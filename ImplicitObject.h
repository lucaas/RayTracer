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

	virtual bool intersects(Ray & ray) const = 0;
	virtual cbh::vec3 getNormal(cbh::vec3 intersection) const = 0;
	virtual const IMaterial & getMaterial() const = 0;
	void setMaterial(IMaterial * mat) { material = mat; };

protected:
	IMaterial * material;
};



#endif
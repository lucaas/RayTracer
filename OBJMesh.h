#ifndef OBJ_MESH
#define OBJ_MESH

#include <vector>

#include "Ray.h"
#include "SimpleMaterial.h"
#include "Vector3.h"
#include "ImplicitObject.h"
#include "ImplicitSphere.h"

class OBJMesh : public ImplicitObject
{
private:
	enum ProcessingType 
	{ 
		OTHER = 0,
		VERTEX, 
		NORMAL,
		FACE,
		TEXTURE_COORD,
	};
	
	struct Face {
		unsigned int v0, v1, v2; // indices to vertices
		unsigned int n0, n1, n2; // indices to normals
	};
	
	std::vector<cbh::vec3> vertices;
	std::vector<Face> faces;
	std::vector<cbh::vec3> normals;
    
    cbh::vec3 position;
    ImplicitSphere *boundingSphere;
    
    
    bool intersectsFace(Ray &ray, const Face &face, double &t, cbh::vec3 &normal) const;
    
public:
	OBJMesh(char *file);
    OBJMesh(char *file, cbh::vec3 pos);
    
	virtual bool intersects(Ray & ray) const;
	virtual cbh::vec3 getNormal(cbh::vec3 intersection) const;
	virtual cbh::vec3 getPosition() const;
};



#endif
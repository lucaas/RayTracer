#include "OBJMesh.h"
#include <fstream>
#include <string>
#include <sstream>


OBJMesh::OBJMesh(char *file) {
    OBJMesh(file, cbh::vec3(0));
}


// Constructor, loads the file
OBJMesh::OBJMesh(char *file, cbh::vec3 pos) {
    
    position = pos;
    
    
	// Open file
	std::ifstream input(file);
	if( !input.is_open() ) {
		std::cerr << "ERROR, could not open OBJ file: " << file << std::endl;
		exit(45);
	}
    
	ProcessingType processing;
	std::string buffer;
	std::string token;
    
    double maxDistSquared = 0, distSquared;
    
	while( getline(input, buffer) ) {
		processing = OTHER;
		// Get line type
		std::stringstream ss(buffer);
		ss >> token;
        
		if (token == "v") {
			processing = VERTEX;
		}
		else if (token == "vn") {
			processing = NORMAL;
		}		
		else if (token == "f") {
			processing = FACE;
		}
		
		switch (processing) {
			case VERTEX:
                
				double x,y,z;
				ss >> x;
				ss >> y;
				ss >> z;
                
                distSquared = x*x + y*y + z*z;
                if (distSquared > maxDistSquared)
                    maxDistSquared = distSquared;
                
				vertices.push_back(cbh::vec3(x,y,z));
                
				break;
			case NORMAL:
                
				double nx,ny,nz;
				ss >> nx;
				ss >> ny;
				ss >> nz;
				normals.push_back(cbh::vec3(nx,ny,nz));
                
				break;
                
			case FACE:
                
				Face face;
                
				ss >> face.v0;
                ss.ignore(128,'/');
                ss.ignore(128,'/');
                ss >> face.n0;
                
				ss >> face.v1;
                ss.ignore(128,'/');
                ss.ignore(128,'/');
                ss >> face.n1;
                
                ss >> face.v2;
                ss.ignore(128,'/');
                ss.ignore(128,'/');
                ss >> face.n2;
                
                face.v0--;
                face.v1--;
                face.v2--;
                face.n0--;
                face.n1--;
                face.n2--;
				faces.push_back(face);
                
				break;
                
			default:
				break;
		}
	}
    
	boundingSphere = new ImplicitSphere(sqrt(maxDistSquared), position);
    
	std::cout << "Finished reading " << file << std::endl;
	std::cout << vertices.size() << " vertices." << std::endl;
	std::cout << faces.size() << " faces." <<  std::endl;
	/*
     for (std::vector<Face>::const_iterator it = faces.begin(); it != faces.end(); ++it) {
     std::cout << "face: " << (*it).v0 << " (" << vertices.at((*it).v0).getX() << ") ";
     std::cout << ", " << (*it).v1 << " (" << vertices.at((*it).v1).getX() << ") ";
     std::cout << ", " << (*it).v2 << " (" << vertices.at((*it).v2).getX() << ") " << std::endl;
     }
     std::cout << std::endl;
     */
}

bool OBJMesh::intersectsFace(Ray &ray, const Face &face, double &t, cbh::vec3 &normal) const {
    static const double epsilon = 0.000001;
    
    
    // Triangle edges
    cbh::vec3 e1 = vertices.at(face.v1) - vertices.at(face.v0);
    cbh::vec3 e2 = vertices.at(face.v2) - vertices.at(face.v0);
    
    // calulate detrimnant
    cbh::vec3 pvec = ray.direction.cross(e2);
    double det = e1.dot(pvec);
    
    // avoid small determinants
    if (det > -epsilon && det < epsilon) 
        return false;
    double inv_det = 1.0 / det;
    
    // origin <-> tri distance
    cbh::vec3 tvec = ray.origin - (position + vertices.at(face.v0));
    
    // U parameter test
    double u =	tvec.dot(pvec) * inv_det;
    if (u < 0.0  || u > 1.0)
        return false;
    
    // V parameter test
    cbh::vec3 qvec = tvec.cross(e1);
    double v = ray.direction.dot(qvec) * inv_det;
    if (v < 0.0 || (u + v) > 1.0)
        return false;
    
    t = e2.dot(qvec) * inv_det;
    
    // trilinear interpolate normal
    normal = (1-u-v)*normals.at(face.n0);
    normal += u*normals.at(face.n1);
    normal += v*normals.at(face.n2);
    
    return true;
}

// Only Front-face intersection
// algorithm from the paper Fast Minimum Storage RayTriangle Intersection
// http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
bool OBJMesh::intersects(Ray & ray) const {
    
    if (boundingSphere->intersects(ray) == false)
        return false;
    
	double tMin = 10e10;
    double t = 0;
    cbh::vec3 normal, normalMin;
	bool result = false;
    
	for (unsigned int i = 0; i < faces.size(); ++i) {
		
		if (intersectsFace(ray, faces.at(i), t, normal)) {
            if (t > 0 && t < tMin) {
                result = true;
                tMin = t;
                normalMin = normal.normalize();
            }
        }
	}
	
	if (result) {
		ray.t = tMin;
        ray.normal = normalMin.normalize();
		
	}
    
	return result;
}

cbh::vec3 OBJMesh::getPosition() const {
	return position;
}

cbh::vec3 OBJMesh::getNormal(cbh::vec3 intersection) const {
	return (intersection - position).normalize();
}

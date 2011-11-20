#include "OBJMesh.h"
#include <fstream>
#include <string>
#include <sstream>
// Constructor, loads the file
OBJMesh::OBJMesh(char *file) {

	// Open file
	std::ifstream input(file);
	if( !input.is_open() ) {
		std::cerr << "ERROR, could not open OBJ file: " << file << std::endl;
		exit(45);
	}

	ProcessingType processing;
	std::string buffer;
	std::string token;

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
				ss >> face.v1;
				ss >> face.v2;
				faces.push_back(face);

				break;

			default:
				break;
		}
	}

	
	std::cout << "Finished reading " << file << std::endl;
	std::cout << vertices.size() << " vertices." << std::endl;
	std::cout << faces.size() << " faces." <<  std::endl;
	/*
	for (std::vector<Face>::const_iterator it = faces.begin(); it != faces.end(); ++it) {
			std::cout << "face: " << (*it).v0 << ", " << (*it).v1 << ", " << (*it).v2 << ". " << std::endl;
	}*/
	std::cout << std::endl;
}

// Only Front-face intersection
// algorithm from the paper Fast Minimum Storage RayTriangle Intersection
// http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
bool OBJMesh::intersects(Ray & ray) const {
	static const double epsilon = 10e-5;

	double tMin = 10e10;
	bool result = false;

	for (std::vector<Face>::const_iterator it = faces.begin(); it != faces.end(); ++it) {
		
		// Triangle edges
		cbh::vec3 e1 = vertices.at((*it).v1) - vertices.at((*it).v0);
		cbh::vec3 e2 = vertices.at((*it).v2) - vertices.at((*it).v0);

		// calulate detrimnant
		cbh::vec3 pvec = ray.direction.cross(e2);
		double det = e1.dot(pvec);
		
		// avoid small determinants
		if (det > -epsilon && det < epsilon) 
			continue;

		// origin <-> tri distance
		cbh::vec3 tvec = ray.origin - vertices.at((*it).v0);

		// U parameter test
		double u =	tvec.dot(pvec);
		if (u < 0.0  || u > det)
			continue;

		// V parameter test
		cbh::vec3 qvec = tvec.cross(e1);
		double v = ray.direction.dot(qvec);
		if (v < 0.0 || (u + v) > det)
			continue;

		double inv_det = 1 / det;
		double t = e2.dot(qvec) * inv_det;

		if (t > 0 && t < tMin) {
			result = true;
			tMin = t;
		}
	}
	
	if (result) {
		ray.t = tMin;
		
	}

	return result;
}


cbh::vec3 OBJMesh::getNormal(cbh::vec3 intersection) const {
	cbh::vec3 result = intersection - cbh::vec3(0);
	return result.normalize();
}

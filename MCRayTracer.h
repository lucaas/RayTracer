#ifndef RayTracer_MCRayTracer_h
#define RayTracer_MCRayTracer_h

#include "Vector3.h"
#include "Scene.h"
#include "Scene.h"
#include "Img.h"
#include "Ray.h"

class MCRayTracer {
private:
	cbh::vec3 trace(Ray &ray);
	cbh::vec3 computeRadiance(Ray &ray);
	cbh::vec3 directIllumination(Ray &ray);
	cbh::vec3 indirectIllumination(Ray &ray);
	double radianceTransfer(cbh::vec3 &p1, cbh::vec3 &p2);
	cbh::vec3 getIntersection(Ray &ray);
	cbh::vec3 sampleHemisphere(const cbh::vec3 & normal, double & pdf);
	
	unsigned int raysPerPixel, maxDepth, indirectPaths, shadowRays;
	Scene *scene;
	Img *image;

public:
	MCRayTracer() : raysPerPixel(1), maxDepth(1), indirectPaths(1) { }
	MCRayTracer(unsigned int raysPerPixel,unsigned int maxDepth, unsigned int indirectPaths, unsigned int shadowRays) 
		: raysPerPixel(raysPerPixel), maxDepth(maxDepth), indirectPaths(indirectPaths), shadowRays(shadowRays) { }

	inline void setScene(Scene * _scene) { scene = _scene; }
	inline void setImage(Img * _image) { image = _image; } 

	void render();

};


#endif

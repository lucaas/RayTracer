#ifndef RayTracer_MCRayTracer_h
#define RayTracer_MCRayTracer_h

#include "Vector3.h"
#include "Scene.h"
#include "Scene.h"
#include "Img.h"
#include "Ray.h"
#include "OpenGLViewer.h"

class MCRayTacer {
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
	OpenGLViewer *viewer;

public:
	MCRayTacer() : raysPerPixel(1), maxDepth(1), indirectPaths(1) { }
	MCRayTacer(unsigned int raysPerPixel,unsigned int maxDepth, unsigned int indirectPaths, unsigned int shadowRays) 
		: raysPerPixel(raysPerPixel), maxDepth(maxDepth), indirectPaths(indirectPaths), shadowRays(shadowRays) { }

	inline void setScene(Scene * _scene) { scene = _scene; }
	inline void setImage(Img * _image) { image = _image; }
	inline void setViewer(OpenGLViewer * _viewer) { viewer = _viewer; } 

	void render();

};


#endif

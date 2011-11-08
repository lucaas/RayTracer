#ifndef RayTracer_MCRayTracer_h
#define RayTracer_MCRayTracer_h

#include "Vector3.h"
#include "Scene.h"
#include "Img.h"

class Ray;

class MCRayTacer {
private:
	cbh::vec3 trace(Ray &ray);
	cbh::vec3 computeRadiance(Ray &ray);
	cbh::vec3 directIllumination(Ray &ray);
	cbh::vec3 indirectIllumination(Ray &ray);
	double radianceTransfer(cbh::vec3 p1, cbh::vec3 p2);
	cbh::vec3 getIntersection(Ray &ray);
	cbh::vec3 sampleHemisphere(const cbh::vec3 & normal);

	unsigned int raysPerPixel, maxReflectionRays, maxRefractionRays, indirectPaths;
	Scene *scene;
	Img *image;

public:
	MCRayTacer() : raysPerPixel(1), maxReflectionRays(1), maxRefractionRays(1) { }
	MCRayTacer(unsigned int raysPerPixel,unsigned int maxReflectionRays, unsigned int maxRefractionRays) 
		: raysPerPixel(raysPerPixel), maxReflectionRays(maxReflectionRays), maxRefractionRays(maxRefractionRays) { }

	void setScene(Scene * _scene);
	void setImage(Img * _image);

	void render();

};


#endif

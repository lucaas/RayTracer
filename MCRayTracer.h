#ifndef RayTracer_MCRayTracer_h
#define RayTracer_MCRayTracer_h

#include "Vector3.h"
#include "Scene.h"
#include "Scene.h"
#include "Img.h"
#include "Ray.h"
#include "OpenGLViewer.h"
#include "PhotonMap.h"

class MCRayTracer {
private:
	cbh::vec3 trace(Ray &ray, bool intersectLights = false);
	cbh::vec3 refractTrace(Ray &ray);
	cbh::vec3 computeRadiance(Ray &ray);
	cbh::vec3 directIllumination(Ray &ray);
	cbh::vec3 indirectIllumination(Ray &ray);

	//incoming points toward the surface, normal points away from surface
	double fresnel(const cbh::vec3 & incoming,const cbh::vec3 & normal,const double & nTo,const double & nFrom );

	double radianceTransfer(cbh::vec3 &p1, cbh::vec3 &p2);
	cbh::vec3 getIntersection(Ray &ray, bool intersectLights = false);
	cbh::vec3 sampleHemisphere(const cbh::vec3 & normal, double & pdf);
	
	unsigned int raysPerPixel, maxDiffuseBounces,maxReflections,maxRefractions, indirectPaths, shadowRays;
	Scene *scene;
	Img *image;
	OpenGLViewer *viewer;



public:
	void setMaxReflection(unsigned int _maxReflections) { maxReflections = _maxReflections; }
	void setMaxRefractions(unsigned int _maxRefractions) { maxRefractions = _maxRefractions; }
	void setMaxDiffuseBounces(unsigned int _maxDiffuseBounces) { maxDiffuseBounces = _maxDiffuseBounces; }
	void setFinalGatherRays(unsigned int fgRays) { indirectPaths = fgRays; }
	void setShadowRays(unsigned int _shadowRays) { shadowRays = _shadowRays; }
	void setSamplesPerPixel(unsigned int pps) { raysPerPixel = pps; }

	inline void setScene(Scene * _scene) { scene = _scene; }
	inline void setImage(Img * _image) { image = _image; }
	inline void setViewer(OpenGLViewer * _viewer) { viewer = _viewer; } 

	void render();

	//********************************
	// Photon mapping
	//********************************
	Photon_map globalPhotonMap;
	Photon_map causticPhotonMap;
	Photon_map *currentPhotonMap;
	int maxPhotonDepth;
	void generate_photon_map(int nPhotons, int _maxPhotonDepth );
	void generate_caustic_map(int nPhotons, int _maxPhotonDepth );
	void trace_photon(Ray & ray, cbh::vec3 power);
	void trace_caustic_photon(Ray & ray, cbh::vec3 power);
	void render_photon_map();

};


#endif

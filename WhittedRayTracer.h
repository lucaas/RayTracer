#ifndef WHITTED_RAY_TRACER
#define WHITTED_RAY_TRACER

#include "glm.hpp"
#include "Scene.h"
#include "Img.h"
#include "ColorRGB.h"

class WhittedRayTracer
{
public:
	WhittedRayTracer();
	~WhittedRayTracer();
	WhittedRayTracer(unsigned int raysPerPixel,unsigned int maxReflectionRays, unsigned int maxRefractionRays);
	void setScene(Scene *);
	void setImage(Img * );
	glm::vec3 trace(Ray & ray);
	void render();
	
private:
    bool getIntersection(ImplicitObject *&hitObject, float &t, const Ray &ray);
    glm::vec3 WhittedRayTracer::directIllumination(ImplicitObject *&surfObject, const glm::vec3 &surfPoint);
    
	unsigned int raysPerPixel,maxReflectionRays,maxRefractionRays;
	Scene * scene;
	Img * image;

};



#endif
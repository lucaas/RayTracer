#ifndef WHITTED_RAY_TRACER
#define WHITTED_RAY_TRACER


#include "Scene.h"
#include "Img.h"
#include "Vector3.h"
#include "ImplicitObject.h"
#include "Ray.h"

class WhittedRayTracer
{
public:
	WhittedRayTracer();
	~WhittedRayTracer();
	WhittedRayTracer(unsigned int raysPerPixel,unsigned int maxReflectionRays, unsigned int maxRefractionRays);
	void setScene(Scene * _scene);
	void setImage(Img * _image);
	cbh::vec3 trace(Ray &ray);
	void render();

private:
	bool getIntersection(ImplicitObject *&hitObject, float &t, const Ray &ray);
	cbh::vec3 directIllumination(ImplicitObject *&surfObject, const cbh::vec3 &surfPoint);

	unsigned int raysPerPixel,maxReflectionRays,maxRefractionRays;
	Scene * scene;
	Img * image;

};



#endif
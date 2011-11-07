

#include "WhittedRayTracer.h"
#include "Utility.h"

WhittedRayTracer::WhittedRayTracer()
	: raysPerPixel(1), maxReflectionRays(1), maxRefractionRays(1)
{

}

WhittedRayTracer::WhittedRayTracer(unsigned int raysPerPixel,unsigned int maxReflectionRays, unsigned int maxRefractionRays) 
	: raysPerPixel(raysPerPixel), maxReflectionRays(maxReflectionRays), maxRefractionRays(maxRefractionRays)
{

}

WhittedRayTracer::~WhittedRayTracer()
{

}

void WhittedRayTracer::setScene(Scene * _scene)
{
	scene = _scene;
}

void WhittedRayTracer::setImage(Img * _image)
{
	image = _image;
}

cbh::vec3 WhittedRayTracer::directIllumination(ImplicitObject *&surfObject, const cbh::vec3 &surfPoint) {

	cbh::vec3 color(0);
	Ray shadowRay;
	shadowRay.origin = surfPoint;
	ImplicitObject *hitObject = NULL;
	bool isShadowed = false;
	float t = 0;
	float tlight = 0;
	for(int j = 0; j < scene->getNumLights(); ++j)
	{
		isShadowed = false;
		shadowRay.direction = (scene->getLight(j)->position - surfPoint);
		tlight = sqrt(shadowRay.direction.squareNorm());
		shadowRay.direction = shadowRay.direction.normalize();
		shadowRay.origin = shadowRay.origin + shadowRay.direction*10e-4;
		for(int i = 0; i < scene->getNumImplicitObjects(); ++i) 
		{
			if (getIntersection(hitObject, t, shadowRay) && t < tlight) 
				isShadowed = true;
		}
		
		if (!isShadowed)
			color += phong(surfObject->getMaterial(), surfPoint, *(scene->getLight(j)), (scene->getCam()->getPosition() - surfPoint).normalize(), surfObject->getNormal(surfPoint));

	}

	return color;

}

bool WhittedRayTracer::getIntersection(ImplicitObject *&hitObject, float &t, const Ray &ray) {
	t = 0;
	static float tHit; tHit = 100000;
	hitObject = NULL;

	for(int i = 0; i < scene->getNumImplicitObjects(); ++i)
	{
		ImplicitObject *object = scene->getImplicitObject(i);
		if(object->intersects(ray,t))
		{
			//Save the closest t
			if(t < tHit)
			{
				tHit = t;
				hitObject = object;
			}
		}
	}
	t = tHit;
	return (hitObject != NULL);
}

cbh::vec3 WhittedRayTracer::trace(Ray &ray)
{
	static float t; t = 0;
	static ImplicitObject * hitObject; hitObject = 0;


	if(getIntersection(hitObject, t, ray) == false)
		return cbh::vec3(0);
	else if(ray.depth >= maxReflectionRays || hitObject->getMaterial().reflection <= 10e-6)
	{
		cbh::vec3 position = ray.origin + t * ray.direction;
		return directIllumination(hitObject, position);

	}
	else // hit == true && ray.depth < maxReflectionRays, Only for specular objects
	{
		double epsilon = 10e-4*t;
		cbh::vec3 position = ray.origin + t * ray.direction;
		cbh::vec3 color = directIllumination(hitObject, position);

		++ray.depth;
		//Spawn new ray in perfect reflection direction
		cbh::vec3 N = hitObject->getNormal(position);


		//Perfect reflection
		float reflet = 2.0f * ray.direction.dot(N);
		ray.direction = (ray.direction - reflet * N).normalize();
		ray.origin = position + epsilon * ray.direction;

		//ray.direction = (2*N.dot(-ray.direction)*N + ray.direction)).normalize();

		float reflection = hitObject->getMaterial().reflection;

		cbh::vec3 reflectionColor = trace(ray);
		
		return (1-reflection)*color + reflection*reflectionColor;

	}	

}


void WhittedRayTracer::render()
{
	Ray ray;
	float t = 0;
	for (int x = 0; x < image->width; ++x) 
	{
		for (int y = 0; y < image->height; ++y) 
		{
			ray.depth = 0;
			ray.origin = scene->getCam()->getPosition();

			cbh::vec3 dy = scene->getCam()->getPixelDy();
			cbh::vec3 dx = scene->getCam()->getPixelDx();
			dy = dy*y;
			dx = dx*x;

			ray.direction = (scene->getCam()->getImagePlaneBL() + dx + dy - ray.origin).normalize();

			(*image)(x,y) = float2rgb(trace(ray));
		}
	}
	image->Save();
}
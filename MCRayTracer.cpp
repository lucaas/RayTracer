#include <ctime>
#include "MCRayTracer.h"

//Return a normalized direction on a hemisphere defined on the plane with the passed normal
cbh::vec3 sampleHemisphere(const cbh::vec3 & normal)
{
	//Här bör vi läsa på om Cosine weighted hemisphere sampling. Boken s.137


	cbh::vec3 direction(0);
	static const double PI = 3.14159265;
	double psi = 2.0 * PI * ( (double)rand() / (double)RAND_MAX ); //azimuth angle
	double theta = PI * ( (double)rand() / (double)RAND_MAX ) ; //inclination angle

	//Assumes normal parallel to positive Z-axis
	direction.setX(cos(psi)*sin(theta));
	direction.setY(sin(psi)*sin(theta));
	direction.setZ(cos(theta));
	


	return direction.normalize();
}

//Find the intersection point of the ray and stores the object that it hit in the ray.
cbh::vec3 MCRayTacer::getIntersection(Ray &ray) {
	ray.t = 0;
	static double tHit; tHit = 100000;
	ray.currentObject = NULL;
	for(unsigned int i = 0; i < scene->getNumImplicitObjects(); ++i)
	{
		ImplicitObject *object = scene->getImplicitObject(i);
		if(object->intersects(ray))
		{
			//Save the closest t
			if(ray.t < tHit)
			{
				tHit = ray.t;
				ray.currentObject = object;
			}
		}
	}
	ray.t = tHit;
	if (ray.currentObject != NULL)
		return ray.origin + ray.t * ray.direction;
	else
		return NULL;
}


//Returns the first hit with the scene
cbh::vec3 MCRayTacer::trace(Ray &ray) {
	cbh::vec3 position(getIntersection(ray));
	ray.origin = position;
	// @todo: Check if object is a light source?
	return computeRadiance(ray);
}

//Recursive sampling of radiance
cbh::vec3 MCRayTacer::computeRadiance(Ray &ray)
{
	cbh::vec3 radiance(0);
	radiance += directIllumination(ray);
	radiance += indirectIllumination(ray);
	return radiance;
}


//Samples light sources with shadow rays
cbh::vec3 directIllumination(Ray &ray)
{
	cbh::vec3 radiance(0);






	return radiance;
}


//Samples indirect illumination by sampling the hemisphere
cbh::vec3 indirectIllumination(Ray &ray)
{
	cbh::vec3 radiance(0);

	//Let russian roulette decide wheater the ray gets absorbed or scattered

	//For all indirect paths define and sample hemisphere

	//For all paths trace that ray to get a new position(object)

	//For each point

	// radiance += cumputeRadiance(Ray) * object.BRDF * cos(Nx,psi) / pdf(psi)

	//normalize radiance -> radiance / Numpaths
	//unibas the russian roulette op. -> radiance = radiance / (1-absorption)

	return radiance;
}


void MCRayTacer::render()
{

	srand(time(NULL));

	Ray ray;
	for (unsigned int x = 0; x < image->width; ++x) 
	{
		for (unsigned int y = 0; y < image->height; ++y) 
		{
			cbh::vec3 radiance(0);

			// TODO: H = integral(h(p)) ?

			for (unsigned int k = 0; k < raysPerPixel; ++k) {
				ray.depth = 0;
				ray.origin = scene->getCam()->getPosition();

				cbh::vec3 dy = scene->getCam()->getPixelDy();
				cbh::vec3 dx = scene->getCam()->getPixelDx();
				dy = dy*y;
				dx = dx*x;

				// TODO: Jitter the direction slightly

				ray.direction = (scene->getCam()->getImagePlaneBL() + dx + dy - ray.origin).normalize();

				radiance += trace(ray);
			}

			radiance = radiance / raysPerPixel;
			
			radiance.clamp(0,1);
			(*image)(x,y) = cbh::vec3uc((unsigned char)(255*radiance.getX()),(unsigned char)(255*radiance.getY()),(unsigned char)(255*radiance.getZ()));
		}
	}
	image->Save();
}

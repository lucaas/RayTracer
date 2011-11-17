#define _USE_MATH_DEFINES
#include <ctime>
#include <cmath>
#include "omp.h"
#include "MCRayTracer.h"
#include "Vector3.h"
#include "Ray.h"
#include "ImplicitObject.h"


enum RayCase 
{ 
	DIFFUSE = 0,
	SPECULAR, 
	REFRACTION 
};


//Return a normalized direction on a hemisphere defined on the plane with the passed normal
//Sampling of hemisohere defined isdefined in the material
cbh::vec3 MCRayTracer::sampleHemisphere(const cbh::vec3 & normal, double & pdf)
{
	
		
		return cbh::vec3(0);
}

//Find the intersection point of the ray and stores the object that it hit in the ray.
cbh::vec3 MCRayTracer::getIntersection(Ray &ray) {
	ray.t = 0;
	double tHit; tHit = 100000;
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

cbh::vec3 MCRayTracer::refractTrace(Ray &ray)
{
	double c1,c2,n;
	do
	{
		ray.origin += ray.getOffset();
		if((ray.origin = getIntersection(ray)) == NULL)
			return cbh::vec3(0);

		n = 1/ray.currentObject->getMaterial().rIndex; // n = n1/n2 = 1/n2;
		c1 = ray.currentObject->getNormal(ray.origin).dot(ray.direction);

		c2 = 1 - (n*n*(1-c1*c1));
		if(c2 > 0)
		{
			ray.direction = n * ray.direction + ray.currentObject->getNormal(ray.origin) * (n*c1 - sqrt(c2));
			break;
		}
		else
			ray.direction =cbh::reflect(ray.direction, ray.currentObject->getNormal(ray.origin));


	} while (c2 < 0);

	return trace(ray);
}

//Returns the color from the intersection with the scene
cbh::vec3 MCRayTracer::trace(Ray &ray) {
	
	// offset the ray to avoid numeric errors
	ray.origin += ray.getOffset();
	
	// get new intersection
	cbh::vec3 position(getIntersection(ray));
	if (position == NULL)
		return cbh::vec3(0);
	
	ray.origin = position;

	// @todo: Check if object is a light source?
	return computeRadiance(ray);
}



//Recursive sampling of radiance
cbh::vec3 MCRayTracer::computeRadiance(Ray &ray)
{
	cbh::vec3 radiance(0);	
	radiance += directIllumination(ray);    
	radiance += indirectIllumination(ray);
		
	return radiance;
}

double MCRayTracer::radianceTransfer(cbh::vec3 &p1, cbh::vec3 &p2)
{
	Ray ray;
	ray.origin = p1;

	// Attenuation factor in V(x,y)
	double distance = (p2 - p1).squareNorm();
	double attenuation = 2.0/sqrt(distance); // this should make it reach circa 2 units

	ray.direction = (p2 - p1).normalize();
	double t = ray.t = sqrt((p2 - p1).squareNorm());
	ray.origin += ray.getOffset();
	
	getIntersection(ray);

	if(ray.t > t - 10e-4)
		return attenuation;
	else
		return 0.0;


}


//Samples light sources with shadow rays
cbh::vec3 MCRayTracer::directIllumination(Ray &ray)
{
	cbh::vec3 radiance(0);

	// Sample each light
	for(unsigned int j = 0; j < scene->getNumLights(); ++j)
	{
		// Shoot shadowrays
		for (unsigned int k = 0; k < shadowRays; ++k) {

			// Sample positions randomly on the light's volume
			cbh::vec3 lightPos = scene->getLight(j)->getRandomPosition();
			cbh::vec3 lightDir = (lightPos - ray.origin).normalize();

			// Add diffuse color to randiance
			double costerm = ray.currentObject->getNormal(ray.origin).dot(lightDir);
			costerm = costerm < 0 ? 0 : costerm;
			radiance += costerm * ray.currentObject->getMaterial().color * radianceTransfer(ray.origin, lightPos);
		}
	}
	return radiance/shadowRays;
}


//Samples indirect illumination by sampling the hemisphere
cbh::vec3 MCRayTracer::indirectIllumination(Ray &ray)
{
	cbh::vec3 radiance(0);
	if (ray.depth >= maxDepth)
		return radiance;

	//Let russian roulette decide wheater the ray gets absorbed or scattered
	double r = (double)rand() / ((double)RAND_MAX + 1);

	//Ray is absorbed
	if(r > 0.8)
		return radiance;

	ImplicitObject *object = ray.currentObject;
	cbh::vec3 normal = object->getNormal(ray.origin);
	bool refract = false;
	double pdf = 0;

	int Case = -1; // Not set;
	//Determine what happens to ray
	if(r < object->getMaterial().kd)
		Case = DIFFUSE;
	else if(r < object->getMaterial().kd + object->getMaterial().ks)
		Case = SPECULAR;
	else
		Case = REFRACTION;

	if(Case == DIFFUSE)
	{
		for (unsigned int i = 0; i < indirectPaths; ++i) 
		{
			Ray newRay(ray);
			newRay.depth++;
			newRay.direction = object->getMaterial().sampleHemisphere(normal, pdf);
			radiance += trace(newRay).mtimes(object->getMaterial().brdf(newRay.direction, newRay.direction)) * normal.dot(newRay.direction)*(1/pdf);
		}
		//normalize radiance -> radiance / Numpaths
		radiance = radiance / indirectPaths;
	}
	else if(Case == SPECULAR)
	{
		for (unsigned int i = 0; i < indirectPaths; ++i) 
		{
			Ray newRay(ray);
			newRay.depth++;
			cbh::vec3 perfectReflection = cbh::reflect(ray.direction,normal);
			newRay.direction = object->getMaterial().sampleHemisphere(perfectReflection, pdf);

			if(acos(newRay.direction.dot(normal)) > M_PI/2)
				newRay.direction = perfectReflection;

			radiance += trace(newRay).mtimes(object->getMaterial().brdf(perfectReflection, newRay.direction));

		}
		//normalize radiance -> radiance / Numpaths
		radiance = radiance / indirectPaths;
	}
	else //CASE == REFRACTION
	{
		Ray newRay(ray);
		double n,n2(ray.currentObject->getMaterial().rIndex);
		double c1 = ray.currentObject->getNormal(ray.origin).dot(ray.direction);

		n = 1/n2;

		double c2 = 1 - (n*n*(1-c1*c1));
		newRay.direction = n * ray.direction + normal * (n*c1 - sqrt(c2));
		radiance = refractTrace(newRay);
	}
	
	//Normalize Russian Roulette
	radiance = radiance / 0.8;
	return radiance;
}


void MCRayTracer::render()
{
	std::cout << "Rendering..." << std::endl;

	clock_t start;
	double diff;
	start = clock();
	


	srand(time(NULL));


	cbh::vec3 camPos(scene->getCam()->getPosition());
	cbh::vec3 pixelDy(scene->getCam()->getPixelDy());
	cbh::vec3 pixelDx(scene->getCam()->getPixelDx());
	
	cbh::vec3 radiance(0);


	// Tiles
	int numTiles = 8;
	int pixelsPerTile = image->width / numTiles; 
	
	// Check so that image width is devisable by number of tiles
	if ((image->width / (float)numTiles) - (image->width / numTiles) > 0) {
		std::cerr << "ERROR: image->width / numTiles must be an integer" << std::endl;
		exit(73);
	}

	double raysPerPixelSqrt = sqrt((double)raysPerPixel);
	cbh::vec3 subPixelDx = pixelDx / raysPerPixelSqrt;
	cbh::vec3 subPixelDy = pixelDy / raysPerPixelSqrt;

	// OpenMP, parallelize using tiles to mitigate artifactes caused by:
	// TODO: rand() is not thread safe, which causes artifacts since the same random number is used multiple times
	#pragma omp parallel for schedule(dynamic,1) private(radiance)
	for (int tile = 0; tile < numTiles; ++tile) {

		for (int xt = 0; xt < pixelsPerTile; ++xt) 
		{
			int x = tile * pixelsPerTile + xt;
			for (int y = 0; y < image->height; ++y) 
			{
				radiance = 0;

				// Subpixel sampling
				for (int kx = 0; kx < raysPerPixelSqrt; ++kx) 
				{
					for (int ky = 0; ky < raysPerPixelSqrt; ++ky) 
					{
						double jitterx = (double)rand() / ((double)RAND_MAX);
						double jittery = (double)rand() / ((double)RAND_MAX);

						Ray ray;
						ray.depth = 0;
						ray.origin = camPos;

						cbh::vec3 dx = pixelDx*x + kx * subPixelDx * jitterx;
						cbh::vec3 dy = pixelDy*y + ky * subPixelDy * jittery;

						ray.direction = (scene->getCam()->getImagePlaneBL() +  dx + dy - ray.origin).normalize();

						radiance += trace(ray);
					
					}

				}

				radiance = radiance / raysPerPixel;
				radiance = radiance.clamp(0,1);//radiance.normalizeWithMax();

				(*image)(x,y) = cbh::vec3uc((unsigned char)(255*radiance.getX()),(unsigned char)(255*radiance.getY()),(unsigned char)(255*radiance.getZ()));
			}

			std::cout << "Column: " << x  << " complete." << std::endl;


			// OpenGL is not thread safe and should only be updated by the thread that created it's context
			// Not sure if it is safe to assume that this is thread 0.
			if (omp_get_thread_num() == 0)
				viewer->draw(image);

		}
	}
	image->Save();

	// Update again, since thread 0 might not have been the last to finish
	viewer->draw(image);

	diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
	std::cout <<"Total elapsed time: "<< diff <<'\n';
}




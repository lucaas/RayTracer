#define _USE_MATH_DEFINES
#include <ctime>
#include <cmath>
#include "MCRayTracer.h"
#include "Vector3.h"
#include "Ray.h"
#include "ImplicitObject.h"


//Return a normalized direction on a hemisphere defined on the plane with the passed normal
//Cosine weighted sampling of hemisphere
cbh::vec3 MCRayTacer::sampleHemisphere(const cbh::vec3 & normal, double & pdf)
{
	
	double r1 = (double)rand() / ((double)RAND_MAX + 1);
	double r2 = (double)rand() / ((double)RAND_MAX + 1);
		
	cbh::vec3 d(cos(2.0 * M_PI * r1)*sqrt(1-r2), sin(2.0 * M_PI * r1)*sin(sqrt(1-r2)), sqrt(r2) );

	//
	double theta = -acos(normal.getZ());
	double phi = -atan2(normal.getY(),normal.getX());

	/* Ry * Rz * [x; y; z]
	y*sin(phi) + x*cos(phi)*cos(theta) - z*cos(phi)*sin(theta)
	y*cos(phi) - x*cos(theta)*sin(phi) + z*sin(phi)*sin(theta)
	z*cos(theta) + x*sin(theta)
	*/	
	cbh::vec3 d2(d); //Temporary direction vector;
	d.setX(d2.getY()*sin(phi) + d2.getX()*cos(phi)*cos(theta) - d2.getZ()*cos(phi)*sin(theta));
	d.setY(d2.getY()*cos(phi) - d2.getX()*cos(theta)*sin(phi) + d2.getZ()*sin(phi)*sin(theta));
	d.setZ(d2.getZ()*cos(theta) + d2.getX()*sin(theta));
	
	pdf = sqrt(r2) / M_PI;
	return d.normalize();
}

//Find the intersection point of the ray and stores the object that it hit in the ray.
cbh::vec3 MCRayTacer::getIntersection(Ray &ray) {
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


//Returns the color from the intersection with the scene
cbh::vec3 MCRayTacer::trace(Ray &ray) {
	
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
cbh::vec3 MCRayTacer::computeRadiance(Ray &ray)
{
	cbh::vec3 radiance(0);	
	radiance += directIllumination(ray);    
	radiance += indirectIllumination(ray);
		
	return radiance;
}

double MCRayTacer::radianceTransfer(cbh::vec3 &p1, cbh::vec3 &p2)
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
cbh::vec3 MCRayTacer::directIllumination(Ray &ray)
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
cbh::vec3 MCRayTacer::indirectIllumination(Ray &ray)
{

	cbh::vec3 radiance(0);
	if (ray.depth >= maxDepth)
		return radiance;

	//Let russian roulette decide wheater the ray gets absorbed or scattered
	double r = (double)rand() / ((double)RAND_MAX + 1);
	ImplicitObject *object = ray.currentObject;
	double scatterProb =  object->getMaterial().ks + object->getMaterial().kd;
	cbh::vec3 normal = object->getNormal(ray.origin);
	double pdf = 0;
	// No absortion
	if (r < scatterProb) {
		
		//For all indirect paths define and sample hemisphere
		for (unsigned int i = 0; i < indirectPaths; ++i) {
			
			Ray newRay(ray);
			newRay.depth++;
			newRay.direction = sampleHemisphere(normal, pdf);
			
			//  For all paths trace that ray to get a new position(object)
			// radiance += cumputeRadiance(Ray) * object.BRDF * cos(Normal,outgong direction) / pdf(psi)
			radiance += trace(newRay).mtimes(object->getMaterial().brdf(ray.direction, newRay.direction, normal)) * normal.dot(newRay.direction) / pdf;
			
		}
		
		//normalize radiance -> radiance / Numpaths
		radiance = radiance / indirectPaths;
		
		//unibas the russian roulette op. -> radiance = radiance / (1-absorption)
		radiance = radiance / scatterProb;

	}
	return radiance;
}


void MCRayTacer::render()
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

	//#pragma omp parallel for schedule(dynamic,1) private(radiance)
	for (int x = 0; x < image->width; ++x) 
	{
		for (int y = 0; y < image->height; ++y) 
		{
			radiance = 0;
			for (int k = 0; k < raysPerPixel; ++k) 
			{
				Ray ray;
				ray.depth = 0;
				ray.origin = camPos;

				// TODO: Jitter the direction slightly
				//double r1 = (double)rand() / ((double)RAND_MAX + 1);
				//double r2 = (double)rand() / ((double)RAND_MAX + 1);
				cbh::vec3 dx = pixelDx*x;
				cbh::vec3 dy = pixelDy*y;

				ray.direction = (scene->getCam()->getImagePlaneBL() +  dx + dy - ray.origin).normalize();

				radiance += trace(ray);
			}

			radiance = radiance / raysPerPixel;
			
			radiance = radiance.normalizeWithMax();

			(*image)(x,y) = cbh::vec3uc((unsigned char)(255*radiance.getX()),(unsigned char)(255*radiance.getY()),(unsigned char)(255*radiance.getZ()));
		}

		std::cout << "Column: " << x  << " complete." << std::endl;
	}
	image->Save();


	diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
	std::cout <<"Total elapsed time: "<< diff <<'\n';
}




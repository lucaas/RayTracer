#define _USE_MATH_DEFINES
#include <ctime>
#include <cmath>
#include <stdlib.h>
#include "omp.h"
#include "MCRayTracer.h"
#include "Vector3.h"
#include "Ray.h"
#include "ImplicitObject.h"
#include <random>


enum RayCase 
{ 
	DIFFUSE = 0,
	REFLECT, 
	TRANSMIT 
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

//Assumes incoming pointing towards the surface!
double MCRayTracer::fresnel(const cbh::vec3 & incoming,const cbh::vec3 & normal,const double & nTo,const double & nFrom ) 
{

	double theta = -incoming.dot(normal);

	/*
	if(nFrom > nTo) //Possible TIR (ex: from glass to air)
	{
		double c1 = theta
		//(n = nFrom / nTo) but we always assume air outside of objects -> nTo = 1
		//1 - (n*n*(1-c1*c1));
		double c2 = 1 - (nFrom*nFrom*(1-c1*c1));
		if(c2 < 0) //TIR
		{
			return 1.0;
		}
		else
		{
			double R0 = (nFrom - nTo) / (nTo + nFrom);
			R0 *= R0;
			//c = 1 - (-incoming.dot(normal))
			double c = 1 + incoming.dot(normal); //TECKEN HÄR????????
			return(R0 + (1-R0)*c*c*c*c*c);
		}

	}
	*/

	//e.g from air to glass
	double R0 = (nFrom - nTo) / (nTo + nFrom);
	R0 *= R0;
	double c = 1 - theta;
	return(R0 + (1-R0)*c*c*c*c*c);
}

cbh::vec3 MCRayTracer::refractTrace(Ray &ray)
{
	double c1,c2,n;
	do
	{
		ray.origin += ray.getOffset();
		if((ray.origin = getIntersection(ray)) == NULL)
			return cbh::vec3(0);

		//Normal will point out from the object so we need to flip it!
		cbh::vec3 normal(ray.currentObject->getNormal(ray.origin));
		//n = (nFrom = n1 = object.rIndex) / (nTo = n2 = air = 1) -> n = object.rIndex / 1 = object.rIndex
		n = ray.currentObject->getMaterial().rIndex;
		c1 = normal.dot(ray.direction);

		c2 = 1 - (n*n*(1-c1*c1));
		if(c2 > 0)
		{
			ray.direction = n * ray.direction + normal * (n*c1 - sqrt(c2));
			ray.n = 1;
			break;
		}
		else
			ray.direction = cbh::reflect(ray.direction, -normal);
		


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
	if(ray.currentObject->getMaterial().isLightSource == false)
		return computeRadiance(ray);
	else
		return ray.currentObject->getMaterial().emittance;
}



//Recursive sampling of radiance
cbh::vec3 MCRayTracer::computeRadiance(Ray &ray)
{
	cbh::vec3 radiance(0);	
	//radiance += directIllumination(ray);    
	radiance += indirectIllumination(ray);
	//radiance = radiance.clamp(0,1);
	return radiance;
}


//p1 = position on surface, p2 lightsource position
double MCRayTracer::radianceTransfer(cbh::vec3 &p1, cbh::vec3 &p2)
{
	Ray ray;
	ray.origin = p1;

	// Attenuation factor in V(x,y)
	double distance = sqrt((p2 - p1).squareNorm());
	double attenuation = 1.0/distance; // this should make it reach circa 2 units

	ray.direction = (p2 - p1).normalize();
	//QUICK FIX!! right now we are using the center of the lightsphere so when we check for the intersection we get the surace
	double t = ray.t = distance - 0.6; 
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
	for(unsigned int j = 0; j < scene->getNumImplicitObjects(); ++j)
	{
		ImplicitObject * potentialLight = scene->getImplicitObject(j);
		if(potentialLight->getMaterial().isLightSource == false)
			continue;
		

		// Shoot shadowrays
		for (unsigned int k = 0; k < shadowRays; ++k) {

			// Sample positions randomly on the light's volume
			
			double rad = 0.5;
			double theta = ((double)rand()/(double)(RAND_MAX+1)) * M_PI;
			double phi = ((double)rand()/(double)(RAND_MAX+1)) * 2 * M_PI;

			cbh::vec3 offset(rad*sin(theta)*cos(phi), rad*sin(theta)*sin(phi), rad*cos(theta));
			cbh::vec3 lightPos = scene->getImplicitObject(j)->getPosition() + offset;

			cbh::vec3 lightDir = (lightPos - ray.origin).normalize();
			
			// Add diffuse color to randiance
			double costerm = ray.currentObject->getNormal(ray.origin).dot(lightDir);
			costerm = costerm < 0 ? 0 : costerm;
			radiance += costerm*ray.currentObject->getMaterial().color.mtimes(potentialLight->getMaterial().emittance) * radianceTransfer(ray.origin, lightPos);
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

	ImplicitObject *object = ray.currentObject;
	//Ray is absorbed
	double absorption = object->getMaterial().kd + object->getMaterial().kr + object->getMaterial().kt;
	if(r > absorption)
		return radiance;

	cbh::vec3 normal = object->getNormal(ray.origin);
	bool refract = false;
	double pdf = 0;

	int Case = -1; // Not set;
	//Determine what happens to ray
	if(r < object->getMaterial().kr)
		Case = REFLECT;
	else if(r < object->getMaterial().kr + object->getMaterial().kt)
	{	
		double r2 = (double)rand() / ((double)RAND_MAX + 1);

		double Fresnel = fresnel(ray.direction, normal,object->getMaterial().rIndex,ray.n);
		
		if(r2 < Fresnel)
			Case = REFLECT;
		else
			Case = TRANSMIT;
			
	}
	else
		Case = DIFFUSE;

	if(Case == DIFFUSE)
	{
		for (unsigned int i = 0; i < indirectPaths; ++i) 
		{
			Ray newRay(ray);
			newRay.depth++;
			newRay.direction = object->getMaterial().sampleHemisphere(normal, pdf);
			pdf = pdf < 0.5 ? 0.5 : pdf;
			radiance += trace(newRay).mtimes(object->getMaterial().brdf(newRay.direction, newRay.direction)) * normal.dot(newRay.direction)*(1/pdf);
		}
		//normalize radiance -> radiance / Numpaths
		radiance = radiance / indirectPaths;
	}
	else if(Case == REFLECT)
	{

		Ray newRay(ray);
		newRay.depth++;
		cbh::vec3 perfectReflection = cbh::reflect(ray.direction,normal);
		newRay.direction = perfectReflection;//object->getMaterial().sampleHemisphere(perfectReflection, pdf);

		//if(acos(newRay.direction.dot(normal)) > M_PI/2)
		//	newRay.direction = perfectReflection;

		radiance += trace(newRay).mtimes(object->getMaterial().brdf(perfectReflection, newRay.direction));


		/*for (unsigned int i = 0; i < indirectPaths; ++i) 
		{
			Ray newRay(ray);
			newRay.depth++;
			cbh::vec3 perfectReflection = cbh::reflect(ray.direction,normal);
			newRay.direction = perfectReflection;//object->getMaterial().sampleHemisphere(perfectReflection, pdf);

			//if(acos(newRay.direction.dot(normal)) > M_PI/2)
			//	newRay.direction = perfectReflection;

			radiance += trace(newRay).mtimes(object->getMaterial().brdf(perfectReflection, newRay.direction));

		}
		//normalize radiance -> radiance / Numpaths
		radiance = radiance / indirectPaths;*/
	}
	else //CASE == REFRACTION
	{
		Ray newRay(ray);
		double n,n2(ray.currentObject->getMaterial().rIndex);


		//normal points away! ray.direction points towrds surface! -> flip direction
		//cos(theta)
		double c1 = normal.dot(ray.direction); 
		newRay.n = n2;

		// n = (nFrom = air = 1) / (nTo = object.rIndex) = 1 / object.rIndex
		n = 1/n2;

		// 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 )
		double c2 = 1 - n*n*(1-c1*c1);
		// -(n1/n2)*ray.direction + normal * ((n1/n2)*cos(theta) - sqrt( 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 ) )
		newRay.direction = n * ray.direction + normal * (n*c1 - sqrt(c2));
		radiance = refractTrace(newRay);
	}
	
	//Normalize Russian Roulette
	radiance = radiance / absorption;
	return radiance;
}

inline void toInt(cbh::vec3 & v)
{ 
	v = v.clamp(0,1);
	v.setX(pow(v.getX(),1/2.2)*255+.5);
	v.setY(pow(v.getY(),1/2.2)*255+.5);
	v.setZ(pow(v.getZ(),1/2.2)*255+.5);	
} 


void MCRayTracer::render()
{
	std::cout << "Rendering..." << std::endl;

	clock_t start;
	double diff;
	start = clock();
	

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
	//srand(int(time(NULL)));
	#pragma omp parallel
	{
	#pragma omp for schedule(dynamic,1) private(radiance)
	for (int tile = 0; tile < numTiles; ++tile) {

		srand(int(time(NULL)) ^ omp_get_thread_num());
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

						//Ensures every thread get there own seed
						//srand( int(time(NULL)) ^ omp_get_thread_num() );
						//tr1::seed(double(time(NULL)) ^ omp_get_thread_num() );


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
				//radiance = radiance.clamp(0,1);//radiance.normalizeWithMax();
			//	(*image)(x,y) = cbh::vec3uc((unsigned char)(255*radiance.getX()),(unsigned char)(255*radiance.getY()),(unsigned char)(255*radiance.getZ()));

				//Test från 99 lines of monte carlo med gamma korrigering
				toInt(radiance); 
				(*image)(x,y) = cbh::vec3uc(radiance.getX(),radiance.getY(),radiance.getZ());

			}

			std::cout << "Column: " << x  << " complete." << std::endl;


			// OpenGL is not thread safe and should only be updated by the thread that created it's context
			// Not sure if it is safe to assume that this is thread 0.
			if (omp_get_thread_num() == 0)
				viewer->draw(image);

		}
	}

	} //omp parallel end
	
	image->Save();

	// Update again, since thread 0 might not have been the last to finish
	viewer->draw(image);

	diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
	std::cout <<"Total elapsed time: "<< diff <<'\n';
}




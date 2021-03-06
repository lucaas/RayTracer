#define _USE_MATH_DEFINES
#include <ctime>
#include <cmath>
#include <stdlib.h>
#include "omp.h"
#include "MCRayTracer.h"
#include "Vector3.h"
#include "Ray.h"
#include "ImplicitObject.h"


enum RayCase 
{ 
	DIFFUSE = 0,
	REFLECT, 
	TRANSMIT,
	REFLECT_AND_TRANSMIT
};


//Return a normalized direction on a hemisphere defined on the plane with the passed normal
//Sampling of hemisohere defined isdefined in the material
cbh::vec3 MCRayTracer::sampleHemisphere(const cbh::vec3 & normal, double & pdf)
{
	double r1((double)rand() / ((double)RAND_MAX + 1));
	double r2((double)rand() / ((double)RAND_MAX + 1));

	cbh::vec3 d(cos(2.0 * M_PI * r1)*sqrt(1-r2), sin(2.0 * M_PI * r1)*sqrt(1-r2), sqrt(r2) );

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
cbh::vec3 MCRayTracer::getIntersection(Ray &ray, bool intersectLights) {
	ray.t = 0;
	double tHit; tHit = 100000;
	cbh::vec3 normHit(0,1,0);
	ray.currentObject = NULL;
	for(unsigned int i = 0; i < scene->getNumImplicitObjects(); ++i)
	{
		ImplicitObject *object = scene->getImplicitObject(i);
		
		//Don't check for lights
		if(intersectLights == false && object->getMaterial().isLightSource == true)
			continue;
			
		if(object->intersects(ray))
		{
			//Save the closest t
			if(ray.t < tHit)
			{
				tHit = ray.t;
				normHit = ray.normal;
				ray.currentObject = object;
			}
		}
	}
	ray.t = tHit;
	ray.normal = normHit;
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
	double c = 1 + incoming.dot(normal); //TECKEN H�R????????
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
		ray.Offset();
		if((ray.origin = getIntersection(ray,true)) == NULL)
			return cbh::vec3(0);

		//n = (nFrom = n1 = object.rIndex) / (nTo = n2 = air = 1) -> n = object.rIndex / 1 = object.rIndex
		n = ray.currentObject->getMaterial().rIndex;
		c1 = ray.normal.dot(ray.direction);

		c2 = 1 - (n*n*(1-c1*c1));
		if(c2 > 0)
		{
			ray.direction = n * ray.direction + ray.normal * (n*c1 - sqrt(c2));
			ray.n = 1;
			break;
		}
		else
			ray.direction = cbh::reflect(ray.direction, -ray.normal);



	} while (c2 < 0);

	return trace(ray,true);
}

//Returns the color from the intersection with the scene
cbh::vec3 MCRayTracer::trace(Ray &ray, bool intersectLights) {

	// offset the ray to avoid numeric errors
	ray.Offset();

	// get new intersection
	cbh::vec3 position(getIntersection(ray,intersectLights));
	if (position == NULL)
		return cbh::vec3(0);

	ray.origin = position;

	// @todo: Check if object is a light source?
	if(ray.currentObject->getMaterial().isLightSource == false)
	{
		return computeRadiance(ray);
	}
	else //
	{
		double costerm = -ray.direction.dot(ray.normal);
		double attenuation = 1.0/(ray.t*ray.t);
		attenuation = attenuation > 1.0 ? 1.0 : attenuation;
		if(costerm > 0)
			return ray.currentObject->getMaterial().emittance.normalizeWithMax();//*attenuation;
		else
			return cbh::vec3(0);
	}
}



//Recursive sampling of radiance
cbh::vec3 MCRayTracer::computeRadiance(Ray &ray)
{
	cbh::vec3 radiance(0);


	
	causticPhotonMap.irradiance_estimate(radiance,ray.origin,ray.normal,0.01,100);
	
	//Only use direct illumination on diffuse surfaces
	if(ray.currentObject->getMaterial().kd > 0)
		radiance += directIllumination(ray);

	radiance += indirectIllumination(ray);

	return radiance;
}


//p1 = position on surface, p2 lightsource position
double MCRayTracer::radianceTransfer(cbh::vec3 &p1, cbh::vec3 &p2)
{
	Ray ray;
	ray.origin = p1;

	// Attenuation factor in V(x,y)
	double distance = (p2 - p1).squareNorm();
	double attenuation = 1.0/distance; // this should make it reach circa 2 units

	ray.direction = (p2 - p1).normalize();
	double t = ray.t = sqrt(distance); 
	ray.Offset();

	//Check if some object is occluding the lightsource
	getIntersection(ray,false);

	if(ray.t > t - 10e-4)
		return 1.0;//(attenuation > 1.0 ? 1.0 : attenuation);
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


			cbh::vec3 lightPos = potentialLight->getRandomPosition();

			cbh::vec3 lightDir = (lightPos - ray.origin).normalize();

			// Add diffuse color to randiance
			double costerm = ray.normal.dot(lightDir);
			double costermLight = potentialLight->getNormal(lightPos).dot(lightDir);
			costermLight = costermLight < 0 ? 0 : costermLight;
			costerm = costerm < 0 ? 0 : costerm;
			radiance += costermLight*costerm*ray.currentObject->getMaterial().color.mtimes(potentialLight->getMaterial().emittance) * radianceTransfer(ray.origin, lightPos);
		}
	}
	return radiance/shadowRays;
}


//Samples indirect illumination by sampling the hemisphere
cbh::vec3 MCRayTracer::indirectIllumination(Ray &ray)
{
	cbh::vec3 radiance(0);

	//Let russian roulette decide wheater the ray gets absorbed or scattered
	double r = (double)rand() / ((double)RAND_MAX + 1);

	ImplicitObject *object = ray.currentObject;
	//Ray is absorbed
	double absorption = object->getMaterial().kd + object->getMaterial().kr + object->getMaterial().kt;
	if(r > absorption)
		return cbh::vec3(0);


	bool refract = false;
	double pdf = 0;

	double Fresnel = 1;

	int Case = -1; // Not set;
	//Determine what happens to ray
	if(r < object->getMaterial().kr)
		Case = REFLECT;
	else if(r < object->getMaterial().kr + object->getMaterial().kt)
	{	
		Fresnel = fresnel(ray.direction, ray.normal,object->getMaterial().rIndex,ray.n);			
		Case = REFLECT_AND_TRANSMIT;
	}
	else
		Case = DIFFUSE;

	if(Case == DIFFUSE)
	{
		//if(ray.diffuseDepth >= maxDiffuseBounces)
		//	return radiance;
		if(ray.diffuseDepth >= 1)
		{
			globalPhotonMap.irradiance_estimate(radiance,ray.origin,ray.normal,0.2,1000);
			radiance = radiance.mtimes(object->getMaterial().color);
			return radiance;
		}

		//globalPhotonMap.irradiance_estimate(radiance,ray.origin,ray.normal,0.2,1000);
		//radiance = radiance.mtimes(object->getMaterial().color);

		for (unsigned int i = 0; i < indirectPaths; ++i) 
		{
			Ray newRay(ray);
			newRay.diffuseDepth++;
						
			newRay.direction = object->getMaterial().sampleHemisphere(ray.normal, pdf);
			//radiance += trace(newRay,true).mtimes(object->getMaterial().brdf(newRay.direction, newRay.direction)) * ray.normal.dot(newRay.direction) * (1/pdf);
			radiance += trace(newRay,true).mtimes(object->getMaterial().brdf(newRay.direction, newRay.direction));
		}
		radiance *= M_PI;
		radiance = radiance / (double)indirectPaths;

	}
	if(Case == REFLECT || Case == REFLECT_AND_TRANSMIT)
	{
		if(ray.reflectionDepth >= maxReflections)
			return radiance;

		Ray newRay(ray);
		newRay.reflectionDepth++;
		cbh::vec3 perfectReflection = cbh::reflect(ray.direction,ray.normal);
		newRay.direction = perfectReflection; 
		radiance += Fresnel*trace(newRay,true).mtimes(object->getMaterial().brdf(perfectReflection, newRay.direction));
	
	}
	if(Case == TRANSMIT || Case == REFLECT_AND_TRANSMIT)
	{

		if(ray.refractionDepth >= maxRefractions)
			return radiance;

		Ray newRay(ray);
		newRay.refractionDepth++;
		double n,n2(ray.currentObject->getMaterial().rIndex);


		//normal points away! ray.direction points towards surface! -> flip direction
		//cos(theta)
		double c1 = ray.normal.dot(ray.direction); 
		newRay.n = n2;

		// n = (nFrom = air = 1) / (nTo = object.rIndex) = 1 / object.rIndex
		n = 1/n2;

		// 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 )
		double c2 = 1 - n*n*(1-c1*c1);
		// -(n1/n2)*ray.direction + normal * ((n1/n2)*cos(theta) - sqrt( 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 ) )
		newRay.direction = n * ray.direction + ray.normal * (n*c1 - sqrt(c2));
		radiance += (1-Fresnel)*refractTrace(newRay);
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

void MCRayTracer::trace_photon(Ray & ray, cbh::vec3 power)
{	

	//********************************
	//get intersection of ray in scene
	//********************************

	// offset the ray to avoid numeric errors
	ray.Offset();

	// get new intersection
	cbh::vec3 position(getIntersection(ray,false));
	if (position == NULL)
		return;
	ray.origin = position;

	

	ImplicitObject *object = ray.currentObject;

	if(ray.diffuseDepth >= maxPhotonDepth)
	{
		if(object->getMaterial().kd > 0)
			currentPhotonMap->store(power,ray.origin,ray.direction.normalize());
		return;
	}


	//Let russian roulette decide wheater the photon gets absorbed or scattered
	double r = (double)rand() / ((double)RAND_MAX );

	//Ray is absorbed
	double absorption = object->getMaterial().kd + object->getMaterial().kr + object->getMaterial().kt;


	if(r > absorption)
	{
		if(object->getMaterial().kd > 0)
			currentPhotonMap->store(power,ray.origin,ray.direction.normalize());
		return;
	}


	if(r < object->getMaterial().kr) //Reflect
	{
		//Calc new direction
		Ray newRay(ray);
		newRay.reflectionDepth++;
		cbh::vec3 perfectReflection = cbh::reflect(ray.direction,ray.normal);
		newRay.direction = perfectReflection;

		//Then trace new ray(photon)
		power = power.mtimes( object->getMaterial().color );
		trace_photon(newRay,power);
	}
	else if(r < object->getMaterial().kr + object->getMaterial().kt) //Transmit/Refract
	{
		//Calculate perfect refraction
		Ray newRay(ray);
		ray.refractionDepth++;
		double n;
		//Are we inside object?
		if(ray.direction.dot(ray.normal) > 0.0 ) //we are inside going out;
			n = 1/ray.currentObject->getMaterial().rIndex;
		else //we are outside going in
			n = ray.currentObject->getMaterial().rIndex;

		//cos(theta)
		double c1 = ray.normal.dot(ray.direction); 

		// 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 )
		double c2 = 1 - n*n*(1-c1*c1);
		// -(n1/n2)*ray.direction + normal * ((n1/n2)*cos(theta) - sqrt( 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 ) )
		newRay.direction = n * ray.direction + ray.normal * (n*c1 - sqrt(c2));

		//Then trace new ray(photon)
		power = power.mtimes( object->getMaterial().color );
		trace_photon(newRay,power);

	}
	else//(diffuse)
	{
		//First store
		currentPhotonMap->store(power,ray.origin,ray.direction.normalize());

		//Sample hemisphere to get new direction
		Ray newRay(ray);
		newRay.diffuseDepth++;
		double pdf = 0; //dummy, not used
		newRay.direction = object->getMaterial().sampleHemisphere(ray.normal, pdf);

		power = power.mtimes( object->getMaterial().color );
		trace_photon(newRay,power);
	}

}

void MCRayTracer::trace_caustic_photon(Ray & ray, cbh::vec3 power)
{	

	//********************************
	//get intersection of ray in scene
	//********************************

	// offset the ray to avoid numeric errors
	ray.Offset();

	// get new intersection
	cbh::vec3 position(getIntersection(ray,false));
	if (position == NULL)
		return;
	ray.origin = position;

	

	ImplicitObject *object = ray.currentObject;

	if(ray.reflectionDepth >= maxPhotonDepth || ray.refractionDepth >= maxPhotonDepth || ray.diffuseDepth >= maxPhotonDepth)
	{
		currentPhotonMap->store(power,ray.origin,ray.direction.normalize());
		if(object->getMaterial().kd > 0)
			return;
	}

	//Let russian roulette decide wheater the photon gets absorbed or scattered
	double r = (double)rand() / ((double)RAND_MAX );

	//Ray is absorbed
	double absorption = object->getMaterial().kd + object->getMaterial().kr + object->getMaterial().kt;


	if(r > absorption)
	{
		if(object->getMaterial().kd > 0)
			currentPhotonMap->store(power,ray.origin,ray.direction.normalize());
		return;
	}


	if(r < object->getMaterial().kr) //Reflect
	{
		Ray newRay(ray);
		newRay.reflectionDepth++;
		cbh::vec3 perfectReflection = cbh::reflect(ray.direction, ray.normal);
		newRay.direction = perfectReflection;

		//Then trace new ray(photon)
		power = power.mtimes( object->getMaterial().color );
		trace_caustic_photon(newRay,power);
	}
	else if(r < object->getMaterial().kr + object->getMaterial().kt) //Transmit/Refract
	{
		//Calculate perfect refraction
		Ray newRay(ray);
		newRay.refractionDepth++;
		double n;
		//Are we inside object?
		if(ray.direction.dot(ray.normal) > 0.0 ) //we are inside going out;
			n = 1/ray.currentObject->getMaterial().rIndex;
		else //we are outside going in
			n = ray.currentObject->getMaterial().rIndex;

		//cos(theta)
		double c1 = ray.normal.dot(ray.direction); 

		// 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 )
		double c2 = 1 - n*n*(1-c1*c1);
		// -(n1/n2)*ray.direction + normal * ((n1/n2)*cos(theta) - sqrt( 1 - (n1/n2)^2 * (1 - normal.dot(ray.direction)^2 ) )
		newRay.direction = n * ray.direction + ray.normal * (n*c1 - sqrt(c2));

		//Then trace new ray(photon)
		power = power.mtimes( object->getMaterial().color );
		trace_caustic_photon(newRay,power);

	}
	else//(diffuse)
	{
		//For the caustic map we store the photon when it hits a diffuse surface
		if(ray.refractionDepth > 0)
			currentPhotonMap->store(power,ray.origin,ray.direction.normalize());
		return;
	}

}

//********************************
// Generate global photon map (first rev)
//********************************
void MCRayTracer::generate_photon_map(int nPhotons, int _maxPhotonDepth)
{
	std::cout << "Generating photon map.." << std::endl;

	maxPhotonDepth = _maxPhotonDepth;

	ImplicitObject * object = scene->getImplicitObject(scene->getNumImplicitObjects()-1);
	globalPhotonMap.init(nPhotons);
	Ray ray;
	double pdf; //Not used
	cbh::vec3 power(object->getMaterial().emittance*200.0);

	srand(int(time(NULL)) ^ omp_get_thread_num());

	currentPhotonMap = &globalPhotonMap;
	for (int i = 1; i < nPhotons; ++i)
	{
		//Generate random direction from light
		ray.origin = object->getRandomPosition();
		ray.direction = sampleHemisphere(ray.normal,pdf);
		ray.t = 5;

		//Trace photon in scene
		trace_photon(ray,power);		
	}

	std::cout << globalPhotonMap.stored_photons <<  " Photons stored..." << std::endl;

	globalPhotonMap.scale_photon_power(1.0/nPhotons);

	std::cout << "Balancing photon map..." << std::endl;
	globalPhotonMap.balance();
	std::cout << "Balancing photon map complete..." << std::endl;

	std::cout << "photon map complete..." << std::endl;
}

//********************************
// Generate caustic photon map (first rev)
//********************************
void MCRayTracer::generate_caustic_map(int nPhotons, int _maxPhotonDepth)
{
	std::cout << "Generating caustic map.." << std::endl;

	maxPhotonDepth = _maxPhotonDepth;

	ImplicitObject * light = scene->getImplicitObject(scene->getNumImplicitObjects()-1);
	ImplicitObject * object = scene->getImplicitObject(scene->getNumImplicitObjects()-2);
	causticPhotonMap.init(nPhotons);
	Ray ray;
	double pdf; //Not used
	cbh::vec3 power(light->getMaterial().emittance);

	srand(int(time(NULL)) ^ omp_get_thread_num());

	//Set the current photon map trace photon uses currentPhotonMap
	currentPhotonMap = &causticPhotonMap;

	for (int i = 1; i < nPhotons; ++i)
	{
		//Generate random direction from light on caustic object
		ray.origin = light->getRandomPosition();
		ray.direction = (object->getRandomPosition() - ray.origin).normalize();
		ray.t = 5;

		//Trace photon in scene
		trace_caustic_photon(ray,power);		
	}

	std::cout << causticPhotonMap.stored_photons <<  " Photons stored..." << std::endl;

	causticPhotonMap.scale_photon_power(1.0/nPhotons);

	std::cout << "Balancing photon map..." << std::endl;
	causticPhotonMap.balance();
	std::cout << "Balancing photon map complete..." << std::endl;

	std::cout << "photon map complete..." << std::endl;
}


//********************************
// Render scene
//********************************
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
	int tilesPerRow = 8;
	int tileWidth = image->width / tilesPerRow;
	int tileHeight = image->height / tilesPerRow;
	int numTiles = tilesPerRow * tilesPerRow;

	// Check so that image width is devisable by number of tiles
	if ((image->width / (float)tilesPerRow) - (image->width / tilesPerRow) > 0) {
		std::cerr << "ERROR: image->width / tilesPerRow must be an integer" << std::endl;
		exit(73);
	}

	double subPixelsPerDim = 2;
	double raysPerSubPixel = raysPerPixel / (subPixelsPerDim*subPixelsPerDim);
	cbh::vec3 subPixelDx = pixelDx;
	cbh::vec3 subPixelDy = pixelDy;

	// OpenMP, parallelize using tiles to mitigate artifacts caused by:
	// TODO: rand() is not thread safe, which causes artifacts since the same random number is used multiple times
	//srand(int(time(NULL)));
	#pragma omp parallel
	{
		#pragma omp for schedule(dynamic,1) private(radiance)
		for (int tile = 0; tile < numTiles; ++tile) {

			srand(int(time(NULL)) ^ omp_get_thread_num());
			int startx = (tile % tilesPerRow) * tileWidth;
			int starty = (tile / tilesPerRow) * tileHeight;

			for (int x = startx; x < startx+tileWidth; ++x) 
			{
				for (int y = starty; y < starty+tileHeight; ++y) 
				{
					radiance = 0;
					Ray ray;
					// Subpixel sampling
					for (int kx = 0; kx < subPixelsPerDim; ++kx) 
						for (int ky = 0; ky < subPixelsPerDim; ++ky)
							for (int k = 0; k < raysPerSubPixel; ++k)
							{

								ray.reset();

								double jitterx = (double)rand() / ((double)RAND_MAX);
								double jittery = (double)rand() / ((double)RAND_MAX);

								
								ray.origin = camPos;

								cbh::vec3 dx = pixelDx*x + subPixelDx * (kx + jitterx);
								cbh::vec3 dy = pixelDy*y + subPixelDy * (ky + jittery);

								ray.direction = (scene->getCam()->getImagePlaneBL() +  dx + dy - ray.origin).normalize();

								radiance += trace(ray,true);

							}							
							radiance = radiance / raysPerPixel;
							toInt(radiance); 
							(*image)(x,y) = cbh::vec3uc(radiance.getX(),radiance.getY(),radiance.getZ());

				}
				if (omp_get_thread_num() == 0)
					viewer->draw(image);
			}

		} // for tiles end


	} //omp parallel end

	//image->toneMap();
	image->Save();

	diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
	std::cout <<"Total elapsed time: "<< diff <<'\n';
	
	// Set continious OpenGL drawing
	viewer->setLoop(true);
	viewer->draw(image);
}




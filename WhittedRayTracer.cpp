

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
    return color.clamp(0, 1);
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


// Returns normalized refreaction vector
// Equations from the book, page 36
// Changed signs according to albert's implementation
cbh::vec3 refract(cbh::vec3 incomming, cbh::vec3 normal, double n) {
    //double cosI = normal.dot(incomming); // book
    double cosI = -normal.dot(incomming); 
    double sinT2 = 1 - n*n * (1 - cosI*cosI);
    
    if (sinT2 < 0.0)
        return NULL;
    
    // return (-n * incomming + normal * ( n * cosI - sqrt(sinT2))).normalize(); // book
    return (n * incomming + normal * ( n * cosI - sqrt(sinT2))).normalize();

}


// Returns normalized reflected vector
cbh::vec3 reflect(cbh::vec3 incomming, cbh::vec3 normal) {
    const double cosI = normal.dot(incomming);
    return (incomming - 2 * cosI * normal).normalize();
} 

cbh::vec3 WhittedRayTracer::trace(Ray &ray)
{
	static float t; t = 0;
	static ImplicitObject * hitObject; hitObject = 0;


	if(getIntersection(hitObject, t, ray) == false)
		return cbh::vec3(0);
    
	else if(ray.depth >= maxReflectionRays)
	{
		cbh::vec3 position = ray.origin + t * ray.direction;
		return directIllumination(hitObject, position);

	}
	else // hit == true && ray.depth < maxReflectionRays, Only for specular objects
	{
		double epsilon = 10e-4*t;
		cbh::vec3 position = ray.origin + t * ray.direction;
        
        float refraction = hitObject->getMaterial().refraction;
        float reflection = hitObject->getMaterial().reflection;
        
        
        // Direct illuimantion
		cbh::vec3 color = (1-refraction-reflection)*directIllumination(hitObject, position);

        cbh::vec3 N = hitObject->getNormal(position);
        ++ray.depth;

        
        
        // Perfect Refraction
        if (refraction > 0) {
            double n1 = 1;
            double n2 = hitObject->getMaterial().rIndex;
            Ray refractionRay;
            refractionRay.depth = ray.depth;
            
            if (N.dot(ray.direction) > 0) { // inside the object
                refractionRay.direction = refract(ray.direction, -N, n2/n1);
            }
            else
                refractionRay.direction = refract(ray.direction, N, n1/n2);
            
            
            if (refractionRay.direction != NULL) {
                refractionRay.origin = position + epsilon*refractionRay.direction;
                color += refraction*trace(refractionRay);
            }
            else {
                color = cbh::vec3(1.0,0.0,0.0);
                printf("ERROR: REFLECTION\n");
            }
            
        }
        
        
        //Perfect reflection
        if (reflection > 0) {
            
            ray.direction = reflect(ray.direction, N);
            ray.origin = position + epsilon * ray.direction;
            color += reflection*trace(ray);
        }
		
        return color;
	}	

}


void WhittedRayTracer::render()
{
	Ray ray;
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
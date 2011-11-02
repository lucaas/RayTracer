

#include "WhittedRayTracer.h"
#include "Utility.h"

WhittedRayTracer::WhittedRayTracer()
	: raysPerPixel(1), maxReflectionRays(1), maxRefractionRays(1)
{
	scene = new Scene();
}

WhittedRayTracer::WhittedRayTracer(unsigned int raysPerPixel,unsigned int maxReflectionRays, unsigned int maxRefractionRays) 
	: raysPerPixel(raysPerPixel), maxReflectionRays(maxReflectionRays), maxRefractionRays(maxRefractionRays)
{
	scene = new Scene();
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


glm::vec3 WhittedRayTracer::trace(Ray &ray)
{
	float t; t = 0;
	float tHit; tHit = 100000;
	bool hit; hit = false;
	ImplicitObject * hitObject; hitObject = 0;

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

			hit = true;
		}
	}

	if(!hit)
		return glm::vec3(0);
	else if(ray.depth >= maxReflectionRays || hitObject->getMaterial().reflection == 0)
	{
		glm::vec3 position = ray.origin + tHit * ray.direction;
		glm::vec3 color(0);
		for(int j = 0; j < scene->getNumLights(); ++j)
		{
			color += phong(hitObject->getMaterial(), position, *(scene->getLight(j)), glm::normalize(scene->getCam()->origin-position), hitObject->getNormal(position));
		}

		return color;

	}
	else // hit == true && ray.depth < maxReflectionRays, Only for specular objects
	{

		glm::vec3 position = ray.origin + tHit * ray.direction;
		glm::vec3 color(0);
		for(int j = 0; j < scene->getNumLights(); ++j)
		{
			color += phong(hitObject->getMaterial(), position, *(scene->getLight(j)), glm::normalize(scene->getCam()->origin-position), hitObject->getNormal(position));
		}

		++ray.depth;
		//Spawn new ray in perfect reflection direction
		glm::vec3 N = hitObject->getNormal(position);
        ray.origin = position + 0.0001f * N;

        
		//Perfect reflection
        float reflet = 2.0f * glm::dot(ray.direction, N);
        ray.direction = ray.direction - reflet * N;
        
		//ray.direction = glm::normalize(2*glm::dot(N,-ray.direction)*N + ray.direction);
		
		glm::vec3 reflectionColor = trace(ray);
		float reflection = hitObject->getMaterial().reflection;
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
			ray.origin = scene->getCam()->origin;

			glm::vec3 dy = scene->getCam()->pixelDy;
			glm::vec3 dx = scene->getCam()->pixelDx;
			dx *= x;
			dy *= y;

			ray.direction = glm::normalize(scene->getCam()->planeBLeft + dx + dy - ray.origin);
			
			(*image)(x,y) = float2rgb(trace(ray));
			
		/*
			bool hit = false;
			ImplicitObject * currobject;
			float tHit = 1000000;

			for(int i = 0; i < scene->getNumImplicitObjects(); ++i)
			{
				ImplicitObject *object = scene->getImplicitObject(i);
				if(object->intersects(ray,t))
				{
					//Save the closest t
					if(t < tHit)
					{
						tHit = t;
						currobject = object;
					}

					hit = true;
				}
			}

			//Shade
			if(hit)
			{
				glm::vec3 position = ray.origin + tHit * ray.direction;
				glm::vec3 color(0);
				for(int j = 0; j < scene->getNumLights(); ++j)
				{
					color += phong(currobject->getMaterial(), position, *(scene->getLight(j)), glm::normalize(scene->getCam()->origin-position), currobject->getNormal(position));
				}
				(*image)(x,y) = float2rgb(color);
			}
			else
				(*image)(x,y) = float2rgb(glm::vec3(0.5,0.5,0.5));
			

			*/
		}
	}
	image->Save();
}
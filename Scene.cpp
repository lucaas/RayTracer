
#include "Scene.h"

Scene::Scene() : numImplicitObjects(0), numLights(0), WIDTH(400), HEIGHT(400)
{

}

Scene::~Scene()
{
	for (std::vector<ImplicitObject *>::iterator itr = implicitObjects.begin(); itr != implicitObjects.end(); itr++)
	{
		delete *itr;
	}
	
	for (std::vector<ILight *>::iterator itr = lightSources.begin(); itr != lightSources.end(); itr++)
	{
		delete *itr;
	}	
}

void Scene::addImplicitObject(ImplicitObject * object)
{
	implicitObjects.push_back(object);
	++numImplicitObjects;
}

void Scene::addLight(ILight * light)
{
	lightSources.push_back(light);
	++numLights;
}

ImplicitObject * Scene::getImplicitObject(unsigned int i) const
{
	if(i < numImplicitObjects)
		return implicitObjects.at(i);
	else
		return NULL;
}

ILight * Scene::getLight(unsigned int i) const
{
	if(i < numLights)
		return lightSources.at(i);
	else
		return NULL;
}

void Scene::setCamera(Camera * _cam)
{
	cam = _cam;
}

Camera * Scene::getCam() const
{
	return cam;
}

unsigned int Scene::getNumImplicitObjects() const
{
	return numImplicitObjects;
}

unsigned int Scene::getNumLights() const
{
	return numLights;
}
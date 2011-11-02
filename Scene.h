#ifndef SCENE
#define SCENE

#include "ImplicitObject.h"
#include "Camera.h"
#include "PointLight.h"
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();
	void addImplicitObject(ImplicitObject *);
	void addPointLight(PointLight *);
	ImplicitObject * getImplicitObject(unsigned int) const;
	PointLight * getLight(unsigned int i) const;
	void setCamera(Camera *);
	Camera * getCam() const;
	unsigned int getNumImplicitObjects() const;
	unsigned int getNumLights() const;
private:
	Camera * cam;
	std::vector<ImplicitObject *> implicitObjects;
	std::vector<PointLight *> lightSources;
	unsigned int numImplicitObjects,numLights;
};




#endif
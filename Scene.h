#ifndef SCENE
#define SCENE

#include "ImplicitObject.h"
#include "Camera.h"
#include "ILight.h"
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();
	virtual void addImplicitObject(ImplicitObject *);
	virtual void addPointLight(ILight *);
	virtual void Init() = 0;
	virtual ImplicitObject * getImplicitObject(unsigned int) const;
	virtual ILight * getLight(unsigned int i) const;
	virtual void setCamera(Camera *);
	virtual Camera * getCam() const;
	virtual unsigned int getNumImplicitObjects() const;
	virtual unsigned int getNumLights() const;

private:
	Camera * cam;
	std::vector<ImplicitObject *> implicitObjects;
	std::vector<ILight *> lightSources;
	unsigned int numImplicitObjects,numLights,WIDTH,HEIGHT;

};




#endif
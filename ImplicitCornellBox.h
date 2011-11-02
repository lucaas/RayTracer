#ifndef IMPLICIT_CORNELL_BOX
#define IMPLICIT_CORNELL_BOX

#include "Scene.h"
#include "glm.hpp"
#include "ImplicitSphere.h"
#include "ImplicitPlane.h"
#include "PointLight.h"
#include "Ray.h"
#include "Img.h"

class ImplicitCornellBox : public Scene
{
public:
	ImplicitCornellBox() : Scene() {}

	~ImplicitCornellBox()
	{

	}

	void Init()
	{
		PointLight * light = new PointLight(glm::vec3(0,1,0), glm::vec3(1.0f,1.0f,1.0f), 1.0f);


		ImplicitSphere * sphere = new ImplicitSphere(0.7,glm::vec3(1.5,-1.3,1)); //Red
		ImplicitSphere * sphere2 = new ImplicitSphere(0.7,glm::vec3(-2,-1.3,-1)); //Gray
		ImplicitSphere * sphere3 = new ImplicitSphere(1.4f, glm::vec3(1,0,-2)); //Mid


		ImplicitPlane * pfloor = new ImplicitPlane(2,glm::vec3(0,1,0)); // floor
		ImplicitPlane * pleft = new ImplicitPlane(3,glm::vec3(1,0,0)); // left
		ImplicitPlane * pright = new ImplicitPlane(3,glm::vec3(-1,0,0)); // right
		ImplicitPlane * pceil = new ImplicitPlane(2,glm::vec3(0,-1,0)); // Ceiling
		ImplicitPlane * pback = new ImplicitPlane(2,glm::vec3(0,0,1)); // Back
		ImplicitPlane * pfront = new ImplicitPlane(40,glm::vec3(0,0,-1)); // front

		pfloor->material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1,1,1), 0.0f, 0, 0);
		pleft->material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 0.1f, 0.1f), glm::vec3(1,1,1), 0.0f, 0, 0);
		pright->material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(0.1f, 1.0f, 0.1f), glm::vec3(1,1,1), 0.0f, 0, 0);
		pceil->material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1,1,1), 0.0f, 0, 0);
		pback->material = Material(0.1f, 0.8f, 0.8f, glm::vec3(0.1,0.1,0.1), glm::vec3(1,1,1), glm::vec3(0.25f, 0.25f, 0.05f), 0.0f, 0, 0);
		pfront->material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1,1,1), 0.0f, 0, 0);


		sphere->material = Material(0.1f, 0.8f, 0.2f, glm::vec3(1,0,0), glm::vec3(0.7f, 0.2f, 0.2f), glm::vec3(1,1,1), 10.0f, 0.15f, 0);
		sphere2->material = Material(0.0f, 0.75f, 0.5f, glm::vec3(0,0,1), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(1,1,1), 200.0f, 1, 0);
		sphere3->material = Material(0.0f, 1, 1, glm::vec3(0,0,1), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1,1,1), 100.0f, 0.9f, 0);
		

		addImplicitObject(pfloor);
		addImplicitObject(pleft);
		addImplicitObject(pright);
		addImplicitObject(pceil);
		addImplicitObject(pback);
		addImplicitObject(pfront);

		addImplicitObject(sphere);
		addImplicitObject(sphere2);
		addImplicitObject(sphere3);
		addPointLight(light);

	}


};


#endif
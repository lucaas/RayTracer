#ifndef IMPLICIT_CORNELL_BOX
#define IMPLICIT_CORNELL_BOX

#include "Scene.h"
#include "ImplicitSphere.h"
#include "ImplicitPlane.h"
#include "PointLight.h"
#include "Img.h"
#include "PhongMaterial.h"

class ImplicitCornellBox : public Scene
{
public:
	ImplicitCornellBox() : Scene() {}

	~ImplicitCornellBox()
	{

	}

	void Init()
	{
		PointLight * light = new PointLight(cbh::vec3(0,1,0), cbh::vec3(1.0f,1.0f,1.0f), 0.5f);
		PointLight * light2 = new PointLight(cbh::vec3(1,1,2), cbh::vec3(1.0f,1.0f,1.0f), 0.5f);


		ImplicitSphere * sphere = new ImplicitSphere(0.7,cbh::vec3(-1,-1.3,1)); //LEFT
		ImplicitSphere * sphere2 = new ImplicitSphere(0.7,cbh::vec3(1,0,-1)); //RIGHT
		ImplicitSphere * sphere3 = new ImplicitSphere(0.7, cbh::vec3(1,-1.3,2)); //MID

		sphere->setMaterial(new PhongMaterial(cbh::vec3(1.0, 0.1, 0.1), 0.8, 0.0, 0));
		sphere2->setMaterial(new PhongMaterial(cbh::vec3(0.1, 1.0, 0.1), 0.8, 0.0, 0));
		sphere3->setMaterial(new PhongMaterial(cbh::vec3(0.1, 0.1, 1.0), 0.8, 0.0, 0));
		
		ImplicitPlane * pfloor = new ImplicitPlane(3,cbh::vec3(0,1,0)); // floor
		ImplicitPlane * pleft = new ImplicitPlane(3,cbh::vec3(1,0,0)); // left
		ImplicitPlane * pright = new ImplicitPlane(3,cbh::vec3(-1,0,0)); // right
		ImplicitPlane * pceil = new ImplicitPlane(3,cbh::vec3(0,-1,0)); // Ceiling
		ImplicitPlane * pback = new ImplicitPlane(2,cbh::vec3(0,0,1)); // Back
		ImplicitPlane * pfront = new ImplicitPlane(40,cbh::vec3(0,0,-1)); // front

		pfloor->setMaterial(new PhongMaterial(cbh::vec3(1), 0.8, 0.0, 0.0)); //floor
		pceil->setMaterial(new PhongMaterial(cbh::vec3(1), 0.8, 0.0, 0.0)); //ceiling
		pfront->setMaterial(new PhongMaterial(cbh::vec3(1), 0.8, 0.0, 0.0)); //front
		pleft->setMaterial(new PhongMaterial(cbh::vec3(1,0.1,0.1), 0.8, 0.0, 0.0)); //left
		pright->setMaterial(new PhongMaterial(cbh::vec3(0.1,1,0.1), 0.8, 0.0, 0.0)); //right
		pback->setMaterial(new PhongMaterial(cbh::vec3(1), 0.8, 0.0, 0.0)); //back
		



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
		addPointLight(light2);

	}


};


#endif
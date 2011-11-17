#ifndef IMPLICIT_CORNELL_BOX
#define IMPLICIT_CORNELL_BOX

#include "Scene.h"
#include "ImplicitSphere.h"
#include "ImplicitPlane.h"
#include "UniformAreaLight.h"
#include "Img.h"
#include "PhongMaterial.h"
#include "DiffuseMaterial.h"

class ImplicitCornellBox : public Scene
{
public:
	ImplicitCornellBox() : Scene() {}

	~ImplicitCornellBox()
	{

	}

	void Init()
	{
		//PointLight * light = new PointLight(cbh::vec3(0,1,0), cbh::vec3(1.0f,1.0f,1.0f), 0.5f);
		//PointLight * light2 = new PointLight(cbh::vec3(1,1,2), cbh::vec3(1.0f,1.0f,1.0f), 0.5f);
		UniformAreaLight * light = new UniformAreaLight(0.5, cbh::vec3(0,2,1));


		ImplicitSphere * sphere = new ImplicitSphere(0.7,cbh::vec3(-1,-2.3,1)); //LEFT
		ImplicitSphere * sphere2 = new ImplicitSphere(0.7,cbh::vec3(1,-1,-1)); //RIGHT
		ImplicitSphere * sphere3 = new ImplicitSphere(0.7, cbh::vec3(1,-2.3,2)); //MID

		sphere->setMaterial(new PhongMaterial(cbh::vec3(1.0, 1.0, 1.0), 0.0, 1.0, 20.0));
		sphere2->setMaterial(new DiffuseMaterial(cbh::vec3(0.1, 1.0, 0.1), 1.0));
		sphere3->setMaterial(new PhongMaterial(cbh::vec3(0.1, 0.1, 1.0), 0.0, 1.0, 20.0));
		
		ImplicitPlane * pfloor = new ImplicitPlane(3,cbh::vec3(0,1,0)); // floor
		ImplicitPlane * pleft = new ImplicitPlane(3,cbh::vec3(1,0,0)); // left
		ImplicitPlane * pright = new ImplicitPlane(3,cbh::vec3(-1,0,0)); // right
		ImplicitPlane * pceil = new ImplicitPlane(3,cbh::vec3(0,-1,0)); // Ceiling
		ImplicitPlane * pback = new ImplicitPlane(2,cbh::vec3(0,0,1)); // Back
		ImplicitPlane * pfront = new ImplicitPlane(40,cbh::vec3(0,0,-1)); // front

		pfloor->setMaterial(new DiffuseMaterial(cbh::vec3(1),1.0)); //floor
		pceil->setMaterial(new DiffuseMaterial(cbh::vec3(1), 1.0)); //ceiling
		pfront->setMaterial(new DiffuseMaterial(cbh::vec3(1), 1.0)); //front
		pleft->setMaterial(new DiffuseMaterial(cbh::vec3(1,0.1,0.1), 1.0)); //left
		pright->setMaterial(new DiffuseMaterial(cbh::vec3(0.1,1,0.1), 1.0)); //right
		pback->setMaterial(new DiffuseMaterial(cbh::vec3(1), 1.0)); //back

		/*
		pfloor->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,20.0)); //floor
		pceil->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,20.0)); //ceiling
		pfront->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,20.0)); //front
		pleft->setMaterial(new PhongMaterial(cbh::vec3(1,0.1,0.1),0.0,1.0,20.0)); //left
		pright->setMaterial(new PhongMaterial(cbh::vec3(0.1,1,0.1),0.0,1.0,20.0)); //right
		pback->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,20.0)); //back
		*/



		addImplicitObject(pfloor);
		addImplicitObject(pleft);
		addImplicitObject(pright);
		addImplicitObject(pceil);
		addImplicitObject(pback);
		addImplicitObject(pfront);

		addImplicitObject(sphere);
		addImplicitObject(sphere2);
		addImplicitObject(sphere3);

		addLight(light);
		//addPointLight(light2);

	}


};


#endif
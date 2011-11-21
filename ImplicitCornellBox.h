#ifndef IMPLICIT_CORNELL_BOX
#define IMPLICIT_CORNELL_BOX

#include "Scene.h"
#include "ImplicitSphere.h"
#include "ImplicitPlane.h"
#include "OBJMesh.h"
#include "UniformAreaLight.h"
#include "Img.h"
#include "PhongMaterial.h"
#include "DiffuseMaterial.h"
#include "LightMaterial.h"


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
		//UniformAreaLight * light = new UniformAreaLight(0.3, cbh::vec3(0,2,1));
		ImplicitSphere * sphereLight = new ImplicitSphere(0.9,cbh::vec3(0,2.5,2)); //LIGHT SOURCE

	
		ImplicitSphere * sphere = new ImplicitSphere(1.5,cbh::vec3(-2,-1.5,0.5)); //WHITE
		ImplicitSphere * sphere2 = new ImplicitSphere(1.3,cbh::vec3(2, -1,1)); //Glasboll
		//ImplicitSphere * sphere3 = new ImplicitSphere(0.7, cbh::vec3(1,-2.3,2)); //BLUE

		sphereLight->setMaterial(new LightMaterial(cbh::vec3(7), bool(true))); 
		sphere->setMaterial(new PhongMaterial(cbh::vec3(0.99),0.0,1.0,0.0,1)); 
		sphere2->setMaterial(new PhongMaterial(cbh::vec3(0.99),0.0, 0.0, 1.0, 1, 1.5)); //Glasboll
		//sphere3->setMaterial(new PhongMaterial(cbh::vec3(1.0, 1.0, 1.0),0.0,1.0,0.0,50)); //BLUE

		/*
		// OBJ Mesh
		OBJMesh *mesh = new OBJMesh("cube.obj");
		mesh->setMaterial(new DiffuseMaterial(cbh::vec3(0.4,0.8,0.8), 0.6));
		*/

		ImplicitPlane * pfloor = new ImplicitPlane(3,cbh::vec3(0,1,0)); // floor
		ImplicitPlane * pleft = new ImplicitPlane(4,cbh::vec3(1,0,0)); // left
		ImplicitPlane * pright = new ImplicitPlane(4,cbh::vec3(-1,0,0)); // right
		ImplicitPlane * pceil = new ImplicitPlane(3,cbh::vec3(0,-1,0)); // Ceiling
		ImplicitPlane * pback = new ImplicitPlane(2,cbh::vec3(0,0,1)); // Back
		ImplicitPlane * pfront = new ImplicitPlane(15,cbh::vec3(0,0,-1)); // front


		pfloor->setMaterial(new DiffuseMaterial(cbh::vec3(0.5),1.0)); //floor
		pceil->setMaterial(new DiffuseMaterial(cbh::vec3(0.5), 1.0)); //ceiling
		pfront->setMaterial(new DiffuseMaterial(cbh::vec3(0.5), 1.0)); //front
		pleft->setMaterial(new DiffuseMaterial(cbh::vec3(0.5,0.25,0.25), 1.0)); //left
		pright->setMaterial(new DiffuseMaterial(cbh::vec3(0.25,0.25,0.5), 1.0)); //right
		pback->setMaterial(new DiffuseMaterial(cbh::vec3(0.5), 1.0)); //back


		/*
		pfloor->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //floor
		pceil->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //ceiling
		pfront->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //front
		pleft->setMaterial(new PhongMaterial(cbh::vec3(1,0.1,0.1),0.0,1.0,0.0,20.0)); //left
		pright->setMaterial(new PhongMaterial(cbh::vec3(0.1,1,0.1),0.0,1.0,0.0,20.0)); //right
		pback->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //back
		*/



		addImplicitObject(pfloor);
		addImplicitObject(pleft);
		addImplicitObject(pright);
		addImplicitObject(pceil);
		addImplicitObject(pback);
		addImplicitObject(pfront);

		addImplicitObject(sphereLight);
		addImplicitObject(sphere);
		addImplicitObject(sphere2);
		//addImplicitObject(sphere3);

		//addImplicitObject(mesh);

		//addLight(light);
		//addPointLight(light2);

	}


};


#endif
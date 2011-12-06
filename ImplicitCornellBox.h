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
#include "ImplicitBoundedPlane.h"


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
		
		//ImplicitSphere * sphereLight = new ImplicitSphere(20,cbh::vec3(0,22.9,1.5)); //LIGHT SOURCE
		//ImplicitSphere * sphereLight = new ImplicitSphere(0.5,cbh::vec3(0,2,1.5)); //LIGHT SOURCE
		ImplicitBoundedPlane * planeLight = new ImplicitBoundedPlane(cbh::vec3(0,2.999,1.5), 2, 2, cbh::vec3(0,-1,0)); //LIGHT SOURCE

		ImplicitSphere * sphere = new ImplicitSphere(1.5,cbh::vec3(-2.5,-1.5,0.8)); //WHITE
		ImplicitSphere * sphere2 = new ImplicitSphere(1.3,cbh::vec3(2.5, -1,2)); //Glasboll
		ImplicitSphere * sphere3 = new ImplicitSphere(0.5, cbh::vec3(0,-2,2)); //Green

		//sphereLight->setMaterial(new LightMaterial(cbh::vec3(1000), bool(true))); 
		planeLight->setMaterial(new LightMaterial(cbh::vec3(6.0), bool(true)));
		sphere->setMaterial(new PhongMaterial(cbh::vec3(0.99),0.0, 1.0, 0.0, 100)); 
		sphere2->setMaterial(new PhongMaterial(cbh::vec3(0.99),         0.0, 0.0, 1.0, 400, 1.5)); //Glasboll
		sphere3->setMaterial(new DiffuseMaterial(cbh::vec3(0.2, 0.8, 0.2),1.0)); //BLUE

		
		// OBJ Mesh
		OBJMesh *mesh = new OBJMesh("cube.obj", cbh::vec3(0));
		mesh->setMaterial(new DiffuseMaterial(cbh::vec3(0.4,0.8,0.8), 1.0));
		

		ImplicitPlane * pfloor = new ImplicitPlane(3,cbh::vec3(0,1,0)); // floor
		ImplicitPlane * pleft = new ImplicitPlane(4.5,cbh::vec3(1,0,0)); // left
		ImplicitPlane * pright = new ImplicitPlane(4.5,cbh::vec3(-1,0,0)); // right
		ImplicitPlane * pceil = new ImplicitPlane(3,cbh::vec3(0,-1,0)); // Ceiling
		ImplicitPlane * pback = new ImplicitPlane(2,cbh::vec3(0,0,1)); // Back
		ImplicitPlane * pfront = new ImplicitPlane(15,cbh::vec3(0,0,-1)); // front


		pfloor->setMaterial(new DiffuseMaterial(cbh::vec3(0.85),1.0)); //floor
		pceil->setMaterial(new DiffuseMaterial(cbh::vec3(0.85), 1.0)); //ceiling
		pfront->setMaterial(new DiffuseMaterial(cbh::vec3(0.85), 1.0)); //front
		pleft->setMaterial(new DiffuseMaterial(cbh::vec3(0.75,0.25,0.25), 1.0)); //left
		pright->setMaterial(new DiffuseMaterial(cbh::vec3(0.25,0.25,0.75), 1.0)); //right
		pback->setMaterial(new DiffuseMaterial(cbh::vec3(0.85), 1.0)); //back


		/*
		pfloor->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //floor
		pceil->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //ceiling
		pfront->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //front
		pleft->setMaterial(new PhongMaterial(cbh::vec3(1,0.1,0.1),0.0,1.0,0.0,20.0)); //left
		pright->setMaterial(new PhongMaterial(cbh::vec3(0.1,1,0.1),0.0,1.0,0.0,20.0)); //right
		pback->setMaterial(new PhongMaterial(cbh::vec3(1),0.0,1.0,0.0,20.0)); //back
		*/

		//----------------------------------------------------------------------------//
		// KOM FÖR I HeLVETE IHÅG ATT GLASBOLL MÅSTE SÄTTAS IN NÄST SIST FÖR ATT CAUSTIC MAP SKA FUNKA
		//----------------------------------------------------------------------------//

		//----------------------------------------------------------------------------//
		// KOM FÖR I HeLVETE IHÅG ATT LAMPAN MÅSTE SÄTTAS IN SIST FÖR ATT NÅTT SKA FUNKA
		//----------------------------------------------------------------------------//

		addImplicitObject(pfloor);
		addImplicitObject(pleft);
		addImplicitObject(pright);
		addImplicitObject(pceil);
		addImplicitObject(pback);
		addImplicitObject(pfront);

		addImplicitObject(sphere);
		addImplicitObject(mesh);
		addImplicitObject(sphere3);

		addImplicitObject(sphere2); //Glas
		addImplicitObject(planeLight);

		//addLight(light);
		//addPointLight(light2);

	}


};


#endif
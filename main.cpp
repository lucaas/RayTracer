#include "OpenGLViewer.h"
#include "Img.h"
#include "MCRayTracer.h"
#include "Camera.h"
#include "Vector3.h"
#include "ImplicitCornellBox.h"
#include "OBJMesh.h"

<<<<<<< HEAD
const unsigned int WIDTH = 600;
=======
const unsigned int WIDTH = 400;
>>>>>>> 70f6e6e50711b5130376c0df7d6a51909dca115c
const unsigned int HEIGHT = 400;
	
int main()
{	

	// Create the camera
	Camera camera(WIDTH,HEIGHT,1,1);
	camera.SetFov(50);
	camera.LookAt(cbh::vec3(0,0,10), cbh::vec3(0,0,0), cbh::vec3(0,1,0));
	
	ImplicitCornellBox scene;
	//Init the scene -> Creates all objects/materials/etc
	scene.Init();
	//Sets (current) camera to the scene
	//By having this outside the scene class we can define multiple camera
	//and render several frame from different camera
	scene.setCamera(&camera);

	//Screenbuffer
	Img image(WIDTH,HEIGHT);

	// OpenGL Viewer
	OpenGLViewer viewer(WIDTH,HEIGHT);

	//Create the render engine then set the screenbuffer and scene to render
<<<<<<< HEAD
	MCRayTracer tracer(2000, 5, 1, 5);
=======
	MCRayTracer tracer(200, 5, 1, 5);
>>>>>>> 70f6e6e50711b5130376c0df7d6a51909dca115c
	tracer.setImage(&image);
	tracer.setScene(&scene);
	tracer.setViewer(&viewer);
	tracer.render();

	/*
	// OBJ Mesh testing...
	OBJMesh mesh("cube.obj");

	Ray ray;
	ray.origin = cbh::vec3(-10,-10,-10);
	ray.direction = cbh::vec3(1,1,1).normalize();
	
	bool intersects = mesh.intersects(ray);
	std::cout << "intersects: " << ((intersects) ? "yes" : "no")  <<  " at " << ray.t << std::endl;

	ray.direction = cbh::vec3(-1,-1,-1).normalize();
	
	intersects = mesh.intersects(ray);
	std::cout << "intersects: " << ((intersects) ? "yes" : "no") <<  " at " << ray.t << std::endl;
	*/

	system("PAUSE");
	
	return 0;
}

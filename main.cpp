
#include "ImplicitCornellBox.h"
#include "MCRayTracer.h"
#include "Camera.h"
#include "Vector3.h"

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;
	
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

	//Create the render engine then set the screenbuffer and scene to render
	MCRayTacer tracer(1, 1, 40, 5);
	tracer.setImage(&image);
	tracer.setScene(&scene);
	tracer.render();

	system("PAUSE");
	
	return 0;
}

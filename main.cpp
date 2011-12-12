#include "OpenGLViewer.h"
#include "Img.h"
#include "MCRayTracer.h"
#include "Camera.h"
#include "Vector3.h"
#include "ImplicitCornellBox.h"
#include "OBJMesh.h"
#include "PhotonMap.h"

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 400;

#include <ctime>
#include <cmath>
#include <stdlib.h>
	
int main()
{	
	// Create the camera
	Camera camera(WIDTH,HEIGHT,1.5,1);
	camera.SetFov(65);
	camera.LookAt(cbh::vec3(0,0,10), cbh::vec3(0,0,0), cbh::vec3(0,1,0));
	
	ImplicitCornellBox scene;
	scene.Init();

	scene.setCamera(&camera);

	//Screenbuffer
	Img image(WIDTH,HEIGHT);

	// OpenGL Viewer
	OpenGLViewer viewer(WIDTH,HEIGHT);

	//Create the render engine then set the screenbuffer and scene to render
	MCRayTracer tracer;

	tracer.setMaxReflection(3);
	tracer.setMaxRefractions(3);
	tracer.setMaxDiffuseBounces(3);
	tracer.setFinalGatherRays(1);
	tracer.setShadowRays(1);
	tracer.setSamplesPerPixel(512);
	tracer.setImage(&image);
	tracer.setScene(&scene);
	tracer.setViewer(&viewer);
	tracer.generate_photon_map(100000,10);
	tracer.generate_caustic_map(100000,5);
	tracer.render();
	

	std::cin.get();

	return 0;
}

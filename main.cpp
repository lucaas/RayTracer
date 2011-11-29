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

/*
	const int N = 10000;
	Photon_map pmap;
	pmap.init(N);
		

	//std::cin.get();

		srand(int(time(NULL)));
	for (int i = 1; i <= N; i++)
	{
		float x = 2 *( (double)rand() / ((double)RAND_MAX) - 0.5 );
		float y = 2 *( (double)rand() / ((double)RAND_MAX) - 0.5 );
		float z = 2 *( (double)rand() / ((double)RAND_MAX) - 0.5 );

		Photon photon;
		photon.pos[0] = x;
		photon.pos[1] = y;
		photon.pos[2] = z;
		photon.power[0] = photon.power[1] = photon.power[2] = 1.0;
	
		float dir[3] = {1,1,1};
		pmap.store(photon.power,photon.pos,photon.pos);
	}

// 	for (int i = 1; i <= N; i++)
// 		std::cout << pmap.photons[i].power[0] << ", plane: " << pmap.photons[i].plane << std::endl;
	pmap.scale_photon_power(1.0);
	pmap.balance();

// 	for (int i = 1; i <= N; i++)
// 		std::cout << pmap.photons[i].power[0] << ", plane: " << pmap.photons[i].plane << std::endl;

	cbh::vec3 irrad(0);
	cbh::vec3 pos = pmap.photons[5].pos;
	cbh::vec3 normal(1);

	pmap.irradiance_estimate(irrad,pos,normal,0.5,9);*/
	
	
	
	// Create the camera
	Camera camera(WIDTH,HEIGHT,1.5,1);
	camera.SetFov(65);
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
	MCRayTracer tracer(100, 5,1, 5);
	tracer.setImage(&image);
	tracer.setScene(&scene);
	tracer.setViewer(&viewer);

	
	tracer.generate_photon_map(100000,5);
	tracer.generate_caustic_map(100000,5);

	/*for (int i = 1; i <= 1000; i++)
	{
		using namespace std;
		std::cout.unsetf(std::ios::floatfield);
		std::cout.setf(ios::fixed,ios::floatfield);
		std::cout << tracer.globalPhotonMap.photons[i].power[0] << "," << tracer.globalPhotonMap.photons[i].power[1] << "," << tracer.globalPhotonMap.photons[i].power[2] << ",";
		std::cout << tracer.globalPhotonMap.photons[i].pos[0] << "," << tracer.globalPhotonMap.photons[i].pos[1] << "," << tracer.globalPhotonMap.photons[i].pos[2] << endl;
	}*/

	
	tracer.render();
	

	std::cin.get();

	return 0;
}

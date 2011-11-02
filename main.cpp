
#include <math.h>

#include "Scene.h"
#include "ImplicitCornellBox.h"
#include "WhittedRayTracer.h"
#include "Camera.h"

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

int main()
{	
	//Create the camera
	Camera camera(WIDTH,HEIGHT,1,1);
	camera.SetFov(40);
	camera.LookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));

	
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
	WhittedRayTracer tracer(1,2,2);
	tracer.setImage(&image);
	tracer.setScene(&scene);
	tracer.render();
	
	return 0;
}









// 	Img image(WIDTH,HEIGHT,"render.ppm");
// 
// 	Ray ray;
// 	ray.origin = camera.origin;
// 	float t = 0;
// 
// 	for (int x = 0; x < WIDTH; ++x) 
// 	{
// 		for (int y = 0; y < HEIGHT; ++y) 
// 		{
// 			
// 			glm::vec3 dy = camera.pixelDy;
// 			glm::vec3 dx = camera.pixelDx;
// 			dx *= x;
// 			dy *= y;
// 
// 			ray.direction = camera.planeBLeft + dx + dy - ray.origin;
// 
// 			if (sphere.intersects(ray, t)) 
// 			{
// 				glm::vec3 position = ray.origin + t * ray.direction;
// 				glm::vec3 color = phong(sphere.material, position, light, glm::normalize(camera.origin-position), sphere.getNormal(position));
// 				//image[x + y*WIDTH] = float2rgb(color);
// 				image(x,y) = float2rgb(color);
// 			}
// 			else if(sphere2.intersects(ray, t)) 
// 			{
// 				glm::vec3 position = ray.origin + t * ray.direction;
// 				glm::vec3 color = phong(sphere2.material, position, light, glm::normalize(camera.origin-position), sphere2.getNormal(position));
// 				image(x,y) = float2rgb(color);
// 			}
// 			else if(plane.intersects(ray, t)) 
// 			{
// 				glm::vec3 position = ray.origin + t * ray.direction;
// 				glm::vec3 color = phong(plane.material, position, light, glm::normalize(camera.origin-position), plane.getNormal(position));
// 				image(x,y) = float2rgb(color);
// 			}
// 			else 
// 				image(x,y) =  ColorRGB(128);
// 		}
// 	}
// 
// 
// 	image.Save();

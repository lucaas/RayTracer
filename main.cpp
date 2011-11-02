
#include <math.h>
#include "glm.hpp"
#include "Camera.h"
#include "ImplicitSphere.h"
#include "ImplicitPlane.h"
#include "PointLight.h"
#include "Ray.h"
#include "Img.h"
#include "Scene.h"
#include "WhittedRayTracer.h"

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

int main()
{

	PointLight light(glm::vec3(0,1,0), glm::vec3(1.0f,1.0f,1.0f), 1.0f);
	
	
	ImplicitSphere sphere(0.7,glm::vec3(1.5,-1.3,1)); //Red
	ImplicitSphere sphere2(0.7,glm::vec3(-2,-1.3,-1)); //Gray
	

	ImplicitPlane pfloor(2,glm::vec3(0,1,0)); // floor
	ImplicitPlane pleft(3,glm::vec3(1,0,0)); // left
	ImplicitPlane pright(3,glm::vec3(-1,0,0)); // right
	ImplicitPlane pceil(2,glm::vec3(0,-1,0)); // Ceiling
	ImplicitPlane pback(2,glm::vec3(0,0,1)); // Back
	ImplicitPlane pfront(40,glm::vec3(0,0,-1)); // front

	pfloor.material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1,1,1), 0.0f);
	pleft.material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 0.1f, 0.1f), glm::vec3(1,1,1), 0.0f);
	pright.material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(0.1f, 1.0f, 0.1f), glm::vec3(1,1,1), 0.0f);
	pceil.material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1,1,1), 0.0f);
	pback.material = Material(0.1f, 0.8f, 0.8f, glm::vec3(0.1,0.1,0.1), glm::vec3(0.25f, 0.25f, 0.75f), glm::vec3(1,1,1), 0.0f);
	pfront.material = Material(0.1f, 0.8f, 0.0f, glm::vec3(0.1,0.1,0.1), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1,1,1), 0.0f);
	

	sphere.material = Material(0.1f, 0.8f, 0.2f, glm::vec3(1,0,0), glm::vec3(0.7f, 0.2f, 0.2f), glm::vec3(1,1,1), 10.0f);
	sphere2.material = Material(0.0f, 0.75f, 0.5f, glm::vec3(0,0,1), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(1,1,1), 200.0f);
	Camera camera(WIDTH,HEIGHT,1,1);
	camera.SetFov(40);
	camera.LookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));

	Scene scene;
	scene.addImplicitObject(&pfloor);
	scene.addImplicitObject(&pleft);
	scene.addImplicitObject(&pright);
	scene.addImplicitObject(&pceil);
	scene.addImplicitObject(&pback);
	scene.addImplicitObject(&pfront);

	scene.addImplicitObject(&sphere);
	scene.addImplicitObject(&sphere2);
	scene.addPointLight(&light);
	scene.setCamera(&camera);
	
	Img image(WIDTH,HEIGHT);

	WhittedRayTracer tracer(1,1,1);
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

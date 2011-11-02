#ifndef CAMERA_H
#define CAMERA_H

#define GLM_SWIZZLE 
#include "glm.hpp"

class Camera
{
public:
	Camera();
	Camera(int,int,float,float);
	~Camera();
	void LookAt(glm::vec3 _position, glm::vec3 _target, glm::vec3 _updirection);
	glm::vec3 planeBLeft,planeBRight,planeTLeft,planeTRight,origin;
	glm::vec3 pixelDx,pixelDy;
	void SetFov(float degrees);
	void SetImagePlaneWidthHeight(float width,float height);
	void SetPixelWidthHeight(unsigned int width, unsigned int height);	
private:
	void SetImagePlaneCoords();
	unsigned int pixelWidth,pixelHeight;
	float planeWidth,planeheight;
	glm::mat4x4 ViewMtx;

};


#endif
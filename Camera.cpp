#include "Camera.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/swizzle.hpp"
#include "gtc/matrix_inverse.hpp"
#include <math.h>

Camera::Camera() : pixelHeight(800), pixelWidth(800), planeheight(1), planeWidth(1),origin(glm::vec3(0,0,-2))
{
	SetImagePlaneCoords();
}

Camera::Camera(int pixelHeight,int pixelWidth,float planeheight,float planeWidth)
	: pixelHeight(pixelHeight), pixelWidth(pixelWidth), planeheight(planeheight), planeWidth(planeWidth), origin(glm::vec3(0,0,-10))
{
	SetImagePlaneCoords();
}

Camera::~Camera()
{

}

void Camera::SetImagePlaneCoords()
{
	float halfwidth = planeWidth*0.5f;
	float halfheight = planeheight*0.5f;
	planeBLeft = glm::vec3(-halfwidth,-halfheight,0);
	planeBRight =  glm::vec3(halfwidth,-halfheight,0);
	planeTLeft = glm::vec3(-halfwidth,halfheight,0);
	planeTRight = glm::vec3(halfwidth,halfheight,0);
}

void Camera::SetFov(float degrees)
{
	origin.z = -(planeWidth*0.5f / tan(degrees * 0.5 * 3.1416 / 180));
}

void Camera::SetImagePlaneWidthHeight(float width,float height)
{
	planeWidth = width;
	planeheight = height;
	SetImagePlaneCoords();
}

void Camera::SetPixelWidthHeight(unsigned int width, unsigned int height)
{
	pixelWidth = width;
	pixelHeight = height;
}

void Camera::LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 updirection)	
{
	glm::mat4x4 Vr = glm::mat4x4(0.0f);
	glm::mat4x4 Vt = glm::mat4x4(0.0f);

	//Negative z points into the screen
	glm::vec3 zaxis = glm::normalize(target-position);
	//yaxis must be orthogonal to the zaxis! 
	glm::vec3 yaxis = glm::normalize(updirection - glm::dot(updirection,zaxis)*zaxis);
	glm::vec3 xaxis = glm::cross(zaxis,yaxis);

	//Rotation,
	Vr[0][0] = xaxis.x; Vr[1][0] = xaxis.y; Vr[2][0] = xaxis.z; Vr[3][0] = 0; //row 1
	Vr[0][1] = yaxis.x; Vr[1][1] = yaxis.y; Vr[2][1] = yaxis.z; Vr[3][1] = 0; //row 2
	Vr[0][2] = zaxis.x; Vr[1][2] = zaxis.y; Vr[2][2] = zaxis.z; Vr[3][2] = 0; //row 3
	Vr[3][3] = 1; //last row is initiated to zero thus we only need to set ViewMat(3,3) = 1

	Vt[0][0] = 1; Vt[1][1] = 1; Vt[2][2] = 1; Vt[3][3] = 1; //Diagonal is (1,1,1)
	Vt[3][0] = -position.x; Vt[3][1] = -position.y; Vt[3][2] = -position.z;	
	
	ViewMtx = glm::mat4x4(0);
	ViewMtx = Vr*Vt;
	glm::mat4x4 invVievMtx = glm::affineInverse(ViewMtx);
	
	origin = (invVievMtx * glm::vec4(origin,1)).xyz;
	planeBLeft = (invVievMtx * glm::vec4(planeBLeft,1)).xyz;
	planeBRight =  (invVievMtx * glm::vec4(planeBRight,1)).xyz;
	planeTLeft = (invVievMtx * glm::vec4(planeTLeft,1)).xyz;
	planeTRight = (invVievMtx * glm::vec4(planeTRight,1)).xyz;

	pixelDx = glm::vec3((planeBRight - planeBLeft)/(float)pixelWidth);
	pixelDy = glm::vec3((planeTLeft-planeBLeft)/(float)pixelHeight);
}

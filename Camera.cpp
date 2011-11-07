#include "Camera.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/swizzle.hpp"
#include "gtc/matrix_inverse.hpp"
#include <cmath>

Camera::Camera() : pixelHeight(800), pixelWidth(800), planeheight(1.0), planeWidth(1.0),position(cbh::vec3(0,0,-2))
{
	SetImagePlaneCoords();
}

Camera::Camera(int pixelHeight,int pixelWidth,double planeheight,double planeWidth)
	: pixelHeight(pixelHeight), pixelWidth(pixelWidth), planeheight(planeheight), planeWidth(planeWidth), position(cbh::vec3(0,0,-2))
{
	SetImagePlaneCoords();
}

Camera::~Camera()
{

}

const cbh::vec3 & Camera::getPosition() const
{
	return position;
}

const cbh::vec3 & Camera::getImagePlaneBL() const
{
	return planeBLeft;
}

const cbh::vec3 & Camera::getImagePlaneBR() const
{
	return planeBRight;
}

const cbh::vec3 & Camera::getImagePlaneTL() const
{
	return planeTLeft;
}

const cbh::vec3 & Camera::getImagePlaneTR() const
{
	return planeTRight;
}

const cbh::vec3 & Camera::getPixelDx() const
{
	return pixelDx;
}
const cbh::vec3 & Camera::getPixelDy() const
{
	return pixelDy;
}

void Camera::SetImagePlaneCoords()
{
	float halfwidth = planeWidth*0.5f;
	float halfheight = planeheight*0.5f;
	planeBLeft = cbh::vec3(-halfwidth,-halfheight,0);
	planeBRight =  cbh::vec3(halfwidth,-halfheight,0);
	planeTLeft = cbh::vec3(-halfwidth,halfheight,0);
	planeTRight = cbh::vec3(halfwidth,halfheight,0);
}

void Camera::SetFov(float degrees)
{
	position.setZ(-(planeWidth*0.5f / tan(degrees * 0.5 * 3.1416 / 180)));
}

void Camera::SetImagePlaneWidthHeight(double width,double height)
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

void Camera::LookAt(cbh::vec3 _position, cbh::vec3 target, cbh::vec3 updirection)	
{
	//@todo:	All dependency on glm should be removed
	//			But then we have to implement a 4x4 matrix whiiiiich is not a proority
	//			But this method uses the gml mat4x4 class just because the have a inverse method =)

	

	//Negative z points into the screen
	cbh::vec3 zaxis = (target-_position).normalize();
	//yaxis must be orthogonal to the zaxis! 
	cbh::vec3 yaxis = updirection - (updirection*zaxis) * zaxis;
	cbh::vec3 xaxis = zaxis.cross(yaxis);


	glm::mat4x4 Vr = glm::mat4x4(0.0f);
	glm::mat4x4 Vt = glm::mat4x4(0.0f);

	//Rotation,
	Vr[0][0] = xaxis.getX(); Vr[1][0] = xaxis.getY(); Vr[2][0] = xaxis.getZ(); Vr[3][0] = 0; //row 1
	Vr[0][1] = yaxis.getX(); Vr[1][1] = yaxis.getY(); Vr[2][1] = yaxis.getZ(); Vr[3][1] = 0; //row 2
	Vr[0][2] = zaxis.getX(); Vr[1][2] = zaxis.getY(); Vr[2][2] = zaxis.getZ(); Vr[3][2] = 0; //row 3
	Vr[3][3] = 1; //last row is initiated to zero thus we only need to set ViewMat(3,3) = 1

	Vt[0][0] = 1; Vt[1][1] = 1; Vt[2][2] = 1; Vt[3][3] = 1; //Diagonal is (1,1,1)
	Vt[3][0] = -_position.getX(); Vt[3][1] = -_position.getY(); Vt[3][2] = -_position.getZ();	
	
	glm::mat4x4 ViewMtx(0);
	ViewMtx = Vr*Vt;
	glm::mat4x4 invVievMtx = glm::affineInverse(ViewMtx);
	
	glm::vec4 originTemp = invVievMtx * glm::vec4(position.getX(),position.getY(),position.getZ(),1);
	position.setX(originTemp.x),position.setY(originTemp.y),position.setZ(originTemp.z);

	glm::vec4 planeBLeftTemp = invVievMtx * glm::vec4(planeBLeft.getX(),planeBLeft.getY(),planeBLeft.getZ(),1);
	planeBLeft.setX(planeBLeftTemp.x),planeBLeft.setY(planeBLeftTemp.y),planeBLeft.setZ(planeBLeftTemp.z);

	glm::vec4 planeBRightTemp =  invVievMtx * glm::vec4(planeBRight.getX(),planeBRight.getY(),planeBRight.getZ(),1);
	planeBRight.setX(planeBRightTemp.x),planeBRight.setY(planeBRightTemp.y),planeBRight.setZ(planeBRightTemp.z);

	glm::vec4 planeTLeftTemp = invVievMtx * glm::vec4(planeTLeft.getX(),planeTLeft.getY(),planeTLeft.getZ(),1);
	planeTLeft.setX(planeTLeftTemp.x),planeTLeft.setY(planeTLeftTemp.y),planeTLeft.setZ(planeTLeftTemp.z);

	pixelDx = (planeBRight - planeBLeft)/(double)pixelWidth;
	pixelDy = (planeTLeft-planeBLeft)/(double)pixelHeight;
}

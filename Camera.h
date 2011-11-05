#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
	Camera();
	Camera(int,int,double,double);
	~Camera();
	void LookAt(cbh::vec3 _position, cbh::vec3 _target, cbh::vec3 _updirection);
	void SetFov(float degrees);
	void SetImagePlaneWidthHeight(double width,double height);
	void SetPixelWidthHeight(unsigned int width, unsigned int height);	
	void SetImagePlaneCoords();
	const cbh::vec3 & getPosition() const;
	const cbh::vec3 & getImagePlaneBL() const;
	const cbh::vec3 & getImagePlaneBR() const;
	const cbh::vec3 & getImagePlaneTL() const;
	const cbh::vec3 & getImagePlaneTR() const;
	const cbh::vec3 & getPixelDx() const;
	const cbh::vec3 & getPixelDy() const;
private:
	cbh::vec3 pixelDx,pixelDy;
	cbh::vec3 planeBLeft,planeBRight,planeTLeft,planeTRight,position;
	unsigned int pixelWidth,pixelHeight;
	double planeWidth,planeheight;

};


#endif
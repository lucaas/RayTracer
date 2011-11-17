#ifndef IMAGE_H
#define IMAGE_H

#include "Vector3.h"

class Img
{	
public:
	Img();
	Img(unsigned int width, unsigned int height);
	Img(unsigned int width, unsigned int height, char * filename);
	~Img();
	cbh::vec3uc & operator()(unsigned int x, unsigned int y);	
	void Save();	
	void setPixel(cbh::vec3uc color,unsigned int x,unsigned int y);
	unsigned int width, height;

	inline cbh::vec3uc * getImagePointer() { return image; }

private:
	char * filename;
	cbh::vec3uc * image;
};

#endif
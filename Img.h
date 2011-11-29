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
	void setHdrPixel(cbh::vec3 color,unsigned int x,unsigned int y);
	void toneMap();
	unsigned int width, height;

	inline cbh::vec3uc * getImagePointer() { return image; }

private:
	cbh::vec3 maxRadiance;
	char * filename;
	cbh::vec3uc * image;
	cbh::vec3	* hdrimage;
};

#endif
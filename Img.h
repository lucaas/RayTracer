#ifndef IMAGE_H
#define IMAGE_H

#include "ColorRGB.h"

class Img
{	
public:
	Img();
	Img(unsigned int width, unsigned int height);
	Img(unsigned int width, unsigned int height, char * filename);
	~Img();
	ColorRGB & operator()(unsigned int x, unsigned int y);	
	void Save();	
	unsigned int width, height;
private:
	char * filename;
	ColorRGB * image;
};

#endif

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "Img.h"

Img::Img(unsigned int width,unsigned int height) : width(width), height(height), filename("render.ppm")
{
	image = new cbh::vec3uc[width*height];
	hdrimage = new cbh::vec3[width*height];
	for(int i = 0; i<width*height; ++i)
	{
		image[i] = cbh::vec3uc(0,0,0);
		hdrimage[i] = cbh::vec3(0,0,0);
	}
}

Img::Img(unsigned int width, unsigned int height, char * filename) : width(width), height(height), filename("render.ppm")
{
	image = new cbh::vec3uc[width*height];
	for(int i = 0; i<width*height; ++i)
	{
		image[i] = cbh::vec3uc(1,0,1);
	}
}

Img::~Img()
{
	delete [] image;
	filename = NULL;
	delete filename;
}

cbh::vec3uc & Img::operator ()(unsigned int x, unsigned int y)
{
	unsigned int offset = x + y*width;
	return image[offset];
}

void Img::setPixel(cbh::vec3uc color,unsigned int x,unsigned int y)
{
	unsigned int offset = x + y*width;
	image[offset] = cbh::vec3uc(color);
}

void Img::setHdrPixel(cbh::vec3 color,unsigned int x,unsigned int y)
{
	if(color.squareNorm() > maxRadiance.squareNorm())
		maxRadiance = color;

	unsigned int offset = x + y*width;
	hdrimage[offset] = color;
}

void Img::toneMap()
{
	cbh::vec3 scalevec = 1/maxRadiance;
	for(int i = 0; i<width*height; ++i)
	{
		image[i].setX((unsigned char)(hdrimage[i][0] * scalevec[0])*255);
		image[i].setY((unsigned char)(hdrimage[i][1] * scalevec[1])*255);
		image[i].setZ((unsigned char)(hdrimage[i][2] * scalevec[2])*255);
	}
}

void Img::Save()
{
	FILE *fp = fopen(filename, "wb"); /* b - binary mode */
	(void) fprintf(fp, "P6\n%d %d\n255\n", width, height);
	for (int y = height-1; y >= 0; --y)
	{
		for (int x = 0; x < width; ++x)
		{
			static unsigned char color[3];
			color[0] = image[y*width + x].getX();
			color[1] = image[y*width + x].getY();
			color[2] = image[y*width + x].getZ();
			(void) fwrite(color, 1, 3, fp);
		}
	}
	(void) fclose(fp);
}
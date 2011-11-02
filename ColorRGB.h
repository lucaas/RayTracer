#ifndef COLOR_RGB
#define COLOR_RGB

#include "glm.hpp"

struct ColorRGB {

	ColorRGB() : r(0), g(0), b(0) {}
	ColorRGB(unsigned char val) : r(val), g(val), b(val) {}
	ColorRGB(unsigned char r, unsigned char g, unsigned char b) :r(r),g(g),b(b) {}
	unsigned char r,g,b;
};


#endif
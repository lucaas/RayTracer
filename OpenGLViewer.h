#ifndef OPEN_GL_VIEWER_H
#define OPEN_GL_VIEWER_H

#include <stdlib.h>
#include <GL/glfw.h>
#include "Img.h"

class OpenGLViewer
{
public:
	OpenGLViewer(const unsigned int width, const unsigned int height);
	void draw(Img *img);
	void stop();

private:
	void init();
	void setupTexture();
	void shutDown(int return_code);

	bool shouldDraw;


	unsigned int width, height;

	inline int index(int x, int y, int c) {
		return x*3 + y*width*3 + c;
	}
};

#endif

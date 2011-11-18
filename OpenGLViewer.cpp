#include "OpenGLViewer.h"

OpenGLViewer::OpenGLViewer(const unsigned int width, const unsigned int height) : width(width), height(height) {
	//unsigned __int8 ***screenData
	screenData = new GLubyte[width*height*3];
	init();
}

void OpenGLViewer::init() {

	if (glfwInit() != GL_TRUE)
		shutDown(1);

	// 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
	if (glfwOpenWindow(width, height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
		shutDown(1);

	glfwSetWindowTitle("RayTracer");
 
	// set the projection matrix to 2D orthogonal
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);        
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);

	setupTexture();
}

// Setup Texture
void OpenGLViewer::setupTexture()
{
	// Clear screen
	for(int y = 0; y < height; ++y)		
		for(int x = 0; x < width; ++x)
			screenData[index(x,y,0)] = screenData[index(x,y,1)] = screenData[index(x,y,2)] = 0;
 
	// Create a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)screenData);
 
	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
 
	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void OpenGLViewer::draw(Img *img) {
	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	// Draw pixels to texture
	cbh::vec3uc *image = img->getImagePointer();
	for(int y = 0; y < height; ++y)		
		for(int x = 0; x < width; ++x) {
			screenData[index(x,y,0)] = image[y*width + x].getX();
			screenData[index(x,y,1)] = image[y*width + x].getY();
			screenData[index(x,y,2)] = image[y*width + x].getZ();
		}

 
	// Update Texture
	glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenData);
 

	// Draw textured full-screen quad
	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,	0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(width, height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,	height);
	glEnd();


	// Swap buffers!
	glfwSwapBuffers();
}

void OpenGLViewer::shutDown(int return_code) {
	glfwTerminate();
	exit(return_code);
}
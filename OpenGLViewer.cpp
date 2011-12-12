#include "OpenGLViewer.h"

OpenGLViewer::OpenGLViewer(const unsigned int width, const unsigned int height) : width(width), height(height) {
	init();
}

void OpenGLViewer::setLoop(bool loop) {
	shouldDraw = loop;
}

void resize(int x, int y)
{
	//std::cout << "inside callback" << std::endl;
	
	glfwSetWindowSize(x, y);
}


void OpenGLViewer::init() {

	if (glfwInit() != GL_TRUE)
		shutDown(1);

	// 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
	if (glfwOpenWindow(width, height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
		shutDown(1);

	glfwSetWindowSizeCallback(&resize);

	glfwSetWindowTitle("RayTracer");
 
	// set the projection matrix to 2D orthogonal
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);        
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);

	setupTexture();
	shouldDraw = false;

}

// Setup Texture
void OpenGLViewer::setupTexture()
{
	// Create a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)NULL);
 
	// Set up the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
 
	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

// Main loop, update texture and draw quad
void OpenGLViewer::draw(Img *img) {
	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	cbh::vec3uc *image = img->getImagePointer();
	
	do {

		// Update Texture
		glTexSubImage2D(GL_TEXTURE_2D, 0 ,0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
		
 
		// Draw textured full-screen quad
		glBegin( GL_QUADS );
			glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,	0.0);
			glTexCoord2d(1.0, 0.0); 	glVertex2d(width, 0.0);
			glTexCoord2d(1.0, 1.0); 	glVertex2d(width, height);
			glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,	height);
		glEnd();

		// Swap buffers!
		glfwSwapBuffers();
	
	
	GLenum errCode;
	errCode = glGetError();
	if (errCode != GL_NO_ERROR)
	{
		printf("OpenGL ERROR: %s\n", gluErrorString(errCode));
		break;
	}
	
	
	} while(shouldDraw);
}

void OpenGLViewer::shutDown(int return_code) {
	glfwTerminate();
	exit(return_code);
}


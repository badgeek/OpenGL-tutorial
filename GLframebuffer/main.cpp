/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Putting It All Together" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */



#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"


Image* myimg;
Image* myimg2;

using namespace std;
GLuint _textureId[3];           //The OpenGL id of the 
GLuint _fboDepth;
GLuint FBO;

#define checkImageWidth 64 
#define checkImageHeight 64

const unsigned int TEXTURE_WIDTH = 640;
const unsigned int TEXTURE_HEIGHT = 480;

unsigned int width;
unsigned int height;
unsigned int angle;


GLubyte checkImage[checkImageHeight][checkImageWidth][4];

unsigned int pmode = 0;

void handleResize(int w, int h);

void makeCheckImage(void) {
int i, j, c;
for (i = 0; i < checkImageHeight; i++) 
	{ 
		for (j = 0; j < checkImageWidth; j++) 
		{ 
			c = (((i&0x8)==0)^((j&0x8))==0)*255;
			checkImage[i][j][0] = (GLubyte) c; 
			checkImage[i][j][1] = (GLubyte) c; 
			checkImage[i][j][2] = (GLubyte) c; 
			checkImage[i][j][3] = (GLubyte) 255;
		}
	}
}


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	myimg = loadBMP("vtr.bmp");
	myimg2 = loadBMP("face.bmp");

	//generate image
	makeCheckImage();
	
	//texture setup --- TODO ?????
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//allocate 3 texture id
	glGenTextures(3, &_textureId[0]);

	//upload image to first texture (mask)
	glBindTexture(GL_TEXTURE_2D, _textureId[0]);
		//after bind all gltexparam affect to _textureID
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myimg->width, myimg->height, 0, GL_RGB, GL_UNSIGNED_BYTE, myimg->pixels); //no alpha channel
	glBindTexture(GL_TEXTURE_2D, 0);	

	//upload image to second texture (face)
	glBindTexture(GL_TEXTURE_2D, _textureId[1]);
		//after bind all gltexparam affect to _textureID
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myimg2->width, myimg2->height, 0, GL_RGB, GL_UNSIGNED_BYTE, myimg2->pixels); //no alpha channel
	glBindTexture(GL_TEXTURE_2D, 0);	



	//generate texture for fbo attach 640x480
	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	//generate renderbuffer for vbo 640x480
	glGenRenderbuffers(1, &_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, _fboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	
	//generate framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId[2], 0); //attach clor buffer to current FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _fboDepth); //attach depth buffer to current FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	glPointSize(10);
	
}

//Called every 25 milliseconds
void update(int value) {
	//update here
	angle += 10;
	angle %= 360;
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}



void draw()
{

	//!IMPORTANT
	//gl viewport, perspective have to be adjusted to texture width/height
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	
		//framebuffer obj has its own modelview/projection
		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, 640, 480);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, (GLfloat)640/(GLfloat)480, 0.1, 40.0);
		glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear framebuffer
		glPushMatrix();
			glTranslatef(0, 0, -2.0);
			glRotatef(angle, 0, 1, 0);
			glRotatef(30, 1, 0, 0);
			glutWireTeapot(0.2);
		glPopMatrix();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, _textureId[2]);
	glGenerateMipmap(GL_TEXTURE_2D);
	handleResize(width,height);
	glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(0, 0, -2.0);
		//glScalef(1.5, 1.5, 1);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(-0.5, -0.5);
				glTexCoord2f(0.0, 1.0); glVertex2f(-0.5, 0.5);
				glTexCoord2f(1.0, 1.0); glVertex2f(0.5, 0.5);
				glTexCoord2f(1.0, 0.0); glVertex2f(0.5, -0.5);
			glEnd();
		glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);



}



void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	draw();
	glutSwapBuffers();
}


void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
	}
}


void handleResize(int w, int h) {

	width = w;
	height = h;

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (GLfloat)w/(GLfloat)h, 0.1, 40.0);
	glMatrixMode(GL_MODELVIEW);
	/*
	gluPerspective
	fovy	Specifies the field of view angle, in degrees, in the y direction.
	aspect	Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
	zNear	Specifies the distance from the viewer to the near clipping plane (always positive).
	zFar	Specifies the distance from the viewer to the far clipping plane (always positive).
	*/

}


void windowMenu(int k)
{
	if (k == 'o'){
		pmode = 1;
		cout << "ortho" << endl;
	}
	if (k == 'p') {
		pmode = 0;
		cout << "persp" << endl;
	}

	cout << k << endl;
}


void exitCB()
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(3, _textureId);
	glDeleteRenderbuffers(1, &_fboDepth);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	atexit(exitCB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	
	glutCreateWindow("Putting It All Together - videotutorialsrock.com");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutCreateMenu(windowMenu);
	glutAddMenuEntry("projection Ortho", 'o');
	glutAddMenuEntry("projection Perspective", 'p');
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}










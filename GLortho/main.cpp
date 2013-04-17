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
GLuint _textureId[2];           //The OpenGL id of the 

#define checkImageWidth 64 
#define checkImageHeight 64

GLubyte checkImage[checkImageHeight][checkImageWidth][4];

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
	glDisable(GL_DEPTH_TEST);
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	myimg = loadBMP("vtr.bmp");
	myimg2 = loadBMP("face.bmp");

	//generate image
	makeCheckImage();
	
	//texture setup --- TODO ?????
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//allocate 2 texture id
	glGenTextures(2, &_textureId[0]);

	//upload image to first texture (mask)
	glBindTexture(GL_TEXTURE_2D, _textureId[0]);
		//after bind all gltexparam affect to _textureID
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myimg->width, myimg->height, 0, GL_RGB, GL_UNSIGNED_BYTE, myimg->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);	

	//upload image to second texture (face)
	glBindTexture(GL_TEXTURE_2D, _textureId[1]);
		//after bind all gltexparam affect to _textureID
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myimg2->width, myimg2->height, 0, GL_RGB, GL_UNSIGNED_BYTE, myimg2->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);	

	glPointSize(10);
	
}

//Called every 25 milliseconds
void update(int value) {
	//update here

	glutTimerFunc(25, update, 0);
}



void draw()
{

	glutWireCube(0.6);

	
	glBegin(GL_POINTS);
		glVertex2f(0, 0);
		glVertex2f(1, 1); // top right
		glVertex2f(-1, 1); // top left
		glVertex2f(1, -1); // bottom right
		glVertex2f(-1, -1); // bottom left//
	glEnd();



}



void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -20.0f);
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
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(1, 1, 1, 1, -1,1);
	//gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
	//gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
	//gluOrtho2D (0.0, 1.0, 0.0, 1.0*(GLfloat)h/(GLfloat)w);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	
	glutCreateWindow("Putting It All Together - videotutorialsrock.com");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}










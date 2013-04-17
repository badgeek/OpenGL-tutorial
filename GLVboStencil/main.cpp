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

using namespace std;

const float BOX_SIZE = 7.0f; //The length of each side of the cube
float _angle = 0;            //The rotation of the box
GLuint _textureId;           //The OpenGL id of the texture


float rotX;
float rotY;
float rotZ;

GLfloat vertices[] = {
						-2.0, -2.0, 0.0, //0
					     2.0, -2.0, 0.0, //1
						 2.0,  2.0, 0.0,//2
						-2.0,  2.0, 0.0,//3
						-2.0, -2.0, -4.0,//4
					     2.0, -2.0, -4.0,//5
						 2.0,  2.0, -4.0,//6
						-2.0,  2.0, -4.0//7
					 };

// 7 6
// 7-3 2-6  7 6
// 4-0 1-5  4 5
//   4 5

GLuint indexes[] = {
				   		0,1,2,
						2,3,0,

						7,3,2,
						6,7,2,

						5,6,2,
						2,1,5,

						6,5,4,
						4,7,6,

						4,0,3,
						3,7,4,

						1,0,4,
						4,5,1


					};

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);

	glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glColorMask(0,0,0,0);
		glDepthMask(GL_FALSE);
		glClear(GL_STENCIL_BUFFER_BIT);

			glEnableClientState(GL_INDEX_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indexes);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_INDEX_ARRAY);

		glStencilFunc(GL_EQUAL, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glColorMask(1,1,1,1);
		glDepthMask(GL_TRUE);	
		glStencilMask(0x00);
		

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPushMatrix();
				glRotatef(rotX,1,0,0);
				glRotatef(rotY,0,1,0);
				glRotatef(rotZ,0,0,1);
				glEnableClientState(GL_INDEX_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, vertices);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indexes);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_INDEX_ARRAY);
			glPopMatrix();

	glDisable(GL_STENCIL_TEST);

	glutSwapBuffers();
}






void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
		case 'w':
			rotX += 10.0;
		break;
		case 's':
			rotX -= 10.0;
		break;
		case 'q':
			rotZ += 10.0;
		break;
		case 'e':
			rotZ -= 10.0;
		break;	
		case 'a':
			rotY += 10.0;
		break;
		case 'd':
			rotY -= 10.0;
		break;		
	}
}


void initRendering() {
	glEnable(GL_DEPTH_TEST);
}

void handleResize(int w, int h) {
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

//Called every 25 milliseconds
void update(int value) {
	_angle += 1.0f;
	if (_angle > 360) {
		_angle -= 360;
	}
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {

	rotX = 0;
	rotY = 0;
	rotZ = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
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










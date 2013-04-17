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




GLfloat vertex_buffer[] = { -1.0,-1.0, 0.0,
							-1, 1.0, 0.0,
							 1.0, 1.0, 0.0,
							 1.0,-1.0, 0.0
						  };

GLuint vbo_id;


GLuint createVertexBuffer(GLfloat (*vertices), int num)
{
	GLuint buff;
	glGenBuffersARB(1, &buff);
	glBindBufferARB(GL_ARRAY_BUFFER, buff);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(GLfloat)*num, vertices, GL_STATIC_DRAW);
	//											^------------ FCKING IMPORTANT!!!!, read array decay..
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	return buff;
}



void initRendering() {
	glEnable(GL_DEPTH_TEST);
	vbo_id = createVertexBuffer(vertex_buffer,12);
	glPointSize(2);
	glColor3f(1, 0, 0);
}


void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0, 0, -10);
	glColor3f(0, 1, 0);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_id);
		glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, 0); //set vertexpointer setting & use vertex_id
			glDrawArrays(GL_QUADS, 0, 4 );
		glDisableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
	glutSwapBuffers();
}



//Called every 25 milliseconds
void timerFunc(int value) {
	glutPostRedisplay();
	glutTimerFunc(25, timerFunc, 0);
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
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}


int main(int argc, char** argv) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Putting It All Together - videotutorialsrock.com");
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	//glutTimerFunc(25, timerFunc, 0);
	glutDisplayFunc(drawScene);
	initRendering();
	glutMainLoop();
	return 0;
}

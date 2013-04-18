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
#include "matrix.h"

using namespace std;

const float BOX_SIZE = 7.0f; //The length of each side of the cube
float _angle = 0;            //The rotation of the box
GLuint _textureId;           //The OpenGL id of the texture


float rotX;
float rotY;
float rotZ;

Matrix4x4 world_mat;
Matrix4x4 model_mat;
Matrix4x4 rot_mat;

void setup()
{
    world_mat.setIdentity();
    model_mat.setIdentity();
	rot_mat.setIdentity();

   // world_mat.rotateY(-90) ;
    world_mat.translate(0,0,-10);

	//rot_mat.rotateX(5);
    //model_mat.translate(10,10,0);
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(world_mat.getGlMat());

    glPushMatrix();

	model_mat *= rot_mat;
	rot_mat.setIdentity();

    glMultMatrixf(model_mat.getGlMat());
    glPushMatrix();
    glRotatef(90,0.0,0.0,1.0); //z
    glRotatef(90,1.0,0.0,0.0); //z
    glutWireTeapot(2.0);
	//glutWireCube(4.0);
    glPopMatrix();
    glPopMatrix();

    //square box
    /*glScalef(5, 5, 1);
    glBegin(GL_QUADS);
    	glVertex2f(-0.5, 0.5);
    	glVertex2f(0.5, 0.5);
    	glVertex2f(0.5, -0.5);
    	glVertex2f(-0.5, -0.5);
    glEnd();*/

    glutSwapBuffers();
}






void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 27: //Escape key
        exit(0);
    case 'w':
        rotX += 10.0;
        rot_mat.rotateX(10);
        break;
    case 's':
        rotX -= 10.0;
        rot_mat.rotateX(-10);
        break;
    case 'q':
        rotZ += 10.0;
        rot_mat.rotateZ(10);
		
        break;
    case 'e':
        rotZ -= 10.0;
        rot_mat.rotateZ(-10);
		
        break;
    case 'a':
        rotY += 10.0;
        rot_mat.rotateY(10);
        break;
    case 'd':
        rotY -= 10.0;
        rot_mat.rotateY(-10);
        break;
	case 'x':
        rot_mat.print();
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
      //  model_mat.rotateY(2);	
    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {

    rotX = 0;
    rotY = 0;
    rotZ = 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);

    glutCreateWindow("Putting It All Together - videotutorialsrock.com");
    initRendering();
    setup();
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}










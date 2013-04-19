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
#include "util.cpp"

int width = 400;
int height = 400;



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


Vector3 vOrigin(0,0,0);
Vector3 vMouse(0,0,0);
Vector3 vMousePrev(0,0,0);
Vector3 vAxis(0,0,0);

bool isDragged = false;

void setup()
{
    world_mat.setIdentity();
    model_mat.setIdentity();
	rot_mat.setIdentity();

   // world_mat.rotateY(-90) ;
    world_mat.translate(0,0,-10);
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(world_mat.getGlMat());

    glPushMatrix();
	
	model_mat *= rot_mat;
	//rot_mat.rotateY(rotY);
    glMultMatrixf(model_mat.getGlMat());
	rot_mat.setIdentity();

	
    glPushMatrix();
    glRotatef(90,0.0,0.0,1.0); //z
    glRotatef(90,1.0,0.0,0.0); //z
    glutWireSphere(2.0,10,10);
	//glutWireCube(4.0);
    glPopMatrix();
	

    //square box
    //glScalef(5, 5, 1);
	//
	//
	
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		Vector3 tmp = vAxis;
		glVertex3f(vAxis.x,-vAxis.y, -vAxis.z );
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(vMouse.x,-vMouse.y, -vMouse.z );
	glEnd();


		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(vMousePrev.x,-vMousePrev.y, -vMousePrev.z );
	glEnd();



   /* glBegin(GL_QUADS);
    	glVertex2f(-0.5, 0.5);
    	glVertex2f(0.5, 0.5);
    	glVertex2f(0.5, -0.5);
    	glVertex2f(-0.5, -0.5);
    glEnd();
*/
	glPopMatrix();

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



Vector3 arcGetVec(int x, int y)
{
 float _x = (((float) x/ (float)width) -0.5)*2;
 float _y = (((float) y/ (float)height) -0.5)*2;
 float _z = 0;
 
 float square_d = _x*_x + _y*_y;

 if (square_d <1.0)
 {
  _z = sqrt( (0.5) - (square_d*0.5) );
 }else{
  _z = 0;
 }
	
 return Vector3(_x,_y,_z); 

}


void mouseMove(int x, int y)
{
		isDragged = true;
		cout << "drag" << endl;


		vMouse =  arcGetVec(x,y);


		float angle = vMouse.angleBetween(vMousePrev);
		printf("angle: %f\n", angle);

		vAxis  = vMouse.cross(vMousePrev);

		vAxis.normalize();
		vAxis.mult(10);
		
		//vMousePrev = vMouse;

}

void mouseClick(int button, int state, int x, int y)
{
		if (isDragged){
		isDragged = false;
		vMousePrev = vMouse;
		cout << "end drag" << endl;
		}else
		{
		cout << "start drag" << endl;
		vMousePrev = arcGetVec(x,y);	
		}
}


int main(int argc, char** argv) {

    rotX = 0;
    rotY = 0;
    rotZ = 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);

    glutCreateWindow("Putting It All Together - videotutorialsrock.com");
    initRendering();
    setup();

	//glutPassiveMotionFunc(mouseMove);	
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseClick);

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}








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

int width = 1024;
int height = 768;



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

Matrix4x4 this_rot;
Matrix4x4 last_rot;

Matrix4x4 rot_inverse;
Matrix4x4 rot_rot;

Vector3 vOrigin(0,0,0);
Vector3 vMouse(0,0,0);
Vector3 vMousePrev(0,0,0);
Vector3 vAxis(0,0,0);

bool isDragged = false;

void setup()
{
    world_mat.setIdentity();
    model_mat.setIdentity();

	this_rot.setIdentity();
	last_rot.setIdentity();

	//rot_inverse.setIdentity();

    // world_mat.rotateY(-90) ;
    world_mat.translate(0,0,-10);

}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_MODELVIEW);


	rot_inverse = world_mat;
	rot_inverse.inverse();

	glLoadIdentity();
    glMultMatrixf(world_mat.getGlMat());

	
    glPushMatrix();

    glMultMatrixf(rot_inverse.getGlMat());	
    glMultMatrixf(world_mat.getGlMat());


		glMultMatrixf(model_mat.getGlMat());
		glColor3f(0.0,1.0,0.0);
    	glPushMatrix();
			glTranslatef(0,0,1);
			glutSolidCube(1.0);
		glPopMatrix();
		glColor3f(1.0,0.0,0.0);
		glutSolidCube(2.0);
    glPopMatrix();
	
	rot_rot.setIdentity();

    //square box
    //glScalef(5, 5, 1);
    //
	//
	//
    /*
	glPushMatrix();
    	glBegin(GL_LINES);
    		glVertex3f(0,0,0);
    		glVertex3f(vAxis.x*10,-vAxis.y*10, -vAxis.z*10 );
    	glEnd();

    	glBegin(GL_LINES);
    		glVertex3f(0,0,0);
    		glVertex3f(vMouse.x*10,-vMouse.y*10, -vMouse.z *10);
    	glEnd();


    	glBegin(GL_LINES);
    		glVertex3f(0,0,0);
    		glVertex3f(vMousePrev.x*10,-vMousePrev.y*10, -vMousePrev.z*10 );
    	glEnd();
    glPopMatrix();
	*/
    glutSwapBuffers();
}






void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 27: //Escape key
        exit(0);
    case 'w':
        rotX += 10.0;
        rot_mat.rotateX(10);
		rot_rot.rotateX(10);	
		
        break;
    case 's':
        rotX -= 10.0;
        rot_mat.rotateX(-10);
		rot_rot.rotateX(-10);			
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
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Set Perspective Calculations To Most Accurate
	glClearDepth (1.0f);											// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);										// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);										// Enable Depth Testing
	glShadeModel (GL_FLAT);											// Select Flat Shading (Nice Definition Of Objects)
	glEnable(GL_LIGHT0);											// Enable Default Light
	glEnable(GL_LIGHTING);											// Enable Lighting
	glEnable(GL_COLOR_MATERIAL);	
}

void handleResize(int w, int h) {
	
		width = w;
		height = h;

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


float clamp(float _min, float _max,float _inp)
{
	if (_inp < _min) return _min;
	if (_inp > _max) return _max;

	return _inp;
}

Vector3 arcGetVec(int x, int y)
{
    float _x = (((float) x/ (float)width) -0.5)*2;
    float _y = (((float) y/ (float)height) -0.5)*2;
    float _z = 0;

	_x = clamp(-1.0, 1.0, _x);
	_y = clamp(-1.0, 1.0, _y);

    cout << "drag" << "x:" << _x << " y:" << _y << endl;
	

    float square_d = _x*_x + _y*_y;

    if (square_d <1.0)
    {
        _z = sqrt( (1) - (square_d) );
    } else {
        _z = 0;
    }

    return Vector3(_x,_y,_z);

}


void mouseMove(int x, int y)
{

    isDragged = true;
    vMouse =  arcGetVec(x,y);

    float angle = min(1.0f,vMouse.angleBetween(vMousePrev));
    
//	printf("angle: %f\n", angle);
    
	vAxis  = vMouse.cross(vMousePrev);
    vAxis.normalize();

	this_rot.setIdentity();	
	this_rot.rotate(angle * 100, vAxis.x, -vAxis.y, -vAxis.z);
	
	this_rot *= last_rot;
	model_mat = this_rot ;

	

  // vMousePrev = vMouse;
}

void mouseClick(int button, int state, int x, int y)
{
    if (isDragged) {
        isDragged = false;
        cout << "end drag" << endl;
		
    } else
    {
        cout << "start drag" << endl;
		last_rot = this_rot;
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








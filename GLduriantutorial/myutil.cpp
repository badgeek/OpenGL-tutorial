#include "myutil.h"



void vbotest()
{	
	GLuint testb;
	glGenBuffersARB(1,&testb);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, testb);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

}



GLuint createVertexBuffer(GLfloat (*vertices), int num)
{
	GLuint buff;
	glGenBuffersARB(1, &buff);
	glBindBufferARB(GL_ARRAY_BUFFER, buff);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(*vertices)*num, vertices, GL_STATIC_DRAW);
	//													^------------ FCKING IMPORTANT!!!!
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	return buff;
}




void handleResize(int w, int h) {
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

//Called every 25 milliseconds
void timerFunc(int value) {
	glutPostRedisplay();
	glutTimerFunc(25, timerFunc, 0);
}


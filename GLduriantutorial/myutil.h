
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif




void handleResize(int w, int h); 
void timerFunc(int value);
void handleKeypress(unsigned char key, int x, int y) ;
GLuint createVertexBuffer(GLfloat (*vertices), int num);
void vbotest();
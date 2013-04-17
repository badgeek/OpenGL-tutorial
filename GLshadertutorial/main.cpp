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

#include <stdlib.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>
#include "util.cpp"


static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f
};


GLuint texture1;

GLuint shader_prog;
GLuint vert_shader;
GLuint frag_shader;

GLint fade_shader;
GLint texture_shader;


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	texture1 = make_texture("hello1.tga");

	vert_shader = make_shader(GL_VERTEX_SHADER, "test.vs.glsl");
	frag_shader = make_shader(GL_FRAGMENT_SHADER, "test.fs.glsl");

	shader_prog = glCreateProgram();

	glAttachShader(shader_prog, vert_shader);
	glAttachShader(shader_prog, frag_shader);
	glLinkProgram(shader_prog);

	fade_shader = glGetUniformLocation(shader_prog, "fade_factor");
	texture_shader = glGetUniformLocation(shader_prog, "textures");

}


void render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.0f);

	//glColor3f(1.0, 0.0, 0.0);
	glActiveTexture(GL_TEXTURE0); 
	glUseProgram(shader_prog);

			glBindTexture(GL_TEXTURE_2D, texture1); //bind texture to current active GL_TEXTURE0
			glUniform1i(texture_shader, 0);	//tell shader to use GL_TEXTURE0
		
				glBegin(GL_QUADS);
					glTexCoord2f(0, 0); glVertex2f(-0.5, -0.5);
					glTexCoord2f(0, 1); glVertex2f(-0.5, 0.5);
					glTexCoord2f(1, 1); glVertex2f(0.5, 0.5);
					glTexCoord2f(1, 0); glVertex2f(0.5, -0.5);
				glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render();
	glutSwapBuffers();
}


void handleResize(int w, int h) {
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
	}
}


//Called every 25 milliseconds
void timerFunc(int value) {
	int milliseconds = glutGet(GLUT_ELAPSED_TIME);

	glUseProgram(shader_prog);
    	glUniform1f(fade_shader,  (GLfloat) sinf((float)milliseconds * 0.001f) + 1.f  );
    glUseProgram(0);

	glutPostRedisplay();
	glutTimerFunc(25, timerFunc, 0);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("shader tutorial");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, timerFunc, 0);
	initRendering();
	glutMainLoop();
	return 0;
}










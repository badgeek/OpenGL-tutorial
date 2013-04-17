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
#include <cstring>

typedef struct{
	GLuint vertex_buffer, element_buffer;
	GLuint textures[2];


	GLuint vertex_shader, fragment_shader, program;

	struct {
		GLint fade_factor;
		GLint textures[2];
	}uniforms;

	struct{
		GLint position;
	}attributes;

	GLfloat fade_factor;


}g_resources;


static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	-1.0f,  1.0f,
	 1.0f,  1.0f
};

static const GLushort g_element_buffer_data[] = {0,1,2,3};

g_resources gl_res;


//target -GL_ARRAY_BUFFER
//		 -GL_ELEMENT_ARRAY_BUFFER
static GLuint make_buffer( GLenum target, const void *buffer_data, int num)
{
	static GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, sizeof(GLfloat) * num, buffer_data, GL_STATIC_DRAW);
	//usage hint that indicates how often you intend to access and change 
	//the data in the buffer, and OpenGL decides the best place in CPU or GPU memory
	glBindBuffer(target, 0);
	return buffer;

    char source[] = "once upon a midnight dreary...";
    char dest[4];
    std::memcpy(dest, source, sizeof dest);
    //for (char c) {
      //  std::cout << c << '\n';
    //};

	
}

static GLuint make_texture(const char * filename)
{
	GLuint texture;
	int width, height;
	void * pixels = read_tga(filename, &width, &height);
	if (!pixels) {
		printf("texture fail\n");
		return 0;
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels); //upload pixel into opengl memory
	free(pixels); //delete pixels from cpu memory

	// GL_BGR = texture file color format
	// Our TGA files use 24-bit RGB pixels, in other words,
	// they sport three 8-bit components per pixel. This corresponds to the GL_RGB8 internal format. 
	
	glBindTexture(GL_TEXTURE_2D, 0);
	return 	texture;
}



GLuint make_shader(GLenum type, const char *filename)
{
	GLint length;
	GLchar *source = (GLchar*) file_contents(filename, &length);
	GLuint shader;
	GLint shader_ok;

	if(!source) return 0;

	shader = glCreateShader(type);

	glShaderSource(shader,1, (const GLchar**) &source, &length); //upload shader source to opengl
	free(source); //then we can free source here..

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);

    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s:\n", filename);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint make_program(const g_resources * shader_res)
{
	GLuint program;
	GLint status;

	program = glCreateProgram();
	glAttachShader(program, shader_res->fragment_shader);
	glAttachShader(program, shader_res->vertex_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	printf("program : %i\n", (int) status );

	return program;

}

void make_resources(  g_resources * buf_id, const GLfloat * vertex_array, const GLushort * element_array)
{
	//vertex buffer
	buf_id->vertex_buffer = make_buffer(GL_ARRAY_BUFFER, vertex_array, 12);				// BUG?????!!!! -> array decay
	buf_id->element_buffer = make_buffer(GL_ELEMENT_ARRAY_BUFFER, element_array, 12);	// 

	//make texture
	buf_id->textures[0] = make_texture("hello1.tga");
	buf_id->textures[1] = make_texture("hello2.tga");

	//make shader
	buf_id->vertex_shader = make_shader(GL_VERTEX_SHADER, "hello-gl.v.glsl");
	buf_id->fragment_shader = make_shader(GL_FRAGMENT_SHADER, "hello-gl.f.glsl");	

	//make program
	buf_id->program = make_program(buf_id);

	buf_id->uniforms.textures[0] = glGetUniformLocation(buf_id->program, "textures[0]");
	buf_id->uniforms.textures[1] = glGetUniformLocation(buf_id->program, "textures[1]");
	buf_id->uniforms.fade_factor = glGetUniformLocation(buf_id->program, "fade_factor");
	buf_id->attributes.position  = glGetAttribLocation(buf_id->program, "position");

}



void initRendering() {
	glEnable(GL_DEPTH_TEST);
	make_resources(&gl_res, g_vertex_buffer_data, g_element_buffer_data);
}


void render()
{
	glUseProgram(gl_res.program);

		glUniform1f(gl_res.uniforms.fade_factor, gl_res.fade_factor ); //everytime we render change fade factor...

		/*	OpenGL provides a family of glUniform* functions for assigning to uniform variables, 
			with each member corresponding to a possible type for a uniform variable in a GLSL program. 
			These functions all have names of the form glUniform{dim}{type}, 
			where the dim indicates the size of a vector type 
			(1 for an int or float uniform, 2 for a vec2, etc.) */

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gl_res.textures[0]); // BIND TEXTURE TO CURRENT ACTIVE TEXTURE/ GL_TEXTURE0

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gl_res.textures[1]);  // BIND TEXTURE TO CURRENT ACTIVE TEXTURE/ GL_TEXTURE1

		glUniform1i(gl_res.uniforms.textures[0], 0); //GL_TEXTURE0
		glUniform1i(gl_res.uniforms.textures[1], 1); //GL_TEXTURE1

		glBindBuffer(GL_ARRAY_BUFFER,gl_res.vertex_buffer);
		glVertexAttribPointer(
			gl_res.attributes.position,
			2, // size
			GL_FLOAT, //type
			GL_FALSE, //normalized?
			sizeof(GLfloat)*2, //stride
			(void*) 0); //array buffer obj

		glEnableVertexAttribArray(gl_res.attributes.position);
	 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_res.element_buffer);
	    glDrawElements(
	        GL_TRIANGLE_STRIP,  /* mode */
	        4,                  /* count */
	        GL_UNSIGNED_SHORT,  /* type */
	        (void*)0            /* element array buffer offset */
	    );

	    glDisableVertexAttribArray(gl_res.attributes.position);
	
	glUseProgram(0); //disable glsl


}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	render();


	/*
	glBindTexture(GL_TEXTURE_2D, gl_res.textures[0]);
	glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(-0.5, -0.5);
			glTexCoord2f(0, 1); glVertex2f(-0.5, 0.5);
			glTexCoord2f(1, 1); glVertex2f(0.5, 0.5);
			glTexCoord2f(1, 0); glVertex2f(0.5, -0.5);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
	

	glutSwapBuffers();
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
	}
}



void handleResize(int w, int h) {
	//glMatrixMode(GL_MODELVIEW);
	//glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

//Called every 25 milliseconds
void timerFunc(int value) {
	int milliseconds = glutGet(GLUT_ELAPSED_TIME);
    gl_res.fade_factor = sinf((float)milliseconds * 0.001f) * 0.5f + 0.5f;

	glutPostRedisplay();
	glutTimerFunc(25, timerFunc, 0);
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Putting It All Together - videotutorialsrock.com");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	//glutReshapeFunc(handleResize);
	glutTimerFunc(25, timerFunc, 0);

	initRendering();

	glutMainLoop();
	return 0;
}










#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include "display.h"
#include "linmath.h"
#include "texture.h"
#include "object.h"

void callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) {}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static GLfloat vertexBufferData[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static GLfloat uvBufferData[] = {
    0.000059f, 1.0f-0.000004f,
    0.000103f, 1.0f-0.336048f,
    0.335973f, 1.0f-0.335903f,
    1.000023f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.336024f, 1.0f-0.671877f,
    0.667969f, 1.0f-0.671889f,
    1.000023f, 1.0f-0.000013f,
    0.668104f, 1.0f-0.000013f,
    0.667979f, 1.0f-0.335851f,
    0.000059f, 1.0f-0.000004f,
    0.335973f, 1.0f-0.335903f,
    0.336098f, 1.0f-0.000071f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.336024f, 1.0f-0.671877f,
    1.000004f, 1.0f-0.671847f,
    0.999958f, 1.0f-0.336064f,
    0.667979f, 1.0f-0.335851f,
    0.668104f, 1.0f-0.000013f,
    0.335973f, 1.0f-0.335903f,
    0.667979f, 1.0f-0.335851f,
    0.335973f, 1.0f-0.335903f,
    0.668104f, 1.0f-0.000013f,
    0.336098f, 1.0f-0.000071f,
    0.000103f, 1.0f-0.336048f,
    0.000004f, 1.0f-0.671870f,
    0.336024f, 1.0f-0.671877f,
    0.000103f, 1.0f-0.336048f,
    0.336024f, 1.0f-0.671877f,
    0.335973f, 1.0f-0.335903f,
    0.667969f, 1.0f-0.671889f,
    1.000004f, 1.0f-0.671847f,
    0.667979f, 1.0f-0.335851f
};

static Object* cube;

static const GLchar* vertexShaderSrc = 
	"#version 330 core\n"

	"layout(location = 0) in vec3 model;\n"
	"layout(location = 1) in vec2 vertexUV;\n"

	"uniform mat4 mvp;\n"

	"out vec2 UV;\n"

	"void main() {\n"
		"gl_Position = mvp * vec4(model, 1);\n"
		"UV = vertexUV;\n"
	"}\n\0";

static const GLchar* fragmentShaderSrc = 
	"#version 330 core\n"

	"in vec2 UV;\n"

	"out vec3 color;\n"

	"uniform sampler2D textureSampler;\n"

	"void main() {\n"
		"color = texture(textureSampler, UV).rgb;\n"
	"}\n\0";

int buildProgram()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShaderID);

	GLint vertStatus = GL_FALSE;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertStatus);
	if(vertStatus == GL_FALSE)
	{
		printf("Vertex shader failed to compile, status %d\n", vertStatus);
		exit(-1);
	}

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShaderID);

	GLint fragStatus = GL_FALSE;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragStatus);
	if(fragStatus == GL_FALSE)
	{
		printf("Fragment shader failed to compile, status %d\n", vertStatus);
		exit(-1);
	}

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	printf("Linked program\n");

	GLint progStatus = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &progStatus);
	if(progStatus == GL_FALSE)
	{
		printf("Program failed to link, status %d\n", progStatus);
		exit(-1);
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

Display* display_createDisplay(int width, int height)
{
	if(!glfwInit()) printf("GLFW was not initialized.\n");

	printf("GLFW initialized.\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwSetErrorCallback(error_callback);
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);
	if(!window) printf("Unable to create window");
	printf("Window created.\n");

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	
	// callbacks
	glfwSetKeyCallback(window, callback_key);
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	cube = object_buildObject(vertexBufferData, sizeof(vertexBufferData), uvBufferData, sizeof(uvBufferData), "textureTest.DDS");

	GLuint programID = buildProgram();
	
	Display* display = malloc(sizeof(Display));
	display-> window = window;
	display-> width = width;
	display-> height = height;
	display-> programID = programID;

	return display;
}

void display_tick(Display* display)
{
	glfwSwapBuffers(display-> window);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(display-> programID);

	mat4x4 model, view, projection, mvp;

	vec3 eye = {4,3,2};
	vec3 target = {0,0,0};
	vec3 orientation = {0,1,0};

	mat4x4_perspective(projection, 45, (float) display-> width / (float) display-> height, 0.1, 100);
	mat4x4_identity(model);
	mat4x4_look_at(view, eye, target, orientation);
	mat4x4_identity(mvp);
	mat4x4_mul(mvp, mvp, projection);
	mat4x4_mul(mvp, mvp, view);
	mat4x4_mul(mvp, mvp, model);
	GLuint matrixID = glGetUniformLocation(display-> programID, "mvp");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

	// vec3 model
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cube-> vertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// vec3 vertexUV
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cube-> uvBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glBindTexture(GL_TEXTURE_2D, cube-> textureID);

	glDrawArrays(GL_TRIANGLES, 0, cube-> triangleCount);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	int error = glGetError();
	if(error != 0) {
		printf("%d\n", error);
		exit(-1);
	}
}

void display_destroy(Display* display)
{
	glfwDestroyWindow(display-> window);
	glfwTerminate();
}

int display_isWindowClosing(Display* display)
{
	return glfwWindowShouldClose(display-> window);
}